#!/bin/sh
# ------------------------------------------------------------------------------------
# Script:       archive.sh
# Description:  Creates the OS-specific binary archive (bin_{os}.zip) containing the
#               newest installed binary from ~/bin and all historical binaries from
#               ./bin/. This script does NOT compile or install anything.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Wed 2026-07-01 File created and content added.                        Version: 00.01
# ------------------------------------------------------------------------------------

PROJECT="pmake"

# Detect OS
OS="$(uname -s)"
case "$OS" in
    Darwin)
        ZIP="bin_mac.zip"
        INSTALLED="$HOME/bin/$PROJECT"
        ;;
    Linux)
        ZIP="bin_linux.zip"
        INSTALLED="$HOME/bin/$PROJECT"
        ;;
    *)
        echo "Unsupported OS: $OS"
        exit 1
        ;;
esac

# Ensure installed binary exists
if [ ! -f "$INSTALLED" ]; then
    echo "Installed binary not found: $INSTALLED"
    exit 1
fi

# Create temp folder
TMP="./archive_tmp"
rm -rf "$TMP"
mkdir "$TMP"

# Copy installed binary
cp "$INSTALLED" "$TMP/"

# Copy historical binaries (only this OS)
cp ./bin/* "$TMP/" 2>/dev/null

# Create zip (overwrite old)
rm -f "$ZIP"
zip -j "$ZIP" "$TMP"/*

# Cleanup
rm -rf "$TMP"

echo "Created $ZIP"