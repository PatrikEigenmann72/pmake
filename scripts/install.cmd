@echo off
:: ------------------------------------------------------------------------------------
:: Script:       install.cmd
:: Description:  Installs the newest compiled binary from .\bin into %USERPROFILE%\bin.
::               Supports both OS-gnostic and OS-neutral naming.
::               This script does NOT compile the project. Use compile.cmd first.
:: ------------------------------------------------------------------------------------
:: Author:       Patrik Eigenmann
:: email:        p.eigenmann72@gmail.com
:: GitHub:       https://github.com/PatrikEigenmann72/Scripts
:: ------------------------------------------------------------------------------------
:: Change Log:
:: Thu 2026-04-30 File created and content added.                        Version: 00.01
:: Fri 2026-06-19 Bugfix: Windows OS uses ending .exe for binaries.      Version: 00.02
:: Tue 2026-06-30 Added OS-gnostic + fallback + newest-version logic.    Version: 00.03
:: Tue 2026-06-30 Bugfix: fallback logic — Unix executables don't have
::                 extensions; adjusted pattern accordingly.             Version: 00.04
:: Tue 2026-06-30 Improved output messaging: now prints "Installed
::                 <source> to <target>" for clarity.                    Version: 00.05
:: ------------------------------------------------------------------------------------

:: Extract project name from current directory
set "PROJECT=%CD%"
for %%I in ("%PROJECT%") do set "PROJECT=%%~nI"

echo Installing %PROJECT% (win)...

set "LATEST="
set "LATEST_MAJ=0"
set "LATEST_MIN=0"

:: ---------------------------------------------------------------------
:: Search OS-gnostic first: project_win_vXX.XX.exe
:: ---------------------------------------------------------------------
for %%F in (bin\%PROJECT%_win_v*.exe) do (
    if exist "%%F" (
        for %%A in ("%%~nF") do (
            set "NAME=%%~nA"
        )
        for /f "tokens=2 delims=_v." %%M in ("%NAME%") do (
            set "MAJ=%%M"
        )
        for /f "tokens=3 delims=_v." %%N in ("%NAME%") do (
            set "MIN=%%N"
        )

        if %MAJ% GTR %LATEST_MAJ% (
            set "LATEST=%%F"
            set "LATEST_MAJ=%MAJ%"
            set "LATEST_MIN=%MIN%"
        ) else if %MAJ% EQU %LATEST_MAJ% if %MIN% GTR %LATEST_MIN% (
            set "LATEST=%%F"
            set "LATEST_MIN=%MIN%"
        )
    )
)

:: ---------------------------------------------------------------------
:: Fallback: OS-neutral project_vXX.XX*
:: ---------------------------------------------------------------------
if "%LATEST%"=="" (
    for %%F in (bin\%PROJECT%_v*) do (
        if exist "%%F" (
            for %%A in ("%%~nF") do (
                set "NAME=%%~nA"
            )
            for /f "tokens=2 delims=_v." %%M in ("%NAME%") do (
                set "MAJ=%%M"
            )
            for /f "tokens=3 delims=_v." %%N in ("%NAME%") do (
                set "MIN=%%N"
            )

            if %MAJ% GTR %LATEST_MAJ% (
                set "LATEST=%%F"
                set "LATEST_MAJ=%MAJ%"
                set "LATEST_MIN=%MIN%"
            ) else if %MAJ% EQU %LATEST_MAJ% if %MIN% GTR %LATEST_MIN% (
                set "LATEST=%%F"
                set "LATEST_MIN=%MIN%"
            )
        )
    )
)

:: ---------------------------------------------------------------------
:: Ensure we found something
:: ---------------------------------------------------------------------
if "%LATEST%"=="" (
    echo Error: No binaries found for project "%PROJECT%".
    exit /b 1
)

echo Found newest binary: %LATEST%

:: Windows always uses .exe
set "TARGET=%USERPROFILE%\bin\%PROJECT%.exe"

if not exist "%USERPROFILE%\bin" mkdir "%USERPROFILE%\bin"
copy "%LATEST%" "%TARGET%" >nul

echo Installed %LATEST% to %TARGET%
echo Done. Type "%PROJECT%" to run it.