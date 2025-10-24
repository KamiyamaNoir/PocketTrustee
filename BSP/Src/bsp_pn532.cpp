#include "bsp_pn532.h"
#include "usart.h"
#include "simple_buffer.h"
#include <cstring>

using namespace pn532;

static uint8_t _uid[7] {}; // ISO14443A uid
static uint8_t _uidLen {}; // uid len
static uint8_t _key[6] {}; // Mifare Classic key
static uint8_t _inListedTag {}; // Tg number of inlisted tag.

uint8_t pn532ack[] = {
    0x00, 0x00, 0xFF,
    0x00, 0xFF, 0x00
}; ///< ACK message from PN532
uint8_t pn532response_firmwarevers[] = {
    0x00, 0x00, 0xFF,
    0x06, 0xFA, 0xD5
}; ///< Expected firmware version message from PN532


uint8_t pn532_receive_buffer[128];
static uint8_t pn532_ring_buffer[256];
auto pn532_buffer_hander = StaticRingBuffer(pn532_ring_buffer, 256);


#define PN532_PACKBUFFSIZ 64                ///< Packet buffer size in bytes
uint8_t pn532_packetbuffer[PN532_PACKBUFFSIZ];

__STATIC_INLINE void TransmitData(const uint8_t* data, uint16_t len)
{
    HAL_UART_Transmit(&huart3, data, len, HAL_MAX_DELAY);
}


/**************************************************************************/
/*!
    @brief  Reads n bytes of data from the PN532 via SPI or I2C.

    @param  buff      Pointer to the buffer where data will be written
    @param  n         Number of bytes to be read
*/
/**************************************************************************/
static void readdata(uint8_t* buff, uint8_t n)
{
    pn532_buffer_hander.read(buff, n);
}

/**************************************************************************/
/*!
    @brief  Tries to read the SPI or I2C ACK signal
*/
/**************************************************************************/
static bool readack()
{
    uint8_t ackbuff[6];
    readdata(ackbuff, 6);

    return (0 == memcmp((char*)ackbuff, (char*)pn532ack, 6));
}

/**************************************************************************/
/*!
    @brief  Return true if the PN532 is ready with a response.
*/
/**************************************************************************/
static bool isready()
{
    // Serial ready check based on non-zero read buffer
    return !pn532_buffer_hander.is_empty();
}

/**************************************************************************/
/*!
    @brief  Waits until the PN532 is ready.

    @param  timeout   Timeout before giving up
*/
/**************************************************************************/
static bool waitready(const uint16_t timeout)
{
    uint16_t timer = 0;
    while (!isready())
    {
        if (timeout != 0)
        {
            timer += 10;
            if (timer > timeout)
            {
                return false;
            }
        }
        HAL_Delay(10);
    }
    return true;
}

/**************************************************************************/
/*!
    @brief  Writes a command to the PN532, automatically inserting the
            preamble and required frame details (checksum, len, etc.)

    @param  cmd       Pointer to the command buffer
    @param  cmdlen    Command length in bytes
*/
/**************************************************************************/
static void writecommand(const uint8_t* cmd, uint8_t cmdlen)
{
    uint8_t packet[8 + cmdlen];
    uint8_t LEN = cmdlen + 1;

    packet[0] = PN532_PREAMBLE;
    packet[1] = PN532_STARTCODE1;
    packet[2] = PN532_STARTCODE2;
    packet[3] = LEN;
    packet[4] = ~LEN + 1;
    packet[5] = PN532_HOSTTOPN532;
    uint8_t sum = 0;
    for (uint8_t i = 0; i < cmdlen; i++)
    {
        packet[6 + i] = cmd[i];
        sum += cmd[i];
    }
    packet[6 + cmdlen] = ~(PN532_HOSTTOPN532 + sum) + 1;
    packet[7 + cmdlen] = PN532_POSTAMBLE;

    TransmitData(packet, cmdlen + 8);
}

/**************************************************************************/
/*!
    @brief  Setups the HW

    @returns  true if successful, otherwise false
*/
/**************************************************************************/
void pn532::init()
{
    reset(); // HW reset - put in known state
    HAL_Delay(10);
    // HAL_UARTEx_ReceiveToIdle_IT(&huart3, pn532_receive_buffer, 128);
    wakeup(); // hey! wakeup!
}

/**************************************************************************/
/*!
    @brief  Perform a hardware reset. Requires reset pin to have been provided.
*/
/**************************************************************************/
void pn532::reset()
{
    // see Datasheet p.209, Fig.48 for timings
    PN_PD_GPIO_Port->BRR = PN_PD_Pin;
    HAL_Delay(1); // min 20ns
    PN_PD_GPIO_Port->BSRR = PN_PD_Pin;
    HAL_Delay(2); // max 2ms
}

/**************************************************************************/
/*!
    @brief  Wakeup from LowVbat mode into Normal Mode.
*/
/**************************************************************************/
void pn532::wakeup()
{
    constexpr uint8_t w[3] = {0x55, 0x00, 0x00};
    TransmitData(w, 3);
    HAL_Delay(2);
    
    // need to config SAM to stay in Normal Mode
    SAMConfig();
}

/**************************************************************************/
/*!
    @brief  Checks the firmware version of the PN5xx chip

    @returns  The chip's firmware version and ID
*/
/**************************************************************************/
uint32_t pn532::getFirmwareVersion()
{
    pn532_packetbuffer[0] = PN532_COMMAND_GETFIRMWAREVERSION;

    if (!sendCommandCheckAck(pn532_packetbuffer, 1))
    {
        return 0;
    }

    // read data packet
    readdata(pn532_packetbuffer, 13);

    // check some basic stuff
    if (0 != memcmp(pn532_packetbuffer,
                    pn532response_firmwarevers, 6))
    {
        return 0;
    }

    int offset = 7;
    uint32_t response = pn532_packetbuffer[offset++];
    response <<= 8;
    response |= pn532_packetbuffer[offset++];
    response <<= 8;
    response |= pn532_packetbuffer[offset++];
    response <<= 8;
    response |= pn532_packetbuffer[offset];

    return response;
}

/**************************************************************************/
/*!
    @brief  Sends a command and waits a specified period for the ACK

    @param  cmd       Pointer to the command buffer
    @param  cmdlen    The size of the command in bytes
    @param  timeout   timeout before giving up

    @returns  1 if everything is OK, 0 if timeout occured before an
              ACK was recieved
*/
/**************************************************************************/
// default timeout of one second
bool pn532::sendCommandCheckAck(uint8_t* cmd, uint8_t cmdlen, uint16_t timeout)
{
    // write the command
    writecommand(cmd, cmdlen);
    
    // Wait for chip to say its ready!
    if (!waitready(timeout))
    {
        return false;
    }

    // read acknowledgement
    if (!readack())
    {
        return false;
    }
    
    // Wait for chip to say its ready!
    if (!waitready(timeout))
    {
        return false;
    }

    return true; // ack'd command
}


/**************************************************************************/
/*!
    @brief   Configures the SAM (Secure Access Module)
    @return  true on success, false otherwise.
*/
/**************************************************************************/
bool pn532::SAMConfig()
{
    pn532_packetbuffer[0] = PN532_COMMAND_SAMCONFIGURATION;
    pn532_packetbuffer[1] = 0x01; // normal mode;
    pn532_packetbuffer[2] = 0x14; // timeout 50ms * 20 = 1 second
    pn532_packetbuffer[3] = 0x01; // use IRQ pin!

    if (!sendCommandCheckAck(pn532_packetbuffer, 4))
        return false;

    // read data packet
    readdata(pn532_packetbuffer, 9);

    int offset = 6;
    return (pn532_packetbuffer[offset] == 0x15);
}

/**************************************************************************/
/*!
    Sets the MxRtyPassiveActivation byte of the RFConfiguration register

    @param  maxRetries    0xFF to wait forever, 0x00..0xFE to timeout
                          after mxRetries

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool pn532::setPassiveActivationRetries(uint8_t maxRetries)
{
    pn532_packetbuffer[0] = PN532_COMMAND_RFCONFIGURATION;
    pn532_packetbuffer[1] = 5; // Config item 5 (MaxRetries)
    pn532_packetbuffer[2] = 0xFF; // MxRtyATR (default = 0xFF)
    pn532_packetbuffer[3] = 0x01; // MxRtyPSL (default = 0x01)
    pn532_packetbuffer[4] = maxRetries;

    if (!sendCommandCheckAck(pn532_packetbuffer, 5))
        return false; // no ACK

    // read data packet
    readdata(pn532_packetbuffer, 9);

    int offset = 6;
    return (pn532_packetbuffer[offset] == 0x33);
}

/***** ISO14443A Commands ******/

/**************************************************************************/
/*!
    @brief   Waits for an ISO14443A target to enter the field and reads
             its ID.

    @param   cardbaudrate  Baud rate of the card
    @param   uid           Pointer to the array that will be populated
                           with the card's UID (up to 7 bytes)
    @param   uidLength     Pointer to the variable that will hold the
                           length of the card's UID.
    @param   timeout       Timeout in milliseconds.

    @return  1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool pn532::readPassiveTargetID(uint8_t cardbaudrate, uint8_t* uid,
                                         uint8_t* uidLength, uint16_t timeout)
{
    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 1; // max 1 cards at once (we can set this to 2 later)
    pn532_packetbuffer[2] = cardbaudrate;

    if (!sendCommandCheckAck(pn532_packetbuffer, 3, timeout))
    {
        return false; // no cards read
    }

    return readDetectedPassiveTargetID(uid, uidLength);
}

/**************************************************************************/
/*!
    @brief   Put the reader in detection mode, non blocking so interrupts
             must be enabled.
    @param   cardbaudrate  Baud rate of the card
    @return  1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool pn532::startPassiveTargetIDDetection(uint8_t cardbaudrate)
{
    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 1; // max 1 cards at once (we can set this to 2 later)
    pn532_packetbuffer[2] = cardbaudrate;

    return sendCommandCheckAck(pn532_packetbuffer, 3);
}

/**************************************************************************/
/*!
    Reads the ID of the passive target the reader has deteceted.

    @param  uid           Pointer to the array that will be populated
                          with the card's UID (up to 7 bytes)
    @param  uidLength     Pointer to the variable that will hold the
                          length of the card's UID.

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
bool pn532::readDetectedPassiveTargetID(uint8_t* uid,
                                                 uint8_t* uidLength)
{
    // read data packet
    readdata(pn532_packetbuffer, 20);
    // check some basic stuff

    /* ISO14443A card response should be in the following format:

      byte            Description
      -------------   ------------------------------------------
      b0..6           Frame header and preamble
      b7              Tags Found
      b8              Tag Number (only one used in this example)
      b9..10          SENS_RES
      b11             SEL_RES
      b12             NFCID Length
      b13..NFCIDLen   NFCID                                      */

    if (pn532_packetbuffer[7] != 1)
        return false;


    /* Card appears to be Mifare Classic */
    *uidLength = pn532_packetbuffer[12];

    for (uint8_t i = 0; i < pn532_packetbuffer[12]; i++)
    {
        uid[i] = pn532_packetbuffer[13 + i];
    }


    return true;
}

/**************************************************************************/
/*!
    @brief   Exchanges an APDU with the currently inlisted peer

    @param   send            Pointer to data to send
    @param   sendLength      Length of the data to send
    @param   response        Pointer to response data
    @param   responseLength  Pointer to the response data length
    @return  true on success, false otherwise.
*/
/**************************************************************************/
bool pn532::inDataExchange(const uint8_t* send, uint8_t sendLength,
                                    uint8_t* response,
                                    uint8_t* responseLength)
{
    if (sendLength > PN532_PACKBUFFSIZ - 2)
    {
        return false;
    }
    uint8_t i;

    pn532_packetbuffer[0] = 0x40; // PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = _inListedTag;
    for (i = 0; i < sendLength; ++i)
    {
        pn532_packetbuffer[i + 2] = send[i];
    }

    if (!sendCommandCheckAck(pn532_packetbuffer, sendLength + 2, 1000))
    {
        return false;
    }

    if (!waitready(1000))
    {
        return false;
    }

    readdata(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    if (pn532_packetbuffer[0] == 0 && pn532_packetbuffer[1] == 0 &&
        pn532_packetbuffer[2] == 0xff)
    {
        uint8_t length = pn532_packetbuffer[3];
        if (pn532_packetbuffer[4] != static_cast<uint8_t>(~length + 1))
        {
            return false;
        }
        if (pn532_packetbuffer[5] == PN532_PN532TOHOST &&
            pn532_packetbuffer[6] == PN532_RESPONSE_INDATAEXCHANGE)
        {
            if ((pn532_packetbuffer[7] & 0x3f) != 0)
            {
                return false;
            }

            length -= 3;

            if (length > *responseLength)
            {
                length = *responseLength; // silent truncation...
            }

            for (i = 0; i < length; ++i)
            {
                response[i] = pn532_packetbuffer[8 + i];
            }
            *responseLength = length;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/**************************************************************************/
/*!
    @brief   'InLists' a passive target. PN532 acting as reader/initiator,
             peer acting as card/responder.
    @return  true on success, false otherwise.
*/
/**************************************************************************/
bool pn532::inListPassiveTarget()
{
    pn532_packetbuffer[0] = PN532_COMMAND_INLISTPASSIVETARGET;
    pn532_packetbuffer[1] = 1;
    pn532_packetbuffer[2] = 0;

    if (!sendCommandCheckAck(pn532_packetbuffer, 3, 1000))
    {
        return false;
    }

    if (!waitready(30000))
    {
        return false;
    }

    readdata(pn532_packetbuffer, sizeof(pn532_packetbuffer));

    if (pn532_packetbuffer[0] == 0 && pn532_packetbuffer[1] == 0 &&
        pn532_packetbuffer[2] == 0xff)
    {
        uint8_t length = pn532_packetbuffer[3];
        if (pn532_packetbuffer[4] != static_cast<uint8_t>(~length + 1))
        {
            return false;
        }
        if (pn532_packetbuffer[5] == PN532_PN532TOHOST &&
            pn532_packetbuffer[6] == PN532_RESPONSE_INLISTPASSIVETARGET)
        {
            if (pn532_packetbuffer[7] != 1)
            {
                return false;
            }

            _inListedTag = pn532_packetbuffer[8];

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

/***** Mifare Classic Functions ******/

/**************************************************************************/
/*!
    @brief   Indicates whether the specified block number is the first block
             in the sector (block 0 relative to the current sector)
    @param   uiBlock  Block number to test.
    @return  true if first block, false otherwise.
*/
/**************************************************************************/
bool pn532::mifareclassic_IsFirstBlock(uint32_t uiBlock)
{
    // Test if we are in the small or big sectors
    if (uiBlock < 128)
        return ((uiBlock) % 4 == 0);
    else
        return ((uiBlock) % 16 == 0);
}

/**************************************************************************/
/*!
    @brief   Indicates whether the specified block number is the sector
             trailer.
    @param   uiBlock  Block number to test.
    @return  true if sector trailer, false otherwise.
*/
/**************************************************************************/
bool pn532::mifareclassic_IsTrailerBlock(uint32_t uiBlock)
{
    // Test if we are in the small or big sectors
    if (uiBlock < 128)
        return ((uiBlock + 1) % 4 == 0);
    else
        return ((uiBlock + 1) % 16 == 0);
}

/**************************************************************************/
/*!
    Tries to authenticate a block of memory on a MIFARE card using the
    INDATAEXCHANGE command.  See section 7.3.8 of the PN532 User Manual
    for more information on sending MIFARE and other commands.

    @param  uid           Pointer to a byte array containing the card UID
    @param  uidLen        The length (in bytes) of the card's UID (Should
                          be 4 for MIFARE Classic)
    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  keyNumber     Which key type to use during authentication
                          (0 = MIFARE_CMD_AUTH_A, 1 = MIFARE_CMD_AUTH_B)
    @param  keyData       Pointer to a byte array containing the 6 byte
                          key value

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::mifareclassic_AuthenticateBlock(uint8_t* uid,
                                                        uint8_t uidLen,
                                                        uint32_t blockNumber,
                                                        uint8_t keyNumber,
                                                        uint8_t* keyData)
{
    // uint8_t len;

    // Hang on to the key and uid data
    memcpy(_key, keyData, 6);
    memcpy(_uid, uid, uidLen);
    _uidLen = uidLen;

    // Prepare the authentication command //
    pn532_packetbuffer[0] =
        PN532_COMMAND_INDATAEXCHANGE; /* Data Exchange Header */
    pn532_packetbuffer[1] = 1; /* Max card numbers */
    pn532_packetbuffer[2] = (keyNumber) ? MIFARE_CMD_AUTH_B : MIFARE_CMD_AUTH_A;
    pn532_packetbuffer[3] =
        blockNumber; /* Block Number (1K = 0..63, 4K = 0..255 */
    memcpy(pn532_packetbuffer + 4, _key, 6);
    for (uint8_t i = 0; i < _uidLen; i++)
    {
        pn532_packetbuffer[10 + i] = _uid[i]; /* 4 byte card ID */
    }

    if (!sendCommandCheckAck(pn532_packetbuffer, 10 + _uidLen))
        return 0;

    // Read the response packet
    readdata(pn532_packetbuffer, 12);

    // check if the response is valid and we are authenticated???
    // for an auth success it should be bytes 5-7: 0xD5 0x41 0x00
    // Mifare auth error is technically byte 7: 0x14 but anything other and 0x00
    // is not good
    if (pn532_packetbuffer[7] != 0x00)
    {
        return 0;
    }

    return 1;
}

/**************************************************************************/
/*!
    Tries to read an entire 16-byte data block at the specified block
    address.

    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  data          Pointer to the byte array that will hold the
                          retrieved data (if any)

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::mifareclassic_ReadDataBlock(uint8_t blockNumber,
                                                    uint8_t* data)
{
    /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1; /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_READ; /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] =
        blockNumber; /* Block Number (0..63 for 1K, 0..255 for 4K) */

    /* Send the command */
    if (!sendCommandCheckAck(pn532_packetbuffer, 4))
    {
        // Failed to receive ACK for read command
        return 0;
    }

    /* Read the response packet */
    readdata(pn532_packetbuffer, 26);

    /* If byte 8 isn't 0x00 we probably have an error */
    if (pn532_packetbuffer[7] != 0x00)
    {
        // Unexpcted response
        return 0;
    }

    /* Copy the 16 data bytes to the output buffer        */
    /* Block content starts at byte 9 of a valid response */
    memcpy(data, pn532_packetbuffer + 8, 16);

    return 1;
}

/**************************************************************************/
/*!
    Tries to write an entire 16-byte data block at the specified block
    address.

    @param  blockNumber   The block number to authenticate.  (0..63 for
                          1KB cards, and 0..255 for 4KB cards).
    @param  data          The byte array that contains the data to write.

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::mifareclassic_WriteDataBlock(uint8_t blockNumber,
                                                     uint8_t* data)
{
    /* Prepare the first command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1; /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_WRITE; /* Mifare Write command = 0xA0 */
    pn532_packetbuffer[3] =
        blockNumber; /* Block Number (0..63 for 1K, 0..255 for 4K) */
    memcpy(pn532_packetbuffer + 4, data, 16); /* Data Payload */

    /* Send the command */
    if (!sendCommandCheckAck(pn532_packetbuffer, 20))
    {
        return 0;
    }
    HAL_Delay(10);

    /* Read the response packet */
    readdata(pn532_packetbuffer, 26);

    return 1;
}

/**************************************************************************/
/*!
    Formats a Mifare Classic card to store NDEF Records

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::mifareclassic_FormatNDEF()
{
    uint8_t sectorbuffer1[16] = {
        0x14, 0x01, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1,
        0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1
    };
    uint8_t sectorbuffer2[16] = {
        0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1,
        0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1, 0x03, 0xE1
    };
    uint8_t sectorbuffer3[16] = {
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0x78, 0x77,
        0x88, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    // Note 0xA0 0xA1 0xA2 0xA3 0xA4 0xA5 must be used for key A
    // for the MAD sector in NDEF records (sector 0)

    // Write block 1 and 2 to the card
    if (!(mifareclassic_WriteDataBlock(1, sectorbuffer1)))
        return 0;
    if (!(mifareclassic_WriteDataBlock(2, sectorbuffer2)))
        return 0;
    // Write key A and access rights card
    if (!(mifareclassic_WriteDataBlock(3, sectorbuffer3)))
        return 0;

    // Seems that everything was OK (?!)
    return 1;
}

/**************************************************************************/
/*!
    Writes an NDEF URI Record to the specified sector (1..15)

    Note that this function assumes that the Mifare Classic card is
    already formatted to work as an "NFC Forum Tag" and uses a MAD1
    file system.  You can use the NXP TagWriter app on Android to
    properly format cards for this.

    @param  sectorNumber  The sector that the URI record should be written
                          to (can be 1..15 for a 1K card)
    @param  uriIdentifier The uri identifier code (0 = none, 0x01 =
                          "http://www.", etc.)
    @param  url           The uri text to write (max 38 characters).

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::mifareclassic_WriteNDEFURI(uint8_t sectorNumber,
                                                   uint8_t uriIdentifier,
                                                   const char* url)
{
    // Figure out how long the string is
    uint8_t len = strlen(url);

    // Make sure we're within a 1K limit for the sector number
    if ((sectorNumber < 1) || (sectorNumber > 15))
        return 0;

    // Make sure the URI payload is between 1 and 38 chars
    if ((len < 1) || (len > 38))
        return 0;

    // Note 0xD3 0xF7 0xD3 0xF7 0xD3 0xF7 must be used for key A
    // in NDEF records

    // Setup the sector buffer (w/pre-formatted TLV wrapper and NDEF message)
    uint8_t sectorbuffer1[16] = {
        0x00,
        0x00,
        0x03,
        static_cast<uint8_t>(len + 5),
        0xD1,
        0x01,
        static_cast<uint8_t>(len + 1),
        0x55,
        uriIdentifier,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00,
        0x00
    };
    uint8_t sectorbuffer2[16] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t sectorbuffer3[16] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    uint8_t sectorbuffer4[16] = {
        0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7, 0x7F, 0x07,
        0x88, 0x40, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };
    if (len <= 6)
    {
        // Unlikely we'll get a url this short, but why not ...
        memcpy(sectorbuffer1 + 9, url, len);
        sectorbuffer1[len + 9] = 0xFE;
    }
    else if (len == 7)
    {
        // 0xFE needs to be wrapped around to next block
        memcpy(sectorbuffer1 + 9, url, len);
        sectorbuffer2[0] = 0xFE;
    }
    else if ((len > 7) && (len <= 22))
    {
        // Url fits in two blocks
        memcpy(sectorbuffer1 + 9, url, 7);
        memcpy(sectorbuffer2, url + 7, len - 7);
        sectorbuffer2[len - 7] = 0xFE;
    }
    else if (len == 23)
    {
        // 0xFE needs to be wrapped around to final block
        memcpy(sectorbuffer1 + 9, url, 7);
        memcpy(sectorbuffer2, url + 7, len - 7);
        sectorbuffer3[0] = 0xFE;
    }
    else
    {
        // Url fits in three blocks
        memcpy(sectorbuffer1 + 9, url, 7);
        memcpy(sectorbuffer2, url + 7, 16);
        memcpy(sectorbuffer3, url + 23, len - 24);
        sectorbuffer3[len - 22] = 0xFE;
    }

    // Now write all three blocks back to the card
    if (!(mifareclassic_WriteDataBlock(sectorNumber * 4, sectorbuffer1)))
        return 0;
    if (!(mifareclassic_WriteDataBlock((sectorNumber * 4) + 1, sectorbuffer2)))
        return 0;
    if (!(mifareclassic_WriteDataBlock((sectorNumber * 4) + 2, sectorbuffer3)))
        return 0;
    if (!(mifareclassic_WriteDataBlock((sectorNumber * 4) + 3, sectorbuffer4)))
        return 0;

    // Seems that everything was OK (?!)
    return 1;
}

/***** Mifare Ultralight Functions ******/

/**************************************************************************/
/*!
    @brief   Tries to read an entire 4-byte page at the specified address.

    @param   page        The page number (0..63 in most cases)
    @param   buffer      Pointer to the byte array that will hold the
                         retrieved data (if any)
    @return  1 on success, 0 on error.
*/
/**************************************************************************/
uint8_t pn532::mifareultralight_ReadPage(uint8_t page,
                                                  uint8_t* buffer)
{
    if (page >= 64)
    {
        return 0;
    }

    /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1; /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_READ; /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] = page; /* Page Number (0..63 in most cases) */

    /* Send the command */
    if (!sendCommandCheckAck(pn532_packetbuffer, 4))
    {
        return 0;
    }

    /* Read the response packet */
    readdata(pn532_packetbuffer, 26);

    /* If byte 8 isn't 0x00 we probably have an error */
    if (pn532_packetbuffer[7] == 0x00)
    {
        /* Copy the 4 data bytes to the output buffer         */
        /* Block content starts at byte 9 of a valid response */
        /* Note that the command actually reads 16 byte or 4  */
        /* pages at a time ... we simply discard the last 12  */
        /* bytes                                              */
        memcpy(buffer, pn532_packetbuffer + 8, 4);
    }
    else
    {
        return 0;
    }

    /* Display data for debug if requested */

    // Return OK signal
    return 1;
}

/**************************************************************************/
/*!
    Tries to write an entire 4-byte page at the specified block
    address.

    @param  page          The page number to write.  (0..63 for most cases)
    @param  data          The byte array that contains the data to write.
                          Should be exactly 4 bytes long.

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::mifareultralight_WritePage(uint8_t page,
                                                   uint8_t* data)
{
    if (page >= 64)
    {
        // Return Failed Signal
        return 0;
    }

    /* Prepare the first command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1; /* Card number */
    pn532_packetbuffer[2] =
        MIFARE_ULTRALIGHT_CMD_WRITE; /* Mifare Ultralight Write command = 0xA2 */
    pn532_packetbuffer[3] = page; /* Page Number (0..63 for most cases) */
    memcpy(pn532_packetbuffer + 4, data, 4); /* Data Payload */

    /* Send the command */
    if (!sendCommandCheckAck(pn532_packetbuffer, 8))
    {
        // Return Failed Signal
        return 0;
    }
    HAL_Delay(10);

    /* Read the response packet */
    readdata(pn532_packetbuffer, 26);

    // Return OK Signal
    return 1;
}

/***** NTAG2xx Functions ******/

/**************************************************************************/
/*!
    @brief   Tries to read an entire 4-byte page at the specified address.

    @param   page        The page number (0..63 in most cases)
    @param   buffer      Pointer to the byte array that will hold the
                         retrieved data (if any)
    @return  1 on success, 0 on error.
*/
/**************************************************************************/
uint8_t pn532::ntag2xx_ReadPage(uint8_t page, uint8_t* buffer)
{
    // TAG Type       PAGES   USER START    USER STOP
    // --------       -----   ----------    ---------
    // NTAG 203       42      4             39
    // NTAG 213       45      4             39
    // NTAG 215       135     4             129
    // NTAG 216       231     4             225

    if (page >= 231)
    {
        return 0;
    }

    /* Prepare the command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1; /* Card number */
    pn532_packetbuffer[2] = MIFARE_CMD_READ; /* Mifare Read command = 0x30 */
    pn532_packetbuffer[3] = page; /* Page Number (0..63 in most cases) */

    /* Send the command */
    if (!sendCommandCheckAck(pn532_packetbuffer, 4))
    {
        return 0;
    }

    /* Read the response packet */
    readdata(pn532_packetbuffer, 26);

    /* If byte 8 isn't 0x00 we probably have an error */
    if (pn532_packetbuffer[7] == 0x00)
    {
        /* Copy the 4 data bytes to the output buffer         */
        /* Block content starts at byte 9 of a valid response */
        /* Note that the command actually reads 16 byte or 4  */
        /* pages at a time ... we simply discard the last 12  */
        /* bytes                                              */
        memcpy(buffer, pn532_packetbuffer + 8, 4);
    }
    else
    {
        return 0;
    }

    // Return OK signal
    return 1;
}

/**************************************************************************/
/*!
    Tries to write an entire 4-byte page at the specified block
    address.

    @param  page          The page number to write.  (0..63 for most cases)
    @param  data          The byte array that contains the data to write.
                          Should be exactly 4 bytes long.

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::ntag2xx_WritePage(uint8_t page, uint8_t* data)
{
    // TAG Type       PAGES   USER START    USER STOP
    // --------       -----   ----------    ---------
    // NTAG 203       42      4             39
    // NTAG 213       45      4             39
    // NTAG 215       135     4             129
    // NTAG 216       231     4             225

    if ((page < 4) || (page > 225))
    {
        // Return Failed Signal
        return 0;
    }

    /* Prepare the first command */
    pn532_packetbuffer[0] = PN532_COMMAND_INDATAEXCHANGE;
    pn532_packetbuffer[1] = 1; /* Card number */
    pn532_packetbuffer[2] =
        MIFARE_ULTRALIGHT_CMD_WRITE; /* Mifare Ultralight Write command = 0xA2 */
    pn532_packetbuffer[3] = page; /* Page Number (0..63 for most cases) */
    memcpy(pn532_packetbuffer + 4, data, 4); /* Data Payload */

    /* Send the command */
    if (!sendCommandCheckAck(pn532_packetbuffer, 8))
    {
        // Return Failed Signal
        return 0;
    }
    HAL_Delay(10);

    /* Read the response packet */
    readdata(pn532_packetbuffer, 26);

    // Return OK Signal
    return 1;
}

/**************************************************************************/
/*!
    Writes an NDEF URI Record starting at the specified page (4..nn)

    Note that this function assumes that the NTAG2xx card is
    already formatted to work as an "NFC Forum Tag".

    @param  uriIdentifier The uri identifier code (0 = none, 0x01 =
                          "http://www.", etc.)
    @param  url           The uri text to write (null-terminated string).
    @param  dataLen       The size of the data area for overflow checks.

    @returns 1 if everything executed properly, 0 for an error
*/
/**************************************************************************/
uint8_t pn532::ntag2xx_WriteNDEFURI(uint8_t uriIdentifier, char* url,
                                             uint8_t dataLen)
{
    uint8_t pageBuffer[4] = {0, 0, 0, 0};

    // Remove NDEF record overhead from the URI data (pageHeader below)
    uint8_t wrapperSize = 12;

    // Figure out how long the string is
    uint8_t len = strlen(url);

    // Make sure the URI payload will fit in dataLen (include 0xFE trailer)
    if ((len < 1) || (len + 1 > (dataLen - wrapperSize)))
        return 0;

    // Setup the record header
    // See NFCForum-TS-Type-2-Tag_1.1.pdf for details
    uint8_t pageHeader[12] = {
        /* NDEF Lock Control TLV (must be first and always present) */
        0x01, /* Tag Field (0x01 = Lock Control TLV) */
        0x03, /* Payload Length (always 3) */
        0xA0, /* The position inside the tag of the lock bytes (upper 4 = page
               address, lower 4 = byte offset) */
        0x10, /* Size in bits of the lock area */
        0x44, /* Size in bytes of a page and the number of bytes each lock bit can
               lock (4 bit + 4 bits) */
        /* NDEF Message TLV - URI Record */
        0x03, /* Tag Field (0x03 = NDEF Message) */
        static_cast<uint8_t>(len + 5), /* Payload Length (not including 0xFE trailer) */
        0xD1, /* NDEF Record Header (TNF=0x1:Well known record + SR + ME + MB) */
        0x01, /* Type Length for the record type indicator */
        static_cast<uint8_t>(len + 1), /* Payload len */
        0x55, /* Record Type Indicator (0x55 or 'U' = URI Record) */
        uriIdentifier /* URI Prefix (ex. 0x01 = "http://www.") */
    };

    // Write 12 byte header (three pages of data starting at page 4)
    memcpy(pageBuffer, pageHeader, 4);
    if (!(ntag2xx_WritePage(4, pageBuffer)))
        return 0;
    memcpy(pageBuffer, pageHeader + 4, 4);
    if (!(ntag2xx_WritePage(5, pageBuffer)))
        return 0;
    memcpy(pageBuffer, pageHeader + 8, 4);
    if (!(ntag2xx_WritePage(6, pageBuffer)))
        return 0;

    // Write URI (starting at page 7)
    uint8_t currentPage = 7;
    char* urlcopy = url;
    while (len)
    {
        if (len < 4)
        {
            memset(pageBuffer, 0, 4);
            memcpy(pageBuffer, urlcopy, len);
            pageBuffer[len] = 0xFE; // NDEF record footer
            if (!(ntag2xx_WritePage(currentPage, pageBuffer)))
                return 0;
            // DONE!
            return 1;
        }
        else if (len == 4)
        {
            memcpy(pageBuffer, urlcopy, len);
            if (!(ntag2xx_WritePage(currentPage, pageBuffer)))
                return 0;
            memset(pageBuffer, 0, 4);
            pageBuffer[0] = 0xFE; // NDEF record footer
            currentPage++;
            if (!(ntag2xx_WritePage(currentPage, pageBuffer)))
                return 0;
            // DONE!
            return 1;
        }
        else
        {
            // More than one page of data left
            memcpy(pageBuffer, urlcopy, 4);
            if (!(ntag2xx_WritePage(currentPage, pageBuffer)))
                return 0;
            currentPage++;
            urlcopy += 4;
            len -= 4;
        }
    }

    // Seems that everything was OK (?!)
    return 1;
}


/**************************************************************************/
/*!
    @brief   set the PN532 as iso14443a Target behaving as a SmartCard
    @return  true on success, false otherwise.
    @note    Author: Salvador Mendoza (salmg.net) new functions:
             -AsTarget
             -getDataTarget
             -setDataTarget
*/
/**************************************************************************/
uint8_t pn532::AsTarget()
{
    pn532_packetbuffer[0] = 0x8C;
    uint8_t target[] = {
        0x8C, // INIT AS TARGET
        0x00, // MODE -> BITFIELD
        0x08, 0x00, // SENS_RES - MIFARE PARAMS
        0xdc, 0x44, 0x20, // NFCID1T
        0x60, // SEL_RES
        0x01, 0xfe, // NFCID2T MUST START WITH 01fe - FELICA PARAMS - POL_RES
        0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xc0,
        0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, // PAD
        0xff, 0xff, // SYSTEM CODE
        0xaa, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44,
        0x33, 0x22, 0x11, 0x01, 0x00, // NFCID3t MAX 47 BYTES ATR_RES
        0x0d, 0x52, 0x46, 0x49, 0x44, 0x49, 0x4f,
        0x74, 0x20, 0x50, 0x4e, 0x35, 0x33, 0x32 // HISTORICAL BYTES
    };
    if (!sendCommandCheckAck(target, sizeof(target)))
        return false;

    // read data packet
    readdata(pn532_packetbuffer, 8);

    int offset = 6;
    return (pn532_packetbuffer[offset] == 0x15);
}

/**************************************************************************/
/*!
    @brief   Retrieve response from the emulation mode

    @param   cmd    = data
    @param   cmdlen = data length
    @return  true on success, false otherwise.
*/
/**************************************************************************/
uint8_t pn532::getDataTarget(uint8_t* cmd, uint8_t* cmdlen)
{
    pn532_packetbuffer[0] = 0x86;
    if (!sendCommandCheckAck(pn532_packetbuffer, 1, 1000))
    {
        return false;
    }

    // read data packet
    readdata(pn532_packetbuffer, 64);
    uint8_t length = pn532_packetbuffer[3] - 3;

    // if (length > *responseLength) {// Bug, should avoid it in the reading
    // target data
    //  length = *responseLength; // silent truncation...
    //}

    for (int i = 0; i < length; ++i)
    {
        cmd[i] = pn532_packetbuffer[8 + i];
    }
    *cmdlen = length;
    return true;
}

/**************************************************************************/
/*!
    @brief   Set data in PN532 in the emulation mode

    @param   cmd    = data
    @param   cmdlen = data length
    @return  true on success, false otherwise.
*/
/**************************************************************************/
uint8_t pn532::setDataTarget(uint8_t* cmd, uint8_t cmdlen)
{
    // cmd1[0] = 0x8E; Must!

    if (!sendCommandCheckAck(cmd, cmdlen))
        return false;

    // read data packet
    readdata(pn532_packetbuffer, 8);
    uint8_t length = pn532_packetbuffer[3] - 3;
    for (int i = 0; i < length; ++i)
    {
        cmd[i] = pn532_packetbuffer[8 + i];
    }

    int offset = 6;
    return (pn532_packetbuffer[offset] == 0x15);
}

