from PIL import Image, ImageDraw, ImageFont
import qrcode


class BaseCard:
    def __init__(self):
        self.qr = qrcode.QRCode(
            version=None,
            error_correction=qrcode.constants.ERROR_CORRECT_L,
            box_size=2,
            border=2,
        )
        self.background = Image.new("1", (296, 128), "white")
        
    def show(self):
        self.background.show()
        
    def binary(self) -> bytes:
        width, height = self.background.size
        binary = bytearray(int(width*height/8))
        for i in range(width):
            for j in range(height):
                if self.background.getpixel((i, j)) > 127:
                    binary[int(j/8 + i*height/8)] |= (0x80 >> (j % 8))
                else:
                    binary[int(j/8 + i*height/8)] &= ~(0x80 >> (j % 8))
        return binary


class WIFICard(BaseCard):
    def make(self, ssid: str, pwd: str, security: str = 'WPA', hidden: bool = False):
        """
        Generate WIFI Card
        Args:
            ssid (str): WIFI SSID 20 char max recommended
            pwd (str): password 36 char max recommended
            security (str, optional): Wifi security. Defaults to 'WPA'.
            hidden (bool, optional): Wifi visibility. Defaults to False.
        """
        self.qr.clear()
        wstr = f'WIFI:T:{security};S:{ssid};P:{pwd};'
        if hidden:
            wstr += 'H:true;'
        self.qr.add_data(wstr)
        self.qr.make(fit=True)
        self.background = Image.new("1", (296, 128), "white")
        img = self.qr.make_image().get_image()
        x_off = int(148 - img.width / 2)
        y_off = int(64 - img.height / 2)
        self.background.paste(img, (x_off, y_off))
        
        draw = ImageDraw.Draw(self.background)
        draw.text((235, 5), "<MENU", 0, ImageFont.truetype('./font/SourceHanSans-Regular.otf', 15))
        draw.text((6, 4), ssid, 0, ImageFont.truetype('./font/SourceHanSans-Regular.otf', 15))
        draw.text((148, 108), 'Password', 0, ImageFont.truetype('./font/SourceHanSans-Regular.otf', 10), 'mm')
        draw.text((148, 120), pwd, 0, ImageFont.truetype('./font/SourceHanSans-Regular.otf', 13), 'mm')
    
    
class NameCard(BaseCard):
    def make(self, name: str, **kargs):
        self.qr.clear()
        wstr = f'BEGIN:VCARD\nVERSION:3.0\nFN:{name}\n'
        if "TEL" in kargs:
            wstr += f'TEL:{kargs['TEL']}\n'
        if "EMAIL" in kargs:
            wstr += f'EMAIL:{kargs['EMAIL']}\n'
        wstr += 'END:VCARD'
        self.qr.add_data(wstr)
        self.qr.make(fit=True)
        self.background = Image.new("1", (296, 128), "white")
        img = self.qr.make_image().get_image()
        x_off = int(148 - img.width / 2)
        y_off = int(55 - img.height / 2)
        self.background.paste(img, (x_off, y_off))
        
        draw = ImageDraw.Draw(self.background)
        draw.text((235, 5), "<MENU", 0, ImageFont.truetype('./font/SourceHanSans-Regular.otf', 15))
        draw.text((148, 105), name, 0, ImageFont.truetype('./font/SourceHanSans-Regular.otf', 20), 'mm')
