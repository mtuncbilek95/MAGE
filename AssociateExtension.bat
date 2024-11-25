@echo off
:: Get the directory of the script
set SCRIPT_DIR=%~dp0

:: Define the application path relative to the script directory
set APP_PATH="%SCRIPT_DIR%Build-Debug\Example\03-ResourceManagement\03-ResourceManagement.exe"

:: Set the file extension
set EXTENSION=.mproject

:: Associate the extension with the application
reg add "HKEY_CLASSES_ROOT\%EXTENSION%\shell\open\command" /ve /d "%APP_PATH% \"%%1\"" /f

:: Optional: Add icon for .mproject files
:: reg add "HKEY_CLASSES_ROOT\%EXTENSION%\DefaultIcon" /ve /d "%APP_PATH%,0" /f

echo File association for %EXTENSION% has been set with path: %APP_PATH%.
pause