�汾����subversion�����������ű�

�����������ݲ�����svn_run.bat�����ð汾�ⶥĿ¼��˫��������������

=================================================================start

@ echo off
set mypath = ""
if "%1" == ""     goto DEF
if "%1" == "HELP" goto HELP
if "%1" == "help"  goto HELP
 
echo ����ָ���汾�ֿ�%1
svnserve --daemon --root %1
@goto END

:DEF

rem echo ����Ĭ�ϰ汾�ֿ�F:\����Ŀ¼\svn_repository
rem svnserve --daemon --root F:\����Ŀ¼\svn_repository

rem chdir > mypath

echo ���򿪵�ǰĿ¼�汾�ֿ�%CD% 
svnserve --daemon --root  %CD%
@goto END

:HELP
echo �÷���%0 
@goto END

:END
echo �رհ汾�ֿ�

=================================================================end
