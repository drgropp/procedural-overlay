@echo off
setlocal
set "TOOLS=C:\w64devkit\bin"
set "PATH=%TOOLS%;%PATH%"

"%TOOLS%\cmake.exe" -S . -B build -G "MinGW Makefiles" ^
  -DCMAKE_MAKE_PROGRAM="%TOOLS%\mingw32-make.exe" ^
  -DCMAKE_C_COMPILER="%TOOLS%\gcc.exe"
if errorlevel 1 exit /b 1

"%TOOLS%\cmake.exe" --build build
exit /b %errorlevel%
