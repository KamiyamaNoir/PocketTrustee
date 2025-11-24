# PocketTrustee CLI
使用python编写的CLI工具
## 基本命令表
### `connect [-h] [-a] [--init] [--port COM_PORT]`  
连接到设备，当不指定任何参数时，将显示可用串口  
`-a`或`--auto`将尝试自动连接  
`--init`将初始化设备
`--port COM_PORT`可以指定串口连接

### `disconnect`
断开与设备的连接，仅有这种方法能使设备退出连接

### `device_rename [-h] name`  
位置参数`name`指定期望的设备名称，不能超过25个字符，仅能包含ASCII字符

### `totp add, delete, list [-h] ...`  
管理TOTP  
- `totp add --name NAME --key KEY [--format hex, base64, base32]`  
增加一个TOTP  
参数`NAME`为显示的totp名称，不超过25个ascii字符  
参数`KEY`为totp密钥  
可选参数`format`指定密钥的格式，可选hex, base64, base32，默认为hex  
- `totp delete --name NAME`  
删除一个指定的totp  
- `totp list`  
列出所有的totp  

### `password add, modify, delete, list [-h] ...`   
管理密码
- `password add --name NAME --pwd PWD --account ACCOUNT`  
增加一个密码
- `password delete --name NAME`  
删除一个密码
- `password modify --name NAME [RENAME] [--pwd PWD] [--account ACCOUNT]`  
编辑一个已有的密码，若指定了可选参数`--pwd` `-account`，其后为修改值  
若要修改名称，在原名称后加修改后的名称  
示例：修改名称为raw的密码为new，并修改账号为abc123，密码不变  
`password --name "raw" "new" --account "acb123"`
- `password list`  
列出所有密码

### `idcard rename, delete, list [-h] ...`  
管理idcard
- `idcard rename NAME RENAME`  
修改一个已有id卡的名称
- `idcard delete NAME`  
删除一个已有id卡
- `idcard list`  
列出当前所有id卡

### `wifi add, delete, list [-h] ...`
管理WIFI卡片
- `wifi add SSID PWD`
新建一个WIFI卡片，指定SSID和PWD
- `wifi delete SSID`
删除一个WIFI卡片
- `wifi list`
列出所有的wifi卡片

### `namecard add, delete, list [-h] ...`
管理名片，用法基本和wifi卡片一样
- `namecard add NAME [--TEL TEL] [--EMAIL EMAIL]`
新建一个名片，名字为必填，可选参数TEL和EMAIL
- `namecard delete NAME`
- `namecard list`

### `backup`
同步数据（备份数据）  
密码，TOTP，ID卡数据会被同步到devices目录下对应设备名称的pkt文件内，**请注意pkt文件内原来的数据将会被覆盖**  
pkt文件内的密码数据将以明文保存，请注意**像保护ssh私钥一样保护你的pkt文件**  
随时备份你的pkt文件，**pkt文件也将用于还原数据**

### `recover [-h] PATH`
还原数据
- PATH : 要还原的数据源pkt文件路径  
目前仅还原密码，TOTP和ID卡

### `export PATH`
导出密码为txt文件，密码将以JSON形式打印保存为TXT文件
- PATH : 要导出的数据源(pkt文件)

## SuperDebug命令表
如果你不知道SuperDebug是干什么用的，说明你不需要用SuperDebug  
这部分我将写得很简单，相信各位需要su的dever们可以看懂
### `binary PATH`
将PATH指定的图片二值化，保存为GUI可使用的bin文件

### `su_state`
查看FS占用的block数

### `su_ls PATH`
作用等同于 `ls`

### `su_rm PATH`
等同于 `rm`

### `su_mkdir PATH`
等同于 `mkdir`

### `su_rename RAW_PATH NEW_PATH`
重命名文件，但在LittleFS内此命令允许移动文件

### `su_write LOCAL_PATH REMOTE_PATH`
将`LOCAL_PATH`写入`REMOTE_PATH`

### `su_read REMOTE_PATH LOCAL_PATH`
将`REMOTE_PATH`读取到`LOCAL_PATH`