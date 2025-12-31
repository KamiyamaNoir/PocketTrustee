#include "bsp_core.h"
#include "main.h"
#include "usbd_core.h"
#include "usbd_hid.h"

#define USBD_VID           0xffff
#define USBD_PID           0xffff
#define USBD_MAX_POWER     100
#define USBD_LANGID_STRING 1033

#define HID_INT_EP          0x81
#define HID_INT_EP_SIZE     8
#define HID_INT_EP_INTERVAL 10

#define USB_HID_CONFIG_DESC_SIZ       34
#define HID_KEYBOARD_REPORT_DESC_SIZE 63

static constexpr uint8_t device_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0002, 0x01)
};

static constexpr uint8_t config_descriptor[] = {
    USB_CONFIG_DESCRIPTOR_INIT(USB_HID_CONFIG_DESC_SIZ, 0x01, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),

    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x00,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x01,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                          /* bCountryCode: Hardware target country */
    0x01,                          /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                          /* bDescriptorType */
    HID_KEYBOARD_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HID_INT_EP,                   /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    HID_INT_EP_SIZE,              /* wMaxPacketSize: 4 Byte max */
    0x00,
    HID_INT_EP_INTERVAL, /* bInterval: Polling Interval */
    /* 34 */
};

static constexpr uint8_t device_quality_descriptor[] = {
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x00,
    0x00,
};

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 }, /* Langid */
    "KamiyamaNoir",                  /* Manufacturer */
    "PocketTrustee",         /* Product */
    "233",                 /* Serial Number */
};

static const uint8_t *device_descriptor_callback(uint8_t speed)
{
    return device_descriptor;
}

static const uint8_t *config_descriptor_callback(uint8_t speed)
{
    return config_descriptor;
}

static const uint8_t *device_quality_descriptor_callback(uint8_t speed)
{
    return device_quality_descriptor;
}

static const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    if (index > 3) {
        return nullptr;
    }
    return string_descriptors[index];
}

constexpr struct usb_descriptor hid_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .device_quality_descriptor_callback = device_quality_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback
};

static const uint8_t hid_keyboard_report_desc[HID_KEYBOARD_REPORT_DESC_SIZE] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)
    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs)
    0x95, 0x05, // REPORT_COUNT (5)
    0x75, 0x01, // REPORT_SIZE (1)
    0x05, 0x08, // USAGE_PAGE (LEDs)
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    0x29, 0x05, // USAGE_MAXIMUM (Kana)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x03, // REPORT_SIZE (3)
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0xFF, // LOGICAL_MAXIMUM (255)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)
    0xc0        // END_COLLECTION
};

constexpr uint8_t ASCII_TO_HID_BYTE2[] = {
    HID_KBD_USAGE_SPACE,        // 32
    HID_KBD_USAGE_EXCLAM,       // 33
    HID_KBD_USAGE_DQUOUTE,      // 34
    HID_KBD_USAGE_POUND,        // 35
    HID_KBD_USAGE_DOLLAR,       // 36
    HID_KBD_USAGE_PERCENT,      // 37
    HID_KBD_USAGE_AMPERSAND,    // 38
    HID_KBD_USAGE_SQUOTE,       // 39
    HID_KBD_USAGE_LPAREN,       // 40
    HID_KBD_USAGE_RPAREN,       // 41
    HID_KBD_USAGE_ASTERISK,     // 42
    HID_KBD_USAGE_PLUS,         // 43
    HID_KBD_USAGE_KPDCOMMA,     // 44
    HID_KBD_USAGE_HYPHEN,       // 45
    HID_KBD_USAGE_PERIOD,       // 46
    HID_KBD_USAGE_DIV,          // 47
    HID_KBD_USAGE_0,            // 48
    HID_KBD_USAGE_1,            // 49
    HID_KBD_USAGE_1 + 1,        // 50
    HID_KBD_USAGE_1 + 2,        // 51
    HID_KBD_USAGE_1 + 3,        // 52
    HID_KBD_USAGE_1 + 4,        // 53
    HID_KBD_USAGE_1 + 5,        // 54
    HID_KBD_USAGE_1 + 6,        // 55
    HID_KBD_USAGE_1 + 7,        // 56
    HID_KBD_USAGE_1 + 8,        // 57
    HID_KBD_USAGE_COLON,        // 58
    HID_KBD_USAGE_SEMICOLON,    // 59
    HID_KBD_USAGE_LT,           // 60
    HID_KBD_USAGE_EQUAL,        // 61
    HID_KBD_USAGE_GT,           // 62
    HID_KBD_USAGE_QUESTION,     // 63
    HID_KBD_USAGE_AT,           // 64
    HID_KBD_USAGE_A,
    HID_KBD_USAGE_A + 1,
    HID_KBD_USAGE_A + 2,
    HID_KBD_USAGE_A + 3,
    HID_KBD_USAGE_A + 4,
    HID_KBD_USAGE_A + 5,
    HID_KBD_USAGE_A + 6,
    HID_KBD_USAGE_A + 7,
    HID_KBD_USAGE_A + 8,
    HID_KBD_USAGE_A + 9,
    HID_KBD_USAGE_A + 10,
    HID_KBD_USAGE_A + 11,
    HID_KBD_USAGE_A + 12,
    HID_KBD_USAGE_A + 13,
    HID_KBD_USAGE_A + 14,
    HID_KBD_USAGE_A + 15,
    HID_KBD_USAGE_A + 16,
    HID_KBD_USAGE_A + 17,
    HID_KBD_USAGE_A + 18,
    HID_KBD_USAGE_A + 19,
    HID_KBD_USAGE_A + 20,
    HID_KBD_USAGE_A + 21,
    HID_KBD_USAGE_A + 22,
    HID_KBD_USAGE_A + 23,
    HID_KBD_USAGE_A + 24,
    HID_KBD_USAGE_A + 25,
    HID_KBD_USAGE_LBRACKET,     // 91
    HID_KBD_USAGE_BSLASH,       // 92
    HID_KBD_USAGE_RBRACKET,     // 93
    HID_KBD_USAGE_CARAT,        // 94
    HID_KBD_USAGE_UNDERSCORE,   // 95
    HID_KBD_USAGE_GACCENT,      // 96
    HID_KBD_USAGE_A,
    HID_KBD_USAGE_A + 1,
    HID_KBD_USAGE_A + 2,
    HID_KBD_USAGE_A + 3,
    HID_KBD_USAGE_A + 4,
    HID_KBD_USAGE_A + 5,
    HID_KBD_USAGE_A + 6,
    HID_KBD_USAGE_A + 7,
    HID_KBD_USAGE_A + 8,
    HID_KBD_USAGE_A + 9,
    HID_KBD_USAGE_A + 10,
    HID_KBD_USAGE_A + 11,
    HID_KBD_USAGE_A + 12,
    HID_KBD_USAGE_A + 13,
    HID_KBD_USAGE_A + 14,
    HID_KBD_USAGE_A + 15,
    HID_KBD_USAGE_A + 16,
    HID_KBD_USAGE_A + 17,
    HID_KBD_USAGE_A + 18,
    HID_KBD_USAGE_A + 19,
    HID_KBD_USAGE_A + 20,
    HID_KBD_USAGE_A + 21,
    HID_KBD_USAGE_A + 22,
    HID_KBD_USAGE_A + 23,
    HID_KBD_USAGE_A + 24,
    HID_KBD_USAGE_A + 25,
    HID_KBD_USAGE_LBRACE,       // 123
    HID_KBD_USAGE_VERTBAR,      // 124
    HID_KBD_USAGE_RBRACE,       // 125
    HID_KBD_USAGE_TILDE         // 126
};

constexpr uint8_t ASCII_TO_HID_BYTE0[] = {
    0,                          // 32: ' ' → 无 Shift
    HID_MODIFIER_LSHIFT,        // 33: '!' → Shift + 1
    HID_MODIFIER_LSHIFT,        // 34: '"' → Shift + '
    HID_MODIFIER_LSHIFT,        // 35: '#' → Shift + 3
    HID_MODIFIER_LSHIFT,        // 36: '$' → Shift + 4
    HID_MODIFIER_LSHIFT,        // 37: '%' → Shift + 5
    HID_MODIFIER_LSHIFT,        // 38: '&' → Shift + 7
    0,                          // 39: ''' → 默认键（无需 Shift）
    HID_MODIFIER_LSHIFT,        // 40: '(' → Shift + 9
    HID_MODIFIER_LSHIFT,        // 41: ')' → Shift + 0
    HID_MODIFIER_LSHIFT,        // 42: '*' → Shift + 8
    HID_MODIFIER_LSHIFT,        // 43: '+' → Shift + =
    0,                          // 44: ',' → 默认键
    0,                          // 45: '-' → 默认键
    0,                          // 46: '.' → 默认键
    0,                          // 47: '/' → 默认键
    0,                          // 48: '0' → 默认键
    0,                          // 49: '1' → 默认键
    0,                          // 50: '2' → 默认键
    0,                          // 51: '3' → 默认键
    0,                          // 52: '4' → 默认键
    0,                          // 53: '5' → 默认键
    0,                          // 54: '6' → 默认键
    0,                          // 55: '7' → 默认键
    0,                          // 56: '8' → 默认键
    0,                          // 57: '9' → 默认键
    HID_MODIFIER_LSHIFT,        // 58: ':' → Shift + ;
    0,                          // 59: ';' → 默认键
    HID_MODIFIER_LSHIFT,        // 60: '<' → Shift + ,
    0,                          // 61: '=' → 默认键
    HID_MODIFIER_LSHIFT,        // 62: '>' → Shift + .
    HID_MODIFIER_LSHIFT,        // 63: '?' → Shift + /
    HID_MODIFIER_LSHIFT,        // 64: '@' → Shift + 2
    HID_MODIFIER_LSHIFT,        // 65: 'A' → Shift + a
    HID_MODIFIER_LSHIFT,        // 66: 'B' → Shift + b
    HID_MODIFIER_LSHIFT,        // 67: 'C' → Shift + c
    HID_MODIFIER_LSHIFT,        // 68: 'D' → Shift + d
    HID_MODIFIER_LSHIFT,        // 69: 'E' → Shift + e
    HID_MODIFIER_LSHIFT,        // 70: 'F' → Shift + f
    HID_MODIFIER_LSHIFT,        // 71: 'G' → Shift + g
    HID_MODIFIER_LSHIFT,        // 72: 'H' → Shift + h
    HID_MODIFIER_LSHIFT,        // 73: 'I' → Shift + i
    HID_MODIFIER_LSHIFT,        // 74: 'J' → Shift + j
    HID_MODIFIER_LSHIFT,        // 75: 'K' → Shift + k
    HID_MODIFIER_LSHIFT,        // 76: 'L' → Shift + l
    HID_MODIFIER_LSHIFT,        // 77: 'M' → Shift + m
    HID_MODIFIER_LSHIFT,        // 78: 'N' → Shift + n
    HID_MODIFIER_LSHIFT,        // 79: 'O' → Shift + o
    HID_MODIFIER_LSHIFT,        // 80: 'P' → Shift + p
    HID_MODIFIER_LSHIFT,        // 81: 'Q' → Shift + q
    HID_MODIFIER_LSHIFT,        // 82: 'R' → Shift + r
    HID_MODIFIER_LSHIFT,        // 83: 'S' → Shift + s
    HID_MODIFIER_LSHIFT,        // 84: 'T' → Shift + t
    HID_MODIFIER_LSHIFT,        // 85: 'U' → Shift + u
    HID_MODIFIER_LSHIFT,        // 86: 'V' → Shift + v
    HID_MODIFIER_LSHIFT,        // 87: 'W' → Shift + w
    HID_MODIFIER_LSHIFT,        // 88: 'X' → Shift + x
    HID_MODIFIER_LSHIFT,        // 89: 'Y' → Shift + y
    HID_MODIFIER_LSHIFT,        // 90: 'Z' → Shift + z
    0,                          // 91: '[' → 默认键
    0,                          // 92: '\' → 默认键
    0,                          // 93: ']' → 默认键
    HID_MODIFIER_LSHIFT,        // 94: '^' → Shift + 6
    HID_MODIFIER_LSHIFT,        // 95: '_' → Shift + -
    0,                          // 96: '`' → 默认键
    0,                          // 97: 'a' → 默认键
    0,                          // 98: 'b' → 默认键
    0,                          // 99: 'c' → 默认键
    0,                          // 100: 'd' → 默认键
    0,                          // 101: 'e' → 默认键
    0,                          // 102: 'f' → 默认键
    0,                          // 103: 'g' → 默认键
    0,                          // 104: 'h' → 默认键
    0,                          // 105: 'i' → 默认键
    0,                          // 106: 'j' → 默认键
    0,                          // 107: 'k' → 默认键
    0,                          // 108: 'l' → 默认键
    0,                          // 109: 'm' → 默认键
    0,                          // 110: 'n' → 默认键
    0,                          // 111: 'o' → 默认键
    0,                          // 112: 'p' → 默认键
    0,                          // 113: 'q' → 默认键
    0,                          // 114: 'r' → 默认键
    0,                          // 115: 's' → 默认键
    0,                          // 116: 't' → 默认键
    0,                          // 117: 'u' → 默认键
    0,                          // 118: 'v' → 默认键
    0,                          // 119: 'w' → 默认键
    0,                          // 120: 'x' → 默认键
    0,                          // 121: 'y' → 默认键
    0,                          // 122: 'z' → 默认键
    HID_MODIFIER_LSHIFT,        // 123: '{' → Shift + [
    HID_MODIFIER_LSHIFT,        // 124: '|' → Shift +
    HID_MODIFIER_LSHIFT,        // 125: '}' → Shift + ]
    HID_MODIFIER_LSHIFT,        // 126: '~' → Shift + `
};

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t hid_state = HID_STATE_IDLE;

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            break;
        case USBD_EVENT_CONNECTED:
            break;
        case USBD_EVENT_DISCONNECTED:
            break;
        case USBD_EVENT_RESUME:
            break;
        case USBD_EVENT_SUSPEND:
            break;
        case USBD_EVENT_CONFIGURED:
            hid_state = HID_STATE_IDLE;
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            break;

        default:
            break;
    }
}

void usbd_hid_int_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_state = HID_STATE_IDLE;
}

static struct usbd_endpoint hid_in_ep = {
    .ep_addr = HID_INT_EP,
    .ep_cb = usbd_hid_int_callback,
};

struct usbd_interface intf0;

extern volatile bool usbd_has_initialized;

void hid_keyboard_init()
{
    if (usbd_has_initialized) return;
    usbd_desc_register(0, &hid_descriptor);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf0, hid_keyboard_report_desc, HID_KEYBOARD_REPORT_DESC_SIZE));
    usbd_add_endpoint(0, &hid_in_ep);

    usbd_initialize(0, USB_BASE, usbd_event_handler);
    usbd_has_initialized = true;
    core::StartIdealTask();
}

void hid_keyboard_char(char chr)
{
    if (chr < 32 || chr > 126)
        return;

    uint16_t timeout = 100;
    uint32_t Tickstart = HAL_GetTick();
    uint8_t sendbuffer[8] = { ASCII_TO_HID_BYTE0[chr-32], 0x00, ASCII_TO_HID_BYTE2[chr-32], 0x00, 0x00, 0x00, 0x00, 0x00 };
    hid_state = HID_STATE_BUSY;
    usbd_ep_start_write(0, HID_INT_EP, sendbuffer, 8);
    while (hid_state == HID_STATE_BUSY) {
        if (((HAL_GetTick() - Tickstart) > timeout) || (timeout == 0))
        {
            return;
        }
    }

    timeout = 100;
    Tickstart = HAL_GetTick();
    sendbuffer[0] = 0;
    sendbuffer[2] = 0;
    hid_state = HID_STATE_BUSY;
    usbd_ep_start_write(0, HID_INT_EP, sendbuffer, 8);
    while (hid_state == HID_STATE_BUSY) {
        if (((HAL_GetTick() - Tickstart) > timeout) || (timeout == 0))
        {
            return;
        }
    }
}

void hid_keyboard_string(const char* str)
{
    for (uint32_t i = 0;;i++)
    {
        if (str[i] == '\0')
            break;
        hid_keyboard_char(str[i]);
    }
}

