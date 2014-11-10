@echo off 

echo 1.修改本机MAC地址 
echo 2.恢复本机MAC地址 

@choice /c:12 cheg,back 
if errorlevel 2 goto back 
if errorlevel 1 goto cheg 

:cheg 

echo 正在禁用本机网卡 
devcon disable *ven_10ec*

echo 将新的MAC值写入注册表 
start new.reg 
@pause 

@echo new ip
netsh -f newip.txt
@pause

@echo new computer name
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Control\ComputerName\ComputerName" /v "ComputerName" /d "可上网机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "NV Hostname" /d "可上网机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "Hostname" /d "可上网机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName" /v "ComputerName" /d "可上网机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "NV Hostname" /d "可上网机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "Hostname" /d "可上网机器名" /f


echo 正在启用本机网卡 
echo 友情提示：此操作时间较长，请耐心等待，脚本执行完成后，本窗口会自动退出。 
devcon enable *ven_10ec*

goto end 

:back
 
echo 正在禁用网卡 
devcon disable *ven_10ec*

echo 将老的MAC值写入注册表 
start native.reg 
@pause

@echo native ip
netsh -f nativeip.txt
@pause

@echo native computer name
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Control\ComputerName\ComputerName" /v "ComputerName" /d "自己机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "NV Hostname" /d "自己机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "Hostname" /d "自己机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName" /v "ComputerName" /d "自己机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "NV Hostname" /d "自己机器名" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "Hostname" /d "自己机器名" /f

echo 将重启网卡，执行完成后，脚本会自动退出。 

echo 正在启用网卡，启用时间稍长，请耐心等待…… 
devcon enable *ven_10ec*

goto end 
