from time import sleep
from time import time
import socket
# import binascii
import hashlib
import serial
import cbor2
from Crypto.Cipher import AES
from rich.progress import Progress
from rich_console import console
from img_generator import WIFICard, NameCard


class Connection:
    aes = None
    device_name = ''
    
    def __init__(self, com_port):
        self.ser = serial.Serial(port=com_port, baudrate=115200)
        
    def aes_pad(self, data: bytes):
        pad_length = (16 - len(data) % 16) % 16
        return data + b'\x00' * pad_length

    def clear_buffer(self):
        self.ser.reset_input_buffer()
        self.ser.reset_output_buffer()

    def wait_data(self, timeout=1000):
        data = bytearray()
        while timeout != 0:
            resp = self.ser.read_all()
            if resp:
                data.extend(resp)
                continue
            if len(data) > 0 and not resp:
                return data
            timeout -= 1
            sleep(0.001)
        return None

    def send_data(self, data: bytes):
        self.ser.write(data)

    def is_availiable(self) -> None | str:
        self.clear_buffer()
        # send hello
        self.send_data(b'\xA1\x63\x72\x65\x71\x65\x68\x65\x6C\x6C\x6F')
        nread = self.wait_data()
        if nread is not None and len(nread) > 8 and nread[:8] == b'trustee:':
            nread = nread.decode(encoding='ascii')
            return nread[8:]
        return None
    
    def send_reset(self):
        self.clear_buffer()
        # send hello
        self.send_data(cbor2.dumps({
            "req": "reset"
        }))
        nread = self.wait_data()
        if nread is None:
            raise SystemError("设备未响应")
        return nread
    
    def send_disconnect(self):
        self.clear_buffer()
        self.send_data(cbor2.dumps({
            "req": "disconnect"
        }))

    def send_challenge_request(self, key_map: dict) -> str:
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req" : "connect",
            "user" : f"{socket.gethostname()[:24]}"
        }))
        # polling for challenge
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应")
        resp = cbor2.loads(nread)
        # decrypt
        if resp['device'] not in key_map:
            raise SystemError("No available key")
        aes = AES.new(key_map[resp['device']], AES.MODE_ECB)
        plain_text = aes.decrypt(resp['challenge'])
        # send
        self.send_data(cbor2.dumps(plain_text))
        # result
        while True:
            nread = self.wait_data(timeout=2000)
            if nread is None:
                raise TimeoutError("验证超时")
            if nread[:5] == b'error':
                raise SystemError(nread.decode(encoding='ascii'))
            resp = cbor2.loads(nread)
            if resp['success']:
                self.aes = aes
                # send timestamp
                self.send_data(cbor2.dumps({
                    "timestamp": int(time())
                }))
                self.device_name = resp['device']
                return resp['device']
            raise TimeoutError("Unknown")
        
    def send_device_initialize(self, device_name: str, device_pin: str):
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "initialize",
            "user": f"{socket.gethostname()[:24]}",
            "device": device_name,
            "pin": device_pin,
            "timestamp": int(time())
        }))
        # polling for data
        nread = self.wait_data(timeout=2000)
        if nread is None:
            raise TimeoutError("设备未响应")
        return nread
    
    def send_su_exit(self):
        self.clear_buffer()
        self.send_data(cbor2.dumps({
            "req": "su_exit",
            "user": f"{socket.gethostname()[:24]}"
        }))
        
    def send_device_rename(self, name: str):
        self.clear_buffer()
        # check name
        if len(name) > 24:
            raise Exception("Too long name")
        # send request
        self.send_data(cbor2.dumps({
            "req": "device-rename",
            "user": f"{socket.gethostname()[:24]}",
            "name": name
        }))
        # polling for data
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应")
        return nread.decode(encoding='ascii')
    
    def send_totp_add(self, name: str, key: bytes, step: int, code_len: int):
        self.clear_buffer()
        # check name
        if len(name) > 24:
            raise Exception("Too long name")
        # check if totp is already exist
        totp_ls = self.send_su_file_ls('otp/')
        totp_ls = totp_ls.split('\n')
        if f'{name}.totp' in totp_ls:
            raise SystemError("该TOTP已被创建")
        # check key
        if len(key) < 2 or len(key) > 20:
            raise Exception("Unrecognized key")
        fbytes = cbor2.dumps([
            key,
            step,
            code_len
        ])
        enbytes = self.aes.encrypt(self.aes_pad(fbytes))
        # send file
        self.send_su_file_write(f'otp/{name}.totp', enbytes)
    
    def send_totp_delete(self, name: str):
        self.clear_buffer()
        # check name
        if len(name) > 24:
            raise Exception("Too long name")
        # coupling path
        path = f'otp/{name}.totp'
        # send request
        console.print(self.send_su_file_rm(path))
    
    def send_totp_list(self):
        self.clear_buffer()
        # send request
        dir_ls = self.send_su_file_ls('otp/')
        dir_ls = dir_ls.split('\n')
        dir_ls.remove('./')
        dir_ls.remove('../')
        for item in dir_ls:
            if len(item) == 0:
                continue
            if item[-5:] != '.totp':
                continue
            console.print(item[:-5])
            
    def send_pwd_add(self, name: str, pwd: str, account: str):
        # check length
        if len(pwd) > 25 or len(account) > 25:
            raise ValueError("密码或账户太长")
        # check if pwd is already exist
        pwd_ls = self.send_su_file_ls('passwords/')
        pwd_ls = pwd_ls.split('\n')
        if f'{name}.pwd' in pwd_ls:
            raise SystemError("该密码已被创建,请使用modify或delete")
        # fill in cbor sheet
        fbytes = cbor2.dumps([
            account,
            pwd
        ])
        # write
        enbyte = self.aes_pad(fbytes)
        enbyte = self.aes.encrypt(enbyte)
        self.send_su_file_write(f'passwords/{name}.pwd', enbyte)
        
    def send_pwd_modify(self, name: str, pwd: str | None, account: str | None, rename: str | None):
        Rpath = f'passwords/{name}.pwd'
        # check existence
        pwd_ls = self.send_su_file_ls('passwords/')
        pwd_ls = pwd_ls.split('\n')
        if rename is not None:
            if f'{rename}.pwd' in pwd_ls:
                raise ValueError("重命名目标已存在")
        # get raw file and decode file
        RFbytes = self.send_su_file_read(Rpath)
        RFpwd = cbor2.loads(self.aes.decrypt(RFbytes))
        # modify
        if account is not None:
            if len(account) > 25:
                raise ValueError("Too long account")
            RFpwd[0] = account
        if pwd is not None:
            if len(pwd) > 25:
                raise ValueError("Too long password")
            RFpwd[1] = pwd
        # remove old pwd
        self.send_su_file_rm(Rpath)
        # send new one
        Enbytes = self.aes_pad(cbor2.dumps(RFpwd))
        Enbytes = self.aes.encrypt(Enbytes)
        if rename is not None:
            Tpath = f'passwords/{rename}.pwd'
            self.send_su_file_write(Tpath, Enbytes)
        else:
            self.send_su_file_write(Rpath, Enbytes)

    def send_pwd_delete(self, name: str):
        path = f'passwords/{name}.pwd'
        console.print(self.send_su_file_rm(path))
        
    def send_pwd_list(self):
        pwd_dir = self.send_su_file_ls('passwords/')
        pwd_dir = pwd_dir.split('\n')
        pwd_dir.remove('./')
        pwd_dir.remove('../')
        for item in pwd_dir:
            if len(item) == 0:
                continue
            if item[-4:] != '.pwd':
                continue
            console.print(item[:-4])
            
    def send_idcard_rename(self, name, rename):
        Rpath = f'idcards/{name}.card'
        Tpath = f'idcards/{rename}.card'
        # check existence
        card_ls = self.send_su_file_ls('idcards/')
        card_ls = card_ls.split('\n')
        if rename is not None:
            if f'{rename}.card' in card_ls:
                raise ValueError("重命名目标已存在")
        # rename
        console.print(self.send_su_file_rename(Rpath, Tpath))
        
    def send_idcard_delete(self, name):
        Rpath = f'idcards/{name}.card'
        console.print(self.send_su_file_rm(Rpath))
            
    def send_idcard_list(self):
        pwd_dir = self.send_su_file_ls('idcards/')
        pwd_dir = pwd_dir.split('\n')
        pwd_dir.remove('./')
        pwd_dir.remove('../')
        for item in pwd_dir:
            if len(item) == 0:
                continue
            if item[-5:] != '.card':
                continue
            console.print(item[:-5])
            
    def send_wifi_add(self, ssid: str, pwd: str):
        Tpath = f'wifi/{ssid}.wifi'
        wifi_dir = self.send_su_file_ls('wifi/')
        wifi_dir = wifi_dir.split('\n')
        wifi_dir.remove('./')
        wifi_dir.remove('../')
        if f'{ssid}.wifi' in wifi_dir:
            raise ValueError("该WIFI已经存在,请先删除")
        wifi_card = WIFICard()
        wifi_card.make(ssid, pwd)
        data = wifi_card.binary()
        self.send_su_file_write(Tpath, data)
    
    def send_wifi_delete(self, ssid: str):
        Tpath = f'wifi/{ssid}.wifi'
        self.send_su_file_rm(Tpath)
        
    def send_wifi_list(self):
        wifi_dir = self.send_su_file_ls('wifi/')
        wifi_dir = wifi_dir.split('\n')
        wifi_dir.remove('./')
        wifi_dir.remove('../')
        for item in wifi_dir:
            if len(item) == 0:
                continue
            if item[-5:] != '.wifi':
                continue
            console.print(item[:-5])
            
    def send_namecard_add(self, name: str, **kargs):
        Tpath = f'namecard/{name}.ncard'
        card_dir = self.send_su_file_ls('namecard/')
        card_dir = card_dir.split('\n')
        card_dir.remove('./')
        card_dir.remove('../')
        if f'{name}.ncard' in card_dir:
            raise ValueError("该名片已经存在,请先删除")
        name_card = NameCard()
        name_card.make(name, **kargs)
        data = name_card.binary()
        self.send_su_file_write(Tpath, data)
    
    def send_namecard_delete(self, name: str):
        Tpath = f'namecard/{name}.ncard'
        self.send_su_file_rm(Tpath)
        
    def send_namecard_list(self):
        card_dir = self.send_su_file_ls('namecard/')
        card_dir = card_dir.split('\n')
        card_dir.remove('./')
        card_dir.remove('../')
        for item in card_dir:
            if len(item) == 0:
                continue
            if item[-6:] != '.ncard':
                continue
            console.print(item[:-6])

    def send_su_file_ls(self, path: str):
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_ls",
            "user": f"{socket.gethostname()[:24]}",
            "path": path
        }))
        # polling for data
        ls_str = ""
        while True:
            nread = self.wait_data()
            if nread is None or nread == b'endl':
                break
            try:
                rtvl = cbor2.loads(nread)
                ls_str += rtvl[0] + '\n'
                self.send_data(b'ok')
            except:
                continue
        return ls_str
    
    def send_su_fs_state(self):
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_state",
            "user": f"{socket.gethostname()[:24]}"
        }))
        # polling for data
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应")
        return nread.decode(encoding='ascii')

    def send_su_file_rm(self, path: str):
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_rm",
            "user": f"{socket.gethostname()[:24]}",
            "path": path
        }))
        # polling for data
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应")
        return nread.decode(encoding='ascii')
    
    def send_su_file_mkdir(self, path: str):
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_mkdir",
            "user": f"{socket.gethostname()[:24]}",
            "path": path
        }))
        # polling for data
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应")
        return nread.decode(encoding='ascii')

    def send_su_file_rename(self, old: str, new: str):
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_rename",
            "user":  f"{socket.gethostname()[:24]}",
            "old": old,
            "new": new
        }))
        # polling for data
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应")
        return nread.decode(encoding='ascii')

    def send_su_file_write(self, path: str, fbytes: bytes):
        p_size = 128
        total_size = len(fbytes)
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_write",
            "user": f"{socket.gethostname()[:24]}",
            "path": path
        }))
        # polling for response
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应 - Request")
        if nread != b'ok':
            raise SystemError(f"设备错误 - Request {nread.hex()}")
        package = [fbytes[i:i+p_size] for i in range(0, len(fbytes), p_size)]
        # send data
        with Progress() as progress:
            task = progress.add_task('Transmit', total=total_size)
            for index, pak in enumerate(package):
                more = False if index == len(package) - 1 else True
                self.send_data(cbor2.dumps({
                    "more": more,
                    "data": pak
                }))
                # polling for response
                nread = self.wait_data(timeout=5000)
                if nread is None:
                    raise TimeoutError("设备未响应 - Transmit")
                # rich progress
                progress.update(task, advance=len(pak))
                # check ok
                if nread[:2] != b'ok':
                    raise SystemError(f"设备错误 - Transmit{nread.hex()}")
                if not more:
                    # 有时候会把ok和SHA一起收到...
                    if len(nread) != 2:
                        nread = nread[2:]
                    else:
                        nread = self.wait_data(timeout=2000)
        # crc_check
        expected = hashlib.sha1(fbytes).digest()
        if nread is None:
            raise TimeoutError("设备未响应 - SHA1 Check")
        if nread != expected:
            raise SystemError("SHA1校验错误")

    def send_su_file_read(self, path: str) -> bytes:
        freads = bytearray()
        transmit = 0
        self.clear_buffer()
        # send request
        self.send_data(cbor2.dumps({
            "req": "su_read",
            "user": f"{socket.gethostname()[:24]}",
            "path": path
        }))
        # polling for response
        nread = self.wait_data()
        if nread is None:
            raise TimeoutError("设备未响应 - Request")
        if nread != b'ok':
            raise SystemError(f"设备错误,{nread.hex()}")
        self.send_data(b'ok')
        # read data
        while True:
            # polling for response
            nread = self.wait_data()
            if nread is None:
                raise TimeoutError("设备未响应 - Receive")
            resp = cbor2.loads(nread)
            if resp['aval']:
                freads.extend(resp['data'])
                # print
                transmit += len(resp['data'])
                print(f"\rReceive {transmit}")
            self.send_data(b'ok')
            if not resp['aval']:
                break
        # crc_check
        calculate = hashlib.sha1(freads).digest()
        nread = self.wait_data(timeout=2000)
        if nread is None:
            raise TimeoutError("设备未响应 - SHA1 Check")
        if nread != calculate:
            raise SystemError("SHA1校验错误")
        return bytes(freads)
