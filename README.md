# pmake

```
██████╗  ███╗   ███╗ █████╗ ██╗  ██╗███████╗
██╔══██╗ ████╗ ████║██╔══██╗██║ ██╔╝██╔════╝
██████╔╝ ██╔████╔██║███████║█████╔╝ █████╗  
██╔═══╝  ██║╚██╔╝██║██╔══██║██╔═██╗ ██╔══╝  
██║      ██║ ╚═╝ ██║██║  ██║██║  ██╗███████╗
╚═╝      ╚═╝     ╚═╝╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝
```

## About

pmake is a small, personal build tool I wrote to automate the repetitive parts of compiling. It
isn’t meant to replace make, cmake, or any of the industrial‑strength build systems out there.
It’s just a lightweight helper that fits the way I work: simple commands, predictable behavior,
and no hidden magic.

The tool handles the basics — compiling sources while keeping project structure consistent.
It’s intentionally minimal, because that’s all I need for the kind of projects I build in
my spare time.

## Why This Exists

I got tired of melting my brain just to write a Makefile. For small C projects, it makes no sense
that I should have to debug a build script that’s supposed to tell the compiler how to build my code.
Makefiles are powerful, but they’re also cryptic, inconsistent, and full of hidden behavior. One
wrong tab and the whole thing falls apart.

pmake (Patrik Make) exists because I wanted the opposite of that.

A pmake file is not a scripting language. It has no logic, no recursion, no conditionals, no magic.
It’s literally just compiler instructions in config‑file form: key=value. Simple, and self-explanatory.

## Folder Structure

The folders with binaries like executables, *.class - files, *.dll's will not be tracked.

```
pmake/
├── include/
│   ├── pmake.h
│   ├── samael.alchemy.manpage.h
│   ├── samael.alchemy.stringutility.h
│   ├── samael.chronicle.config.h
│   ├── samael.huginandmunin.debug.h
│   └── samael.huginandmunin.log.h
├── resources/
│   ├── doc/
│   │   └── pmake.docx
│   ├── pages/
│   │   └── pmake.pages
│   ├── pdf/
│   │   └── pmake.pdf
│   ├── pmake/
│   │   └── example.pmake
│   └── txt/
│   │   ├── pmake.txt
│   │   └── project.txt
├── scripts/
│   ├── compile.bat
│   ├── compile.ps1
│   ├── compile.sh
│   ├── get.bat
│   ├── get.ps1
│   ├── get.sh
│   ├── install.bat
│   ├── install.ps1
│   ├── install.sh
│   ├── new.bat
│   ├── new.ps1
│   ├── new.sh
│   ├── readme
│   ├── readme.bat
│   └── readme.ps1
├── src/
│   ├── main.c
│   ├── pmake.c
│   ├── samael.alchemy.manpage.c
│   ├── samael.alchemy.stringutility.c
│   ├── samael.chronicle.config.c
│   └── samael.huginandmunin.log.c
├── .gitignore
├── LICENSE
├── README.md
└── pmake.pmake
```

## Author

My name is Patrik Eigenmann. I learned to code when I was eight years old, long before it became my profession. Later I spent nine years as a software engineer, writing code because it was my job. When I moved into live sound, that obligation disappeared — but the curiosity stayed. I still write software because I want to understand things, not because someone assigns me a task. Coding keeps me sharp, and it gives me the freedom to explore ideas on my own terms.
I work on projects in my spare time, usually because something catches my interest or I want to figure out how something works under the hood. Nothing I build is backed by a team or a company. It’s just me, learning and creating because I enjoy it.
If you want to support my work, you can send a donation to p.eigenmann@gmx.net via PayPal. It’s optional, but appreciated.
Everything I publish is free under the GNU Public License v3.0. Use it, modify it, break it, rebuild it — whatever helps you learn or solve your own problems.

## Last Updated
Thu 2026-06-18
