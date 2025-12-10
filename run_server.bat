@echo off
title WhatsDown Server
echo Starting WhatsDown Chat Server...
echo.

REM 1. Check for Windows Executable
if exist chatting_app.exe (
    echo [INFO] Found Windows executable. Launching...
    chatting_app.exe
    goto end
)

REM 2. Check for C++ Compiler to Compile Windows Executable
where g++ >nul 2>nul
if %errorlevel% equ 0 (
    echo [INFO] Windows executable not found, but 'g++' compiler found.
    echo [INFO] Compiling for Windows...
    g++ main.cpp ChatController.cpp UserManager.cpp MessageManager.cpp -o chatting_app.exe -lws2_32 -static
    if exist chatting_app.exe (
        echo [SUCCESS] Compilation successful! Launching...
        echo ----------------------------------------
        chatting_app.exe
        goto end
    )
)

REM 3. Fallback: Try Running via WSL (Linux Subsystem)
echo [INFO] Windows executable/compiler not found.
echo [INFO] Attempting to run via WSL (Linux)...
echo.

wsl ./run.sh

if %errorlevel% neq 0 (
    echo.
    echo [ERROR] Could not start the server in Windows or WSL.
    echo.
    echo Troubleshooting:
    echo 1. If you are the developer: Ensure WSL is working.
    echo 2. If you are a friend: You need 'chatting_app.exe' sent to you.
    echo.
)

:end
pause
