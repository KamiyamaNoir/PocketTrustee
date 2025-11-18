import base64
import os
import json
import cbor2
from cmd2 import (
    Cmd,
    with_argparser,
    Cmd2ArgumentParser,
)
from PIL import Image
from serial.tools import list_ports
from connection_serial import Connection
from rich_console import console
from device_fs import DeviceFS


class PocketTrusteeCLI(Cmd):
    prompt = '(No Device)>'
    device_name = None

    def __init__(self):
        super().__init__()
        self.intro = "PocketTrustee CLI Tool\nVersion 1.0"
        self.connection = None
    
    # ====== Connect ======
    connect_parser = Cmd2ArgumentParser(
        description="连接到PocketTrustee设备"
    )
    connect_parser.add_argument('-p', '--port', type=str, help='设备的串口号', nargs=1)
    connect_parser.add_argument('-a', '--auto', help='尝试自动连接', action='store_true')
    connect_parser.add_argument('--init', help='初始化设备', action='store_true')

    @with_argparser(connect_parser)
    def do_connect(self, args):
        ports = list_ports.comports()
        if not ports:
            console.print("可用串口为空，请检查连接")
            return
        if args.auto:
            for port in ports:
                try:
                    conn = Connection(port.device)
                    if conn.is_availiable():
                        self.connection = conn
                    else:
                        continue
                except:
                    continue
            if not self.connection:
                console.print("无可用设备")
                return
        elif args.port is None:
            [console.print(port.description) for port in ports] # pylint: disable=W0106
            return
        else:
            try:
                conn = Connection(args.port[0])
                if conn.is_availiable():
                    self.connection = conn
                else:
                    console.print("设备无响应")
                    return
            except Exception as e:
                console.print(f"打开串口失败：{e}")
                return
        if args.init:
            console.print("警告:这将重置设备并删除所有数据,是否继续(Y/N)", style='bold red')
            ans = input()
            if ans.upper() != 'Y':
                return
            reset_resp = self.connection.send_reset()
            if reset_resp != b'ok':
                console.print("设备已重启,请重新连接", style='blue')
                return
            console.print("输入你期望的设备名称,不超过25个字符,留空默认")
            device_name = 'PocketTrustee'
            while True:
                ans = input()
                if len(ans) > 25:
                    console.print("名称过长,请重新输入", style='bold red')
                    continue
                if len(ans) != 0:
                    device_name = ans
                break
            console.print("输入你的6位PIN码,[red]设定后不可修改")
            device_pin = ''
            while True:
                device_pin = input()
                if len(device_pin) != 6 or not device_pin.isnumeric():
                    console.print("PIN格式错误,请重新输入", style='bold red')
                    continue
                break
            key = self.connection.send_device_initialize(device_name, device_pin)
            if len(key) != 16:
                console.print("发生错误", style='bold red')
                return
            device_fs = DeviceFS(key)
            device_fs.save_as_file(f'devices/{device_name}.pkt')
            self.invoke_init()
            return
        key_map = {
            'debug_xx_device': b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
        }
        for dirpath, _, filenames in os.walk('./devices'):
            for file in filenames:
                if file[-4:] != '.pkt':
                    continue
                dfs = DeviceFS.load_from_file(os.path.join(dirpath, file))
                key_map[file[:-4]] = dfs.key
        # now ready to respond to challenge
        try:
            resp = self.connection.send_challenge_request(key_map)
            self.device_name = resp
            self.prompt = f'({resp})>'
            console.print(f"成功连接到:{self.connection.ser.port}", style='bold green')
            self.invoke_backup()
        except Exception as e:
            conn.ser.close()
            console.print(f"连接请求被拒绝,{e}", style='bold red')
            return
        
    def invoke_backup(self):
        dfs = DeviceFS.load_from_file(f'./devices/{self.connection.device_name}.pkt')
        # Load passwords
        dfs.passwords.clear()
        file_ls = self.connection.send_su_file_ls('passwords/').split('\n')
        for file_name in file_ls:
            if file_name[-4:] != '.pwd':
                continue
            fbytes = self.connection.send_su_file_read(f'passwords/{file_name}')
            fb_decrypto = self.connection.aes.decrypt(fbytes)
            flist = cbor2.loads(fb_decrypto)
            dfs.add_password(file_name[:-4], flist[0], flist[1])
            console.print(f'Backup: {file_name}')
        # Load totp
        dfs.totp.clear()
        file_ls = self.connection.send_su_file_ls('otp/').split('\n')
        for file_name in file_ls:
            if file_name[-5:] != '.totp':
                continue
            fbytes = self.connection.send_su_file_read(f'otp/{file_name}')
            fb_decrypto = self.connection.aes.decrypt(fbytes)
            flist = cbor2.loads(fb_decrypto)
            dfs.add_totp(file_name[:-5], flist[0], flist[1], flist[2])
            console.print(f'Backup: {file_name}')
        # Load ID Card
        dfs.idcards.clear()
        file_ls = self.connection.send_su_file_ls('idcards/').split('\n')
        for file_name in file_ls:
            if file_name[-5:] != '.card':
                continue
            fbytes = self.connection.send_su_file_read(f'idcards/{file_name}')
            # id卡并没有被加密,也没有format
            # fb_decrypto = self.connection.aes.decrypt(fbytes)
            # flist = cbor2.loads(fb_decrypto)
            dfs.add_idcard(file_name[:-5], fbytes)
            console.print(f'Backup: {file_name}')
        dfs.save_as_file(f'./devices/{self.connection.device_name}.pkt')
        console.print('备份完成', style='bold green')
            
    def invoke_recover(self, doc_path):
        dfs = DeviceFS.load_from_file(doc_path)
        # Recover passwords
        for file in dfs.passwords:
            console.print(f'Recover: {file['name']}')
            try:
                self.connection.send_pwd_add(file['name'], file['password'], file['account'])
            except Exception as e:
                console.print(e)
        # Recover TOTP
        for file in dfs.totp:
            console.print(f'Recover: {file['name']}')
            try:
                self.connection.send_totp_add(file['name'], file['key'], file['step'], file['num'])
            except Exception as e:
                console.print(f'Exception: {e}')
        # Recover ID Card
        for file in dfs.idcards:
            console.print(f'Recover: {file['name']}')
            try:
                file_ls = self.connection.send_su_file_ls('idcards/').split('\n')
                if f'{file['name']}.card' in file_ls:
                    continue
                self.connection.send_su_file_write(f'idcards/{file['name']}.card', file['code'])
            except Exception as e:
                console.print(e)
        console.print('还原完成', style='bold green')

    def invoke_init(self):
        finfo = open('./resource/resource.json', 'r', encoding='utf-8')
        finfo = json.loads(finfo.read())
        for tdir in finfo['directory']:
            console.print(f'mkdir {tdir}', style='bold blue')
            resp = self.connection.send_su_file_mkdir(tdir)
            console.print(resp)
        for tdir, local in finfo['package'].items():
            console.print(f'Transmit {tdir}', style='bold blue')
            with open(f'./resource/{local}', 'rb') as f:
                self.connection.send_su_file_write(tdir, f.read())
        self.connection.send_su_exit()
        console.print("初始化完成", style='bold green')
        
    # ====== Disconnect ======
    def do_disconnect(self, args):
        self.connection.send_disconnect()
        self.connection.ser.close()
        self.connection = None
        self.device_name = None
        self.prompt = '(No Device)>'
        
    # ====== Device Rename ======
    device_rename_parser = Cmd2ArgumentParser()
    device_rename_parser.add_argument('name', type=str, help='设备名称')
    
    @with_argparser(device_rename_parser)
    def do_device_rename(self, args):
        try:
            self.prompt = f'({self.connection.send_device_rename(args.name)})>'
        except Exception as e:
            self.poutput(e)
            
    # ====== Device Backup ======
    def do_backup(self, args):
        try:
            self.invoke_backup()
        except Exception as e:
            self.poutput(e)

    # ====== Device Recover ======
    device_recover_parser = Cmd2ArgumentParser()
    device_recover_parser.add_argument('path', type=str, help='还原文件路径(pkt文件)')

    @with_argparser(device_recover_parser)
    def do_recover(self, args):
        try:
            self.invoke_recover(args.path)
        except Exception as e:
            self.poutput(e)
            
    # ====== TOTP Function ======
    totp_parser = Cmd2ArgumentParser()
    totp_sub = totp_parser.add_subparsers(dest='subcmd', required=True, help='add / delete / list')
    
    totp_add = totp_sub.add_parser('add', help='新增一个totp')
    totp_add.add_argument('--name', type=str, help='TOTP名称', required=True, nargs=1)
    totp_add.add_argument('--key', type=str, help='TOTP密钥', required=True, nargs=1)
    totp_add.add_argument('--format', type=str, help='密钥的格式,可选base64, base32, hex', required=False, nargs=1, default='hex')
    
    totp_delete = totp_sub.add_parser('delete', help='删除totp')
    totp_delete.add_argument('--name', type=str, required=True, nargs=1)
    
    totp_list = totp_sub.add_parser('list', help='列出所有totp')
    
    @with_argparser(totp_parser)
    def do_totp(self, args):
        if args.subcmd == 'add':
            if args.format[0] == 'base64':
                key = base64.b64decode(args.key[0])
            elif args.format[0] == 'base32':
                key = base64.b32decode(args.key[0])
            elif args.format[0] == 'hex':
                key = bytes.fromhex(args.key[0])
            else:
                raise SystemError("非法格式")
            try:
                self.connection.send_totp_add(args.name[0], key, 30, 6)
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'delete':
            try:
                self.connection.send_totp_delete(args.name[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'list':
            try:
                self.connection.send_totp_list()
            except Exception as e:
                self.poutput(e)
                
    # ====== Password Function ======
    password_parser = Cmd2ArgumentParser()
    password_sub = password_parser.add_subparsers(dest='subcmd', required=True, help='add / rename / delete / list')
    
    password_add = password_sub.add_parser('add', help='新增一个密码')
    password_add.add_argument('--name', type=str, help='密码名称', required=True, nargs=1)
    password_add.add_argument('--account', type=str, help='账号', required=False, nargs=1, default='Unknown')
    password_add.add_argument('--pwd', type=str, help='密码', required=False, nargs=1, default='Unknown')
    
    password_rename = password_sub.add_parser('modify', help='修改')
    password_rename.add_argument('--name', type=str, help='密码名称 若修改后接新名称', required=True, nargs='+')
    password_rename.add_argument('--account', type=str, help='账号', required=False, nargs=1, default=None)
    password_rename.add_argument('--pwd', type=str, help='密码', required=False, nargs=1, default=None)
    
    password_delete = password_sub.add_parser('delete', help='删除密码')
    password_delete.add_argument('--name', type=str, required=True, nargs=1)
    
    password_list = password_sub.add_parser('list', help='列出所有密码')
    
    @with_argparser(password_parser)
    def do_password(self, args):
        if args.subcmd == 'add':
            try:
                self.connection.send_pwd_add(args.name[0], args.pwd[0], args.account[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'modify':
            try:
                rename = args.name[1] if len(args.name) == 2 else None
                repwd = args.pwd[0] if args.pwd is not None else None
                reaccount = args.accountp[0] if args.account is not None else None
                self.connection.send_pwd_modify(args.name[0], repwd, reaccount, rename)
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'delete':
            try:
                self.connection.send_pwd_delete(args.name[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'list':
            try:
                self.connection.send_pwd_list()
            except Exception as e:
                self.poutput(e)
                
    # ====== ID Card Function ======
    idcard_parser = Cmd2ArgumentParser()
    idcard_sub = idcard_parser.add_subparsers(dest='subcmd', required=True, help='rename / delete / list')
    
    idcard_rename = idcard_sub.add_parser('rename', help='重命名')
    idcard_rename.add_argument('name', type=str, help='idcard名称 后接新名称', nargs=2)
    
    idcard_delete = idcard_sub.add_parser('delete', help='删除card')
    idcard_delete.add_argument('name', type=str, nargs=1)
    
    idcard_list = idcard_sub.add_parser('list', help='列出所有card')
    
    @with_argparser(idcard_parser)
    def do_idcard(self, args):
        if args.subcmd == 'rename':
            try:
                self.connection.send_idcard_rename(args.name[0], args.name[1])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'delete':
            try:
                self.connection.send_idcard_delete(args.name[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'list':
            try:
                self.connection.send_idcard_list()
            except Exception as e:
                self.poutput(e)
                
    # ====== WIFI Card Function ======
    wificard_parser = Cmd2ArgumentParser()
    wificard_sub = wificard_parser.add_subparsers(dest='subcmd', required=True, help='rename / delete / list')
    
    wificard_add = wificard_sub.add_parser('add', help='新建')
    wificard_add.add_argument('ssid', type=str, help='WIFI SSID', nargs=1)
    wificard_add.add_argument('pwd', type=str, help='WIFI Password', nargs=1)
    
    wificard_delete = wificard_sub.add_parser('delete', help='删除card')
    wificard_delete.add_argument('ssid', type=str, nargs=1)
    
    wificard_list = wificard_sub.add_parser('list', help='列出所有card')
    
    @with_argparser(wificard_parser)
    def do_wifi(self, args):
        if args.subcmd == 'add':
            try:
                self.connection.send_wifi_add(args.ssid[0], args.pwd[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'delete':
            try:
                self.connection.send_wifi_delete(args.ssid[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'list':
            try:
                self.connection.send_wifi_list()
            except Exception as e:
                self.poutput(e)
                
    # ====== Namecard Function ======
    namecard_parser = Cmd2ArgumentParser()
    namecard_sub = namecard_parser.add_subparsers(dest='subcmd', required=True, help='rename / delete / list')
    
    namecard_add = namecard_sub.add_parser('add', help='新建')
    namecard_add.add_argument('name', type=str, help='Your name', nargs=1)
    namecard_add.add_argument('--TEL', type=str, help='Your phone number', nargs=1, required=False)
    namecard_add.add_argument('--EMAIL', type=str, help='Your email address', nargs=1, required=False)
    
    namecard_delete = namecard_sub.add_parser('delete', help='删除card')
    namecard_delete.add_argument('name', type=str, nargs=1)
    
    namecard_list = namecard_sub.add_parser('list', help='列出所有card')
    
    @with_argparser(namecard_parser)
    def do_namecard(self, args):
        if args.subcmd == 'add':
            try:
                kargs = {}
                if args.TEL is not None:
                    kargs['TEL'] = args.TEL[0]
                if args.EMAIL is not None:
                    kargs['EMAIL'] = args.EMAIL[0]
                self.connection.send_namecard_add(args.name[0], **kargs)
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'delete':
            try:
                self.connection.send_namecard_delete(args.name[0])
            except Exception as e:
                self.poutput(e)
        elif args.subcmd == 'list':
            try:
                self.connection.send_namecard_list()
            except Exception as e:
                self.poutput(e)
        
    # ====== Binary ======
    bianry_parser = Cmd2ArgumentParser()
    bianry_parser.add_argument('path', type=str, help='Path')
    bianry_parser.add_argument('-t', '--threshold', type=int, help='阈值', default=127)
    
    @with_argparser(bianry_parser)
    def do_binary(self, args):
        with Image.open(args.path).convert('L') as img:
            width, height = img.size
            binary = bytearray(int(width*height/8))
            for i in range(width):
                for j in range(height):
                    if img.getpixel((i, j)) > args.threshold:
                        binary[int(j/8 + i*height/8)] |= (0x80 >> (j % 8))
                    else:
                        binary[int(j/8 + i*height/8)] &= ~(0x80 >> (j % 8))
            with open(f'{args.path}.bin', 'wb') as f:
                f.write(binary)
        
    # ====== Super Debug ls ======
    su_ls_parser = Cmd2ArgumentParser()
    su_ls_parser.add_argument('path', type=str, help='Path')
    
    @with_argparser(su_ls_parser)
    def do_su_ls(self, args):
        try:
            self.poutput(self.connection.send_su_file_ls(args.path))
        except Exception as e:
            self.poutput(e)
            
    # ====== Super Debug state ======
    def do_su_state(self, args):
        try:
            self.poutput(self.connection.send_su_fs_state())
        except Exception as e:
            self.poutput(e)
            
    # ====== Super Debug rm ======
    su_rm_parser = Cmd2ArgumentParser()
    su_rm_parser.add_argument('path', type=str, help='Path')
    
    @with_argparser(su_rm_parser)
    def do_su_rm(self, args):
        try:
            self.poutput(self.connection.send_su_file_rm(args.path))
        except Exception as e:
            self.poutput(e)
            
    # ====== Super Debug mkdir ======
    su_mkdir_parser = Cmd2ArgumentParser()
    su_mkdir_parser.add_argument('path', type=str, help='Path')
    
    @with_argparser(su_mkdir_parser)
    def do_su_mkdir(self, args):
        try:
            self.poutput(self.connection.send_su_file_mkdir(args.path))
        except Exception as e:
            self.poutput(e)
            
    # ====== Super Debug rename ======
    su_rename_parser = Cmd2ArgumentParser()
    su_rename_parser.add_argument('path_old', type=str, help='Original path')
    su_rename_parser.add_argument('path_new', type=str, help='New path')
    
    @with_argparser(su_rename_parser)
    def do_su_rename(self, args):
        try:
            self.poutput(self.connection.send_su_file_rename(args.path_old, args.path_new))
        except Exception as e:
            self.poutput(e)
            
    # ====== Super Debug write ======
    su_write_parser = Cmd2ArgumentParser()
    su_write_parser.add_argument('path', type=str, help='Local path')
    su_write_parser.add_argument('dest', type=str, help='Remote path')
    
    @with_argparser(su_write_parser)
    def do_su_write(self, args):
        with open(args.path, 'rb') as f:
            try:
                self.connection.send_su_file_write(args.dest, f.read())
            except Exception as e:
                self.poutput(e)
                
    # ====== Super Debug read ======
    su_read_parser = Cmd2ArgumentParser()
    su_read_parser.add_argument('path', type=str, help='Remote path')
    su_read_parser.add_argument('dest', type=str, help='Local path')
    
    @with_argparser(su_read_parser)
    def do_su_read(self, args):
        with open(args.dest, 'wb') as f:
            try:
                fsread = self.connection.send_su_file_read(args.path)
                f.write(fsread)
            except Exception as e:
                self.poutput(e)
        
        
if __name__ == '__main__':
    app = PocketTrusteeCLI()
    app.cmdloop()
