from PIL import Image
import qrcode


class WIFICard:
    def __init__(self, ssid: str, pwd: str, type: str = 'WPA', hidden: bool = False):
        self.ssid = ssid
        self.pwd = pwd
        self.qr = qrcode.QRCode(
            version=None,
            error_correction=qrcode.constants.ERROR_CORRECT_L,
            box_size=2,
            border=2,
        )
        wstr = f'WIFI:T:{type};S:{ssid};P:{pwd};'
        if hidden:
            wstr += 'H:true;'
        self.qr.add_data(wstr)
        self.qr.make(True)
        self.background = Image.new("1", (296, 128), "white")
        