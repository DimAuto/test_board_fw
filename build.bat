@echo off

rmdir /s /q "./build"
if errorlevel 1 goto rmerr
echo ./build directory removed

cmake -G "Unix Makefiles" -B build -DCMAKE_VERBOSE_MAKEFILE=TRUE
if errorlevel 1 goto cmakeerr
echo Cmakefile created

cmake --build build
if errorlevel 1 goto builderr
echo Build Finished!!!
pause
goto:eof



:rmerr
echo Fail to remove build directory
pause


:cmakeerr
echo Fail to create Cmakefile
pause
goto:eof

:builderr
echo Fail to build the targer
pause