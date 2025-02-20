@ECHO OFF
REM Create zipfile using PowerShell
SET content=.\code,.\tools,*.bat,*.command,*.md,*.lua,*.txt,*.sh
DEL /Q worktest.zip
powershell Compress-Archive -Path %content% -DestinationPath .\worktest.zip
