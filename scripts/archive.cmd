@echo off
REM ------------------------------------------------------------------------------------
REM Script:       archive.cmd
REM Description:  Creates the Windows-specific binary archive (bin_win.zip) containing
REM               the newest installed binary from %USERPROFILE%\bin and all historical
REM               binaries from .\bin\. This script does NOT compile or install anything.
REM ------------------------------------------------------------------------------------
REM Author:       Patrik Eigenmann
REM email:        p.eigenmann72@gmail.com
REM GitHub:       https://github.com/PatrikEigenmann72/Scripts
REM ------------------------------------------------------------------------------------
REM Change Log:
REM Wed 2026-07-01 File created and content added.                        Version: 00.01
REM ------------------------------------------------------------------------------------

set PROJECT=pmake
set ZIP=bin_win.zip
set INSTALLED=%USERPROFILE%\bin\%PROJECT%.exe

if not exist "%INSTALLED%" (
    echo Installed binary not found: %INSTALLED%
    exit /b 1
)

set TMP=archive_tmp
if exist "%TMP%" rmdir /s /q "%TMP%"
mkdir "%TMP%"

copy "%INSTALLED%" "%TMP%" >nul
copy bin\* "%TMP%" >nul

if exist "%ZIP%" del "%ZIP%"

powershell -Command "Compress-Archive -Path '%TMP%\*' -DestinationPath '%ZIP%'"

rmdir /s /q "%TMP%"

echo Created %ZIP%