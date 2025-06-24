@echo off
setlocal enabledelayedexpansion

:: Create build directory if it doesn't exist
if not exist "build\" (
    mkdir build
)
cd build

:: Run CMake and build the project
cmake ..
cmake --build .

:: Check if the executable exists
if not exist "bin\Kufar-Telegram-Notifier.exe" (
    echo Error: Build failed - Kufar-Telegram-Notifier.exe not found
    exit /b 1
)

:: Process command-line arguments
set "PARAMS="
set "CONFIG_SET=0"
set "CACHE_SET=0"

for %%a in (%*) do (
    set "arg=%%a"
    
    :: Check for --config=
    if "!arg:~0,9!"=="--config=" (
        set "PARAMS=!PARAMS! !arg!"
        set "CONFIG_SET=1"
    )
    
    :: Check for --cache=
    if "!arg:~0,8!"=="--cache=" (
        set "PARAMS=!PARAMS! !arg!"
        set "CACHE_SET=1"
    )
)

:: Set default config if not provided
if %CONFIG_SET% equ 0 (
    set "PARAMS=!PARAMS! --config=..\kufar-configuration.json"
)

:: Set default cache if not provided
if %CACHE_SET% equ 0 (
    set "PARAMS=!PARAMS! --cache=..\cached-data.json"
)

:: Run the executable
echo Running: bin\Kufar-Telegram-Notifier.exe %PARAMS%
bin\Kufar-Telegram-Notifier.exe %PARAMS%