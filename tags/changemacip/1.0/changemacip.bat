@echo off 

echo 1.�޸ı���MAC��ַ 
echo 2.�ָ�����MAC��ַ 

@choice /c:12 cheg,back 
if errorlevel 2 goto back 
if errorlevel 1 goto cheg 

:cheg 

echo ���ڽ��ñ������� 
devcon disable *ven_10ec*

echo ���µ�MACֵд��ע��� 
start new.reg 
@pause 

@echo new ip
netsh -f newip.txt
@pause

@echo new computer name
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Control\ComputerName\ComputerName" /v "ComputerName" /d "������������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "NV Hostname" /d "������������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "Hostname" /d "������������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName" /v "ComputerName" /d "������������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "NV Hostname" /d "������������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "Hostname" /d "������������" /f


echo �������ñ������� 
echo ������ʾ���˲���ʱ��ϳ��������ĵȴ����ű�ִ����ɺ󣬱����ڻ��Զ��˳��� 
devcon enable *ven_10ec*

goto end 

:back
 
echo ���ڽ������� 
devcon disable *ven_10ec*

echo ���ϵ�MACֵд��ע��� 
start native.reg 
@pause

@echo native ip
netsh -f nativeip.txt
@pause

@echo native computer name
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Control\ComputerName\ComputerName" /v "ComputerName" /d "�Լ�������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "NV Hostname" /d "�Լ�������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Services\Tcpip\Parameters" /v "Hostname" /d "�Լ�������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName" /v "ComputerName" /d "�Լ�������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "NV Hostname" /d "�Լ�������" /f
reg add "HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\Tcpip\Parameters" /v "Hostname" /d "�Լ�������" /f

echo ������������ִ����ɺ󣬽ű����Զ��˳��� 

echo ������������������ʱ���Գ��������ĵȴ����� 
devcon enable *ven_10ec*

goto end 
