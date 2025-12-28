@echo off
setlocal enabledelayedexpansion
REM ═══════════════════════════════════════════════════════════════════════════════
REM                           SMART GROCERY CART
REM                         Easy One-Click Startup
REM ═══════════════════════════════════════════════════════════════════════════════
REM
REM This script handles everything automatically:
REM   - Uses pre-compiled DLL if available (no compiler needed!)
REM   - Auto-installs Python packages
REM   - Compiles C++ only if needed and compiler is available
REM ═══════════════════════════════════════════════════════════════════════════════

title Smart Grocery Cart
color 0A

echo.
echo ══════════════════════════════════════════════════════════════════════════
echo           🛒 SMART GROCERY CART - Easy Startup
echo ══════════════════════════════════════════════════════════════════════════
echo.

REM Navigate to src directory
cd /d "%~dp0src"

REM ═══════════════════════════════════════════════════════════════════════════════
REM STEP 1: Check for DLL (most important - skip compilation if exists)
REM ═══════════════════════════════════════════════════════════════════════════════

echo [1/3] Checking C++ DLL...

if exist "grocery_api.dll" (
    echo ✅ DLL found! No compilation needed.
    echo.
    goto :check_python
)

echo    DLL not found, attempting to compile...
echo.

REM ═══════════════════════════════════════════════════════════════════════════════
REM STEP 1b: Try to compile (only if DLL doesn't exist)
REM ═══════════════════════════════════════════════════════════════════════════════

set COMPILER_FOUND=0

REM Try clang++ first
where clang++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo    Found clang++, compiling...
    clang++ -shared -o grocery_api.dll grocery_api.cpp -DBUILD_DLL -static 2>nul
    if %ERRORLEVEL% EQU 0 (
        set COMPILER_FOUND=1
        echo ✅ Compiled successfully with clang++
        goto :check_python
    )
)

REM Try g++ (MinGW)
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo    Found g++, compiling...
    g++ -shared -o grocery_api.dll grocery_api.cpp -DBUILD_DLL -static 2>nul
    if %ERRORLEVEL% EQU 0 (
        set COMPILER_FOUND=1
        echo ✅ Compiled successfully with g++
        goto :check_python
    )
)

REM Try cl.exe (Visual Studio)
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo    Found MSVC cl.exe, compiling...
    cl /LD /Fe:grocery_api.dll grocery_api.cpp /DBUILD_DLL 2>nul
    if %ERRORLEVEL% EQU 0 (
        set COMPILER_FOUND=1
        echo ✅ Compiled successfully with MSVC
        goto :check_python
    )
)

REM No compiler found - show helpful message
echo.
echo ══════════════════════════════════════════════════════════════════════════
echo ⚠️  NO C++ COMPILER FOUND - But don't worry!
echo ══════════════════════════════════════════════════════════════════════════
echo.
echo The grocery_api.dll file is missing and no compiler is available.
echo.
echo 🔧 EASY FIX OPTIONS:
echo.
echo    Option 1: Get the pre-compiled DLL
echo    ─────────────────────────────────
echo    Ask your teammate for the grocery_api.dll file
echo    and place it in the "src" folder.
echo.
echo    Option 2: Install a C++ compiler (any ONE of these):
echo    ─────────────────────────────────────────────────────
echo    • LLVM/Clang: https://github.com/llvm/llvm-project/releases
echo      (Download LLVM-xx.x.x-win64.exe, install, add to PATH)
echo.
echo    • MinGW-w64: https://winlibs.com/
echo      (Download, extract, add bin folder to PATH)
echo.
echo    • Visual Studio Build Tools: https://visualstudio.microsoft.com/downloads/
echo      (Select "Desktop development with C++")
echo.
echo ══════════════════════════════════════════════════════════════════════════
echo.
pause
exit /b 1

:check_python
REM ═══════════════════════════════════════════════════════════════════════════════
REM STEP 2: Check Python
REM ═══════════════════════════════════════════════════════════════════════════════

echo [2/3] Checking Python...

REM Try python command
python --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set PYTHON_CMD=python
    goto :python_found
)

REM Try python3 command
python3 --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set PYTHON_CMD=python3
    goto :python_found
)

REM Try py launcher
py --version >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    set PYTHON_CMD=py
    goto :python_found
)

echo.
echo ══════════════════════════════════════════════════════════════════════════
echo ❌ PYTHON NOT FOUND
echo ══════════════════════════════════════════════════════════════════════════
echo.
echo Please install Python 3.x from: https://www.python.org/downloads/
echo.
echo ⚠️  IMPORTANT: During installation, check "Add Python to PATH"
echo.
echo ══════════════════════════════════════════════════════════════════════════
echo.
pause
exit /b 1

:python_found
echo ✅ Python found!
echo.

REM ═══════════════════════════════════════════════════════════════════════════════
REM STEP 3: Install Python packages (silently)
REM ═══════════════════════════════════════════════════════════════════════════════

echo [3/3] Installing Python packages...

%PYTHON_CMD% -m pip install --quiet --disable-pip-version-check flask flask-cors >nul 2>&1
echo ✅ Flask and flask-cors ready
echo.

REM ═══════════════════════════════════════════════════════════════════════════════
REM STEP 4: Start the server
REM ═══════════════════════════════════════════════════════════════════════════════

echo ══════════════════════════════════════════════════════════════════════════
echo                     🚀 STARTING SERVER...
echo ══════════════════════════════════════════════════════════════════════════
echo.
echo    📍 Open your browser to: http://localhost:5000
echo.
echo    Press Ctrl+C to stop the server
echo.
echo ══════════════════════════════════════════════════════════════════════════
echo.

%PYTHON_CMD% server.py

echo.
echo Server stopped. Press any key to exit...
pause >nul
