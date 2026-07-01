# ------------------------------------------------------------------------------------
# Script:       install.ps1
# Description:  Installs the newest compiled binary from .\bin into ~/bin.
#               Supports both OS-gnostic and OS-neutral naming.
#               This script does NOT compile the project. Use compile.ps1 first.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Thu 2025-08-14 File created and content added.                        Version: 00.01
# Thu 2026-04-30 Updated to remove compilation logic.                   Version: 00.02
# Thu 2026-04-30 Added manpage-style help section.                      Version: 00.03
# Fri 2026-06-19 Bugfix: Windows OS uses ending .exe for binaries.      Version: 00.04
# Tue 2026-06-30 Bugfix: fallback logic — Unix executables don't have
#                 extensions; adjusted pattern accordingly.             Version: 00.05
# Tue 2026-06-30 Improved output messaging: now prints "Installed
#                 <source> to <target>" for clarity.                    Version: 00.06
# ------------------------------------------------------------------------------------

param(
    [string]$Flag
)

function Show-Help {
@"
NAME
    install.ps1 - install the newest compiled project binary into ~/bin

SYNOPSIS
    .\install.ps1 [OPTIONS]

DESCRIPTION
    This script takes the active directory as project name and
    installs the newest binary from .\bin\ into ~/bin.
    Supports both OS-gnostic and OS-neutral naming.
    It does NOT compile the project. Use compile.ps1 first.

OPTIONS
    -h, -help, -?   Show this help menu

EXAMPLES
    .\install.ps1
"@ | more
}

# Parse arguments
if ($Flag -in @("-h", "-help", "-?")) {
    Show-Help
    exit
}

# Extract project name from current directory
$projectName = Split-Path -Leaf (Get-Location)

Write-Host "Installing $projectName (win)..."

# ---------------------------------------------------------------------
# Find newest binary: first OS-gnostic, then fallback to OS-neutral
# ---------------------------------------------------------------------
$latest = $null
$latestMaj = 0
$latestMin = 0

# Search OS-gnostic first
$patternOS = ".\bin\${projectName}_win_v*.exe"
foreach ($f in Get-ChildItem $patternOS -ErrorAction SilentlyContinue) {

    if (-not $f) { break }

    if ($f.Name -match "_v([0-9]+)\.([0-9]+)") {
        $maj = [int]$Matches[1]
        $min = [int]$Matches[2]

        if (($maj -gt $latestMaj) -or (($maj -eq $latestMaj) -and ($min -gt $latestMin))) {
            $latest = $f.FullName
            $latestMaj = $maj
            $latestMin = $min
        }
    }
}

# Fallback: OS-neutral
if (-not $latest) {
    $patternNeutral = ".\bin\${projectName}_v*"
    foreach ($f in Get-ChildItem $patternNeutral -ErrorAction SilentlyContinue) {

        if (-not $f) { break }

        if ($f.Name -match "_v([0-9]+)\.([0-9]+)") {
            $maj = [int]$Matches[1]
            $min = [int]$Matches[2]

            if (($maj -gt $latestMaj) -or (($maj -eq $latestMaj) -and ($min -gt $latestMin))) {
                $latest = $f.FullName
                $latestMaj = $maj
                $latestMin = $min
            }
        }
    }
}

# ---------------------------------------------------------------------
# Ensure we found something
# ---------------------------------------------------------------------
if (-not $latest) {
    Write-Host "Error: No binaries found for project '$projectName'."
    exit 1
}

Write-Host "Found newest binary: $latest"

# Determine extension (Windows always .exe)
$ext = ".exe"

# Install to ~/bin
$homeBin = Join-Path $HOME "bin"
New-Item -ItemType Directory -Force -Path $homeBin | Out-Null

$target = Join-Path $homeBin ($projectName + $ext)

Copy-Item $latest $target -Force

Write-Host "Installed $latest to $target"
Write-Host "Done. Type '$projectName' to run it."