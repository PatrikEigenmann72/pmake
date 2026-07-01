#!/bin/bash
# ------------------------------------------------------------------------------------
# Script:       install.sh
# Description:  Installs the newest compiled binary from ./bin into ~/bin.
#               Supports both OS-gnostic and OS-neutral naming.
#               This script does NOT compile the project. Use compile.sh first.
# ------------------------------------------------------------------------------------
# Author:       Patrik Eigenmann
# email:        p.eigenmann72@gmail.com
# GitHub:       https://github.com/PatrikEigenmann72/Scripts
# ------------------------------------------------------------------------------------
# Change Log:
# Thu 2025-08-14 File created and content added.                        Version: 00.01
# Thu 2026-04-30 Updated to remove compilation logic.                   Version: 00.02
# Tue 2026-06-30 Added OS-gnostic + fallback + newest-version logic.    Version: 00.03
# Tue 2026-06-30 Bugfix: fallback logic — Unix executables don't have
#                 extensions; adjusted pattern accordingly.             Version: 00.04
# Tue 2026-06-30 Improved output messaging: now prints "Installed
#                 <source> to <target>" for clarity.                    Version: 00.05
# ------------------------------------------------------------------------------------

show_help() {
cat << EOF | less
NAME
    install.sh - install the newest compiled project binary into ~/bin

SYNOPSIS
    install.sh [OPTIONS]

DESCRIPTION
    This script takes the active directory as project name and
    installs the newest binary from ./bin/ into ~/bin/.
    Supports both OS-gnostic and OS-neutral naming.

OPTIONS
    -h, -help, -?   Show this help menu

EXAMPLES
    install.sh
EOF
}

# Parse arguments
for arg in "$@"; do
    case $arg in
        -h|-help|-\?)
            show_help
            exit 0
            ;;
    esac
done

set -e

# Extract project name from current directory
PROJECT="$(basename "$PWD")"

# Detect OS tag
UNAME="$(uname -s)"
case "$UNAME" in
    Darwin) OS="mac" ;;
    Linux)  OS="linux" ;;
    *)      OS="win" ;;   # Windows via MSYS/Cygwin
esac

echo "Installing $PROJECT ($OS)..."

# ---------------------------------------------------------------------
# Find newest binary: first OS-gnostic, then fallback to OS-neutral
# ---------------------------------------------------------------------
LATEST=""
LATEST_MAJ=0
LATEST_MIN=0

# Search OS-gnostic first
for f in ./bin/${PROJECT}_${OS}_v*.*
do
    [ -e "$f" ] || break

    ver=$(echo "$f" | sed -E 's/.*_v([0-9]+)\.([0-9]+).*/\1 \2/')
    maj=$(echo "$ver" | awk '{print $1}')
    min=$(echo "$ver" | awk '{print $2}')

    if [ "$maj" -gt "$LATEST_MAJ" ] || { [ "$maj" -eq "$LATEST_MAJ" ] && [ "$min" -gt "$LATEST_MIN" ]; }; then
        LATEST="$f"
        LATEST_MAJ="$maj"
        LATEST_MIN="$min"
    fi
done

# If none found, fallback to OS-neutral
if [ -z "$LATEST" ]; then
    for f in ./bin/${PROJECT}_v*
    do
        [ -e "$f" ] || break

        ver=$(echo "$f" | sed -E 's/.*_v([0-9]+)\.([0-9]+).*/\1 \2/')
        maj=$(echo "$ver" | awk '{print $1}')
        min=$(echo "$ver" | awk '{print $2}')

        if [ "$maj" -gt "$LATEST_MAJ" ] || { [ "$maj" -eq "$LATEST_MAJ" ] && [ "$min" -gt "$LATEST_MIN" ]; }; then
            LATEST="$f"
            LATEST_MAJ="$maj"
            LATEST_MIN="$min"
        fi
    done
fi

# ---------------------------------------------------------------------
# Ensure we found something
# ---------------------------------------------------------------------
if [ -z "$LATEST" ]; then
    echo "Error: No binaries found for project '$PROJECT'."
    exit 1
fi

echo "Found newest binary: $LATEST"

# Determine extension
EXT=""
case "$LATEST" in
    *.exe) EXT=".exe" ;;
    *.dll) EXT=".dll" ;;
    *.so)  EXT=".so" ;;
    *.dylib) EXT=".dylib" ;;
    *) EXT="" ;;
esac

# Install to ~/bin
mkdir -p "$HOME/bin"
TARGET="$HOME/bin/${PROJECT}${EXT}"

cp "$LATEST" "$TARGET"

echo "Installed $LATEST to $TARGET"
echo "Done. Type '$PROJECT' to run it."