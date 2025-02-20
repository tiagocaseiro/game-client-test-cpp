@ECHO OFF

REM Action can be any of vs2005 - vs2019 for the different Visual Studio versions
SET action=%1
IF "%action%"=="" (SET action=vs2019)

tools\Premake\win\premake5.exe %action%