import cbor2
from Crypto.Cipher import AES


class DeviceFS:
    passwords = []
    totp = []
    idcards = []
    wifi = []
    namecards = []
    
    def __init__(self, key: bytes):
        self.key = key
        self.aes = AES.new(key, AES.MODE_ECB)
    
    @classmethod
    def load_from_file(cls, path):
        with open(path, 'rb') as f:
            raw_bytes = cbor2.loads(f.read())
            ins = cls(raw_bytes['key'])
            ins.key = raw_bytes['key']
            plaintext = ins.aes.decrypt(raw_bytes['data'])
            data = cbor2.loads(plaintext)
            ins.passwords = data['passwords']
            ins.totp = data['totp']
            ins.idcards = data['idcards']
            ins.wifi = data['wifi']
            ins.namecards = data['namecards']
        return ins
    
    def aes_pad(self, data: bytes):
        pad_length = (16 - len(data) % 16) % 16
        return data + b'\x00' * pad_length
    
    def save_as_file(self, path):
        plaintext = cbor2.dumps({
            "passwords": self.passwords,
            "totp": self.totp,
            "idcards": self.idcards,
            "wifi": self.wifi,
            "namecards": self.namecards
        })
        encrypto = self.aes.encrypt(self.aes_pad(plaintext))
        fs_bytes = cbor2.dumps({
            "key": self.key,
            "data": encrypto
        })
        with open(path, 'wb') as f:
            f.write(fs_bytes)
        
    def add_password(self, name:str, account: str, password: str):
        if len(account) > 25 or len(password) > 25 or len(name) > 25:
            raise SystemError("Too long account or password")
        self.passwords.append({
            "name": name,
            "account": account,
            "password": password,
        })
    
    def add_totp(self, name: str, key: bytes, step: int=30, num_len: int=6):
        if len(key) != 20:
            raise ValueError("Wrong key")
        if len(name) > 25:
            raise ValueError("Too long name")
        self.totp.append({
            "name": name,
            "key": key,
            "step": step,
            "num": num_len
        })
        
    def add_idcard(self, name: str, manchester: bytes):
        if len(manchester) != 8:
            raise ValueError("Wrong Mancheter code")
        if len(name) > 25:
            raise ValueError("Too long name")
        self.idcards.append({
            "name": name,
            "code": manchester
        })
        
    def add_wifi(self, image: bytes, name: str):
        if len(image) != 4736:
            raise ValueError("Wrong image")
        if len(name) > 25:
            raise ValueError("Too long name")
        self.wifi.append({
            "name": name,
            "img": image
        })
        
    def add_namecard(self, image: bytes, name: str):
        if len(image) != 4736:
            raise ValueError("Wrong image")
        if len(name) > 25:
            raise ValueError("Too long name")
        self.namecards.append({
            "name": name,
            "img": image
        })
        