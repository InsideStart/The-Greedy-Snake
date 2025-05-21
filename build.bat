@echo off
set BUILD_DIR=build
set CONFIG = build/../bin/debug
set CMAKE_OPTIONS=-G "Visual Studio 17 2022" -A x64
set MAKE_OPTIONS=config Debug

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

cmake .. 
cmake --build . 

:: 运行程序
echo 正在启动程序...
if exist .\%CONFIG%\snake.exe (
    .\%CONFIG%\snake.exe
) else (
    echo 错误：可执行文件未找到
)
cd ..
echo