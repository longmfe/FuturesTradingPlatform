本测试程序包含Windows和Linux两个工程，Windows版本用的是vs2017，如果安装的是其它VS版本，在打开时，右键项目属性，修改平台工具集和sdk版本。
程序运行后可执行“help”命令根据提示进行操作例如：

交易认证：auth（认证）
登录：login（登录）
报单：order cffex /c T2003 fak buy open /v 1 /p 95.795 /q  
      order dce /c a2005 fok buy open /v 1 /p 3715 /q 
      order dce /c a2005 buy open /v 1 /p 3715 /q （根据情况改合约代码，价格等参数）
撤单：cancel /n       408425 /c T1909 cffex（其中"       408425"为报单编号OrderSysID）
行情订阅：sub T2003
更多命令详见"help"

测试程序仅包含登录、下单、撤单、查询等基本功能。
附《6.3.15_API接口说明.chm》