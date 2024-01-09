CLS
echo off
set arg1=%1
set arg2=%2
MSP430Flasher.exe -n:%arg1%  -w:%arg2% -v -z  [VCC] 
pause
