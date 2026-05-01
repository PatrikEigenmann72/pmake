// ****************************************************************************************************
// `pmake` isn’t here to replace your entire toolchain or teach you a new language. It exists because
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
// 
// To build `pmake`, the most direct way is to use your compiler of choice:
// gcc   -Wall -Wextra -std=c99 -Iinclude src/*.c -o bin/pmake  
// clang -Wall -Wextra -std=c99 -Iinclude src/*.c -o bin/pmake  
//
// If you'd rather hand the work off to a file, you've got options.
// Use `pmake.pmake` for self-compilation, `Makefile` for tradition,
// or `CMakeLists.txt` if that’s your flavor.
//
// Installation scripts are available too if you want `pmake` as a system-wide tool available.
// Check out `scripts/install.sh` for Unix-like systems or `install.cmd` on Windows.
// ----------------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann@gmx.net
// GitHub:  www.github.com/PatrikEigenmann/pmake
// ----------------------------------------------------------------------------------------------------
// Fri 2024-11-15 File created.                                                             Version: 00.01
// Sat 2024-11-16 Rearranged the whole program and renamed it. Fixed some bug too.          Version: 00.02
// Sat 2024-11-16 Bug Fix in putting the compiler command togheter.                         Version: 00.03
// Sat 2024-11-16 Rearranged process_makefile() for a clearer, top-down structure.          Version: 00.04
// Sat 2024-11-16 Fixed -c flag handling in process_makefile().                             Version: 00.05
// Sat 2024-11-16 Fixed .o suffix handling with -c flag in process_makefile().              Version: 00.06
// Tue 2024-11-19 Merged cVersion/cManPage into pmake.c for simplicity.                     Version: 00.07
// Thu 2024-11-21 Updated doesFileExist() to validate file and version.                     Version: 00.08
// Wed 2025-01-22 Updated header comment with GitHub URL updated.                           Version: 00.09
// Sun 2025-03-23 Target handles now obj, exec or shared.                                   Version: 00.10
// Sun 2025-03-23 Fixed a bug while handling "shared".                                      Version: 00.11
// Tue 2025-03-25 Changed the way the compiler command is put together.                     Version: 00.12
// Tue 2025-03-25 Shared Library under Windows is now a .dll and under Unix a .so.          Version: 00.13
// Sun 2025-04-06 Increased the library string to 10000 characters.                         Version: 00.14
// Sun 2025-04-06 New approach with a large library string. Using a isLibs flag.            Version: 00.15
// Sun 2025-04-06 BugFix in with the library string. strcpy wasn't working.                 Version: 00.16
// Sun 2025-04-06 BugFix in the library string. Switched to append_format.                  Version: 00.17
// Sun 2025-04-06 Making sure that the bug fix doesn't influence the make_process.          Version: 00.18
// Sun 2025-06-22 Loading now the file {project}.pmake instead of {project}.makefile.       Version: 00.19
// Sun 2025-06-22 Complete overhaul of this tool, because of an unfixable bug.              Version: 00.20
// Tue 2025-06-24 Renewed the manpage style help text because of its new functionality.     Version: 00.21
// Tue 2025-06-25 Updated the manpage style help text.                                      Version: 00.22
// Tue 2025-06-25 Updated the manpage text with how to get out of help.                     Version: 00.23
// Wed 2026-04-23 Swapping over to Samael framework, and rebuilding it because I found a    Version: 00.24
//                 bug on the Windows side.
// wed 2026-04-29 Outsourced everything to the project related header/source. Keeps main.c  Version: 00.25
//                simple and only as entry point of the application.
// Fri 2026-05-01 Implemented --version display, to be compatible with std UNIX tools.      Version: 00.26
// -----------------------------------------------------------------------------------------------------
// To Do's:
// - Take cVersion.h & cVersion.c appart and integrate it directly into this code base.             Done.                             Done.
// - Take cManPage.h & cManPage.c appart and integrate it directly into this code base.             Done.                             Done.
// - Check file exists before version in doesFileExists().                                          Done.
// - Make sure if the library string isn't in the end, the program doesn't crash.                   Done.
// - Implement linker flags directive in the pmake file.                                            Done.
// - Implement -DDEBUG flag through command shell parameter.                                        Done.
// - Implement --version UNIX compliant.                                                            Done.
// *****************************************************************************************************/

// Standard C headers — the foundations we all lean on. These handle essential tasks like printing to
// the terminal, managing dynamic memory, and working with plain old C strings. Nothing fancy — just
// the timeless utilities that keep everything ticking.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Samael framework headers - Samael is growing C and Java library with ease of use tools and application
// helpers, that bring joy to programming CLI or UI driven applications.
#include "samael.huginandmunin.debug.h"
#include "samael.huginandmunin.log.h"
#include "samael.alchemy.manpage.h"
#include "samael.alchemy.stringutility.h"

// Project headers — the parts that breathe life into this tool. Here’s where things get specific:
// `parse.h` defines how we read the `.pmake` file, `debug.h` helps us talk to ourselves when things
// go sideways, `version.h` keeps the numbers honest, and `manpage.h` handles the help text so the user
// isn’t left guessing. These aren’t just utilities — they’re the behaviors that make `pmake` act like
// a tool, not just a compiled blob.
#include "pmake.h"

// -----------------------------------------------------------------------------------------------------
// int main(int argc, char **argv) - This is where execution begins. The main-function serves as the
// entry point for all C and C++ programs that actually do something. When arguments are provided,
// they’re passed through `argc` (the argument count) and `argv` (argument vector) holds the actual inputs
// as strings — the first one’s always the program name.
// 
// The function returns an exit status to the operating system. I use EXIT_SUCCESS and EXIT_FAILURE —
// symbolic constants defined in <stdlib.h> — instead of raw integers like 0 or 1. It keeps things clear,
// portable, and easy to read.
// 
// @param argc  Number of arguments passed to the program
// @param argv  The actual arguments, starting with the program name itself
// @return      EXIT_SUCCESS on successful completion, EXIT_FAILURE if something goes wrong
// -----------------------------------------------------------------------------------------------------
int main(int argc, char **argv) {
    
    // If no arguments were provided, or the user asked for help explicitly, print the help text and exit
    // cleanly. A program that can’t explain itself isn’t ready to be used — this one does, and it does
    // so without assuming you already know what you’re doing.
    if (argc < 2 || manpage_is_help_triggered(argc, argv)) {
        manpage_display();

        //print_help(v);
        return EXIT_SUCCESS;
    }

    // After checking if the help or "manpage" is called, the next UNIX compliant standard is to check for
    // --version flag. If the user asks for the version of the application. Display the component meta data.
    if (argc > 1 && strcmp(argv[1], "--version") == 0) {
        component_display();
        return EXIT_SUCCESS;
    }

    // In case something goes wrong, the error message is stored here.
    char *errmsg = NULL;
    
    // Normalize the filename from the user's input.
    // Then debug it, so we know what we're working with.
    char *filename = normalize_filename(argv[1]);
    debug_info("filename = '%s'\n", filename); 
    
    // Parse the provided `.pmake` file into a structured format.
    // This returns a Makefile pointer with all relevant fields filled out — or
    // sets `errmsg` if something goes wrong before we can proceed.
    Makefile *mf = parse(filename, &errmsg);
 
    // Remove any occurrence of "-DDEBUG" from the pmakefile flags. The pmakefile
    // must never influence whether the build is a debug or release build; that
    // decision is controlled exclusively through the command line. This ensures
    // predictable behavior and prevents accidental debug builds caused by stale
    // configuration entries.
    char *clean_flags = strip_debug_flag(mf->flags);
    free(mf->flags);
    mf->flags = clean_flags;

    // Detect whether the user explicitly passed "-DDEBUG" on the command line.
    // Only an exact, case-sensitive match enables debug mode. This ensures that
    // debug builds are always intentional and never influenced by the pmakefile.
    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-DDEBUG") == 0) {
            debug_from_cli = 1;
        }
    }

    // Debugging output to help understand what the program is doing. This is useful during development.
    // Debug outputs can be turned on with the `-DDEBUG` flag during compilation.
    debug_info("comp    = '%s'\n", mf->comp);
    debug_info("flags   = '%s'\n", mf->flags);
    debug_info("target  = '%s'\n", mf->target);
    debug_info("bin     = '%s'\n", mf->bin);
    debug_info("src     = '%s'\n", mf->src);
    debug_info("libs    = '%s'\n", mf->libs);
    debug_info("project = '%s'\n", mf->project);

    // If an error message was returned during parsing or setup, print it, clean up the allocated string,
    // and exit with failure. The message goes to stdout — this tool doesn't pretend it's more than it is.
    if (errmsg) {
        printf("Error: %s\n", errmsg);
        free(errmsg);
        free_makefile(mf);
        return EXIT_FAILURE;
    }

    // Kick off the build process using the parsed makefile. If something goes wrong, `errmsg` gets
    // populated and handled downstream. `run()` is the part that turns config into action.
    run(mf, &errmsg);

    // If something broke during execution, report the error, free the dynamically allocated error
    // message, and clean up the makefile data. Leaving no mess behind — even when things don't go
    // according to plan.
    if (errmsg) {
        printf("Error: %s\n", errmsg);
        free(errmsg);
        free_makefile(mf);
        return EXIT_FAILURE;
    }

    // Clean up any remaining data tied to the makefile before exiting successfully.
    // If we made it here, the build ran without errors — no drama, no leftovers.
    free_makefile(mf);
    return EXIT_SUCCESS;
}