版本控制subversion服务器启动脚本

拷贝以下内容并命名svn_run.bat，放置版本库顶目录，双击即启动服务器

=================================================================start

@ echo off
set mypath = ""
if "%1" == ""     goto DEF
if "%1" == "HELP" goto HELP
if "%1" == "help"  goto HELP
 
echo 正打开指定版本仓库%1
svnserve --daemon --root %1
@goto END

:DEF

rem echo 正打开默认版本仓库F:\工作目录\svn_repository
rem svnserve --daemon --root F:\工作目录\svn_repository

rem chdir > mypath

echo 正打开当前目录版本仓库%CD% 
svnserve --daemon --root  %CD%
@goto END

:HELP
echo 用法：%0 
@goto END

:END
echo 关闭版本仓库

=================================================================end
