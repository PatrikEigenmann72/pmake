@echo off
REM ------------------------------------------------------------------------------------
REM Script:       readme.bat
REM Description:  Regenerates README.md with About, Author, and a live file tree.
REM ------------------------------------------------------------------------------------
REM Author:       Patrik Eigenmann
REM eMail:        p.eigenmann72@gmail.com
REM GitHub:       https://github.com/PatrikEigenmann72
REM ------------------------------------------------------------------------------------
REM Change log:
REM Mon 2026-04-13 File created.                                          Version: 00.01
REM Tue 2026-04-14 Added the About and Author part.                       Version: 00.02
REM Wed 2026-04-15 First attempt to produce a live tree of the active d.  Version: 00.03
REM Thu 2026-04-16 Second attempt to produce a live tree of the directory.Version: 00.04
REM Sun 2026-04-19 Decided to write a tree clone, because MacOS doesn't ship
REM                with the UNIX tree tool organically. And I don't like homebrew,
REM                so I homebrewed myself -> happy having some C skills.  Version: 00.05
REM Wed 2026-04-22 Fixing a bug, display issues and heredoc issues.       Version: 00.06
REM ------------------------------------------------------------------------------------

REM Determine project name from current directory
for %%I in ("%CD%") do set PROJECT=%%~nI

REM Today's date (Windows format)
for /f "tokens=1-3 delims=/ " %%a in ("%date%") do (
    set TODAY=%date%
)

REM Run treeclone.exe and capture output
treeclone > tree_output.tmp

REM Start writing README.md
(
echo # %PROJECT%
echo.
echo ## About
echo.
echo The original tree command didn’t come from UNIX at all — it first appeared in the DOS world in the 1980s, where early PC operating systems needed a simple way to visualize directory
echo structures. Over time, the idea spread, and the version most developers know today — the one installed on Linux and BSD systems — was written by Steve Baker, whose implementation
echo became the de‑facto standard across UNIX‑like environments. Other variants appeared in FreeDOS (by Dave Dunfield) and ReactOS (by Asif Bahrainwala), but Baker’s version is the one that shaped
echo how developers think about tree today.
echo.
echo "treeclone" is a small, focused re‑implementation of the classic tree utility. It doesn’t aim to replicate every feature of Steve Baker’s version, and it doesn’t include the dozens of flags and formatting modes that the original tool accumulated over the years. Instead, it concentrates on the core behavior that makes tree useful in the first place: walking a directory, sorting entries, and printing a clean, readable ASCII tree.
echo The tool supports a single command‑line option, -exclude, which filters out files or directories matching simple patterns. For my workflow, that’s the only feature I actually need. Everything else — depth limiting, color output, metadata, permissions, timestamps — is intentionally left out. The goal is clarity, not completeness.
echo Internally, treeclone uses a lightweight C kernel with deterministic sorting, directory‑first ordering, and always‑on logging. The output is stable, predictable, and easy to embed into documentation or tooling. It’s a compact utility that does one job well: generate a clean directory tree without external dependencies.
echo.
echo ## Why This Exists
echo.
echo Before treeclone, I tried to generate directory trees on macOS using shell scripts. It was clumsy, inconsistent, and never produced output I actually liked. macOS doesn’t ship with the tree command, and every search result pointed to the same solution: install Homebrew. But Homebrew brings a lot with it — packages, dependencies, updates, and a whole ecosystem I didn’t want just to get one simple tool.
echo I know enough languages to be dangerous — C, Java, PHP, Python — but not enough in any of them to write a perfect clone of the original tree. Still, if I was going to “homebrew” something, I wanted to do it properly. A small, clean C program made the most sense: no dependencies, no package managers, no surprises. Just compile it and run it.
echo So treeclone exists because I needed a lightweight, predictable way to generate directory trees on macOS without pulling in an entire package manager. It’s not a full replacement for the original tree, and it doesn’t try to be. It’s a simple tool that solves a simple problem — and for my workflow, that’s enough.
echo.
echo ## Folder Structure
echo.
echo The folders with binaries like executables, *.class - files, *.dll's will not be tracked.
echo.
echo ```
) > README.md

REM Append tree output
type tree_output.tmp >> README.md
echo ``` >> README.md

REM Append Author section
(
echo.
echo ## Author
echo.
echo My name is Patrik Eigenmann. I learned to code when I was eight years old, long before it became my profession. Later I spent nine years as a software engineer, writing code because it was my job. When I moved into live sound, that obligation disappeared — but the curiosity stayed. I still write software because I want to understand things, not because someone assigns me a task. Coding keeps me sharp, and it gives me the freedom to explore ideas on my own terms.
echo I work on projects in my spare time, usually because something catches my interest or I want to figure out how something works under the hood. Nothing I build is backed by a team or a company. It’s just me, learning and creating because I enjoy it.
echo If you want to support my work, you can send a donation to p.eigenmann@gmx.net via PayPal. It’s optional, but appreciated.
echo Everything I publish is free under the GNU Public License v3.0. Use it, modify it, break it, rebuild it — whatever helps you learn or solve your own problems.
echo.
echo ## Last Updated
echo %TODAY%
) >> README.md

del tree_output.tmp

echo README.md regenerated successfully.