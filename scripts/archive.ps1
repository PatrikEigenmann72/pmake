# ------------------------------------------------------------------------------------
# Script:       archive.ps1
# Description:  Creates the Windows-specific binary archive (bin_win.zip) containing
#               the newest installed binary from ~/bin and all historical binaries
#               from ./bin/. This script does NOT compile or install anything.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Wed 2026-07-01 File created and content added.                        Version: 00.01
# ------------------------------------------------------------------------------------

$Project = "pmake"
$Zip = "bin_win.zip"
$Installed = "$HOME\bin\$Project.exe"

# Ensure installed binary exists
if (-not (Test-Path $Installed)) {
    Write-Host "Installed binary not found: $Installed"
    exit 1
}

# Create temp folder
$Tmp = "./archive_tmp"
Remove-Item $Tmp -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Path $Tmp | Out-Null

# Copy installed binary
Copy-Item $Installed -Destination $Tmp

# Copy historical binaries
Copy-Item "./bin/*" -Destination $Tmp -ErrorAction SilentlyContinue

# Overwrite old zip
if (Test-Path $Zip) {
    Remove-Item $Zip -Force
}

Compress-Archive -Path "$Tmp/*" -DestinationPath $Zip

# Cleanup
Remove-Item $Tmp -Recurse -Force

Write-Host "Created $Zip"