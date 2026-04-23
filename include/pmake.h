// -------------------------------------------------------------------------------------------
// pmake.h - `pmake` isn’t here to replace your entire toolchain or teach you a new language. It exists because
// somewhere along the way, compiling a few lines of C code turned into a ceremony — full of build
// scripts that felt like mini-programs, declarations inside declarations, and files that read more
// like puzzles than instructions. Make is clever, maybe too clever. CMake has good intentions, but it's
// asking you to describe the very idea of a build system before touching a single .c file.
//
// This tool is none of that. No DSLs, no abstraction layers, no strange incantations. Just a small,
// direct program that reads your preferences and passes them to your compiler, like a polite assistant
// who doesn’t interrupt. If your project is sprawling, there are bigger hammers. But if you just want
// to compile your work without learning someone else’s language — that’s exactly where `pmake` steps in.
//
// It’s not trying to impress you. It’s assisting you in the process of building your software.
// -------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann72@gmail.com
// GitHub:  https://github.com/PatrikEigenmann72/helloc
// -------------------------------------------------------------------------------------------
// Change Log:
// Tue 2026-04-20 File created.                                                 Version: 00.01
// -------------------------------------------------------------------------------------------
#ifndef PMAKE_H
#define PMAKE_H

// ------------------------------------------------------------
// pmake identity
// ------------------------------------------------------------
extern const int MAJOR;
extern const int MINOR;
extern const char *APP_NAME;
extern const char *APP_EXTRACT_FILTER;

#endif