# Pocket Trustee
次世代All-in-one数字钥匙

## 免责声明
1. PocketTrustee（以下简称本项目）是由社区支持的开源项目，没有任何个人或集体可以为其安全性提供长期保障；   
2. 本项目仅供学习、交流使用，严禁将PocketTrustee用于任何需要安全保障的领域；   
3. 由于使用本项目造成的财产损失，社区开发者不承担任何责任；   
4. 本项目硬件部分以CC BY 4.0协议发放，软件部分以Apache-2.0协议发放，社区开发者不对本项目任何部分的可靠性负责，社区开发者不对衍生项目负责；   
5. 本项目所使用的开源库不在“本项目软件”的范围内，因此不受Apache-2.0协议约束，不随同本项目分发，关于这些库的开源协议，请查看库文件夹下的LICENSE文件；   


## 硬件设计
硬件设计发布在[OSHW Hub](https://oshwhub.com/)上  


## 编译，下载，初始化说明
1. 下载 [STM32 CubeCLT](https://www.st.com/en/development-tools/stm32cubeclt.html) 工具链  
确保你的工具链已经添加到PATH环境变量中
2. 将命令行工具转到项目目录下，逐句执行：  
`cmake --preset "Debug"`  
`cd build/Debug`  
`ninja`  
在build/Debug目录下，PocketTrustee.elf 即编译产物  
**不要编译Release版，代码优化会带来无法预测的bug**
3. 使用 [STM32 CubeProgrammer](https://www.st.com/en/development-tools/stm32cubeprog.html) 工具将PocketTrustee.elf 烧录到芯片上   
4. 连接设备和电脑，应当能在设备管理器里看到新的串口设备   
5. 使用python运行`tools/PocketTrusteeCLI/main.py`，并执行命令`connect -a --init`  
6. 根据提示操作即可；更多关于PocketTrusteeCLI工具的说明请见后续章节  


## PocketTrusteeCLI
专为PocketTrustee设计的CLI工具，可以用于添加/删除/编辑 密码，ID卡，TOTP，以及进行数据备份和还原等  
请见[PocketTrusteeCLI](tools/PocketTrusteeCLI/README.md)