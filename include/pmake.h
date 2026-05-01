// ------------------------------------------------------------------------------------------------
// pmake.h - `pmake` isn’t here to replace your entire toolchain or teach you a new language. It
// exists because somewhere along the way, compiling a few lines of C code turned into a ceremony —
// full of build scripts that felt like mini-programs, declarations inside declarations, and files
// that read more like puzzles than instructions. Make is clever, maybe too clever. CMake has good
// intentions, but it's asking you to describe the very idea of a build system before touching a
// single .c file.
//
// This tool is none of that. No DSLs, no abstraction layers, no strange incantations. Just a small,
// direct program that reads your preferences and passes them to your compiler, like a polite
// assistant who doesn’t interrupt. If your project is sprawling, there are bigger hammers. But if
// you just want to compile your work without learning someone else’s language — that’s exactly
// where `pmake` steps in.
//
// It’s not trying to impress you. It’s assisting you in the process of building your software.
// ------------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann72@gmail.com
// GitHub:  https://github.com/PatrikEigenmann72/helloc
// ------------------------------------------------------------------------------------------------
// Change Log:
// Tue 2026-04-20 File created.                                                     Version: 00.01
// Wed 2026-04-29 Added methods display_manpage.                                    Version: 00.02
// Wed 2026-04-29 Added functionallity from parse.c and removed version.c.          Version: 00.03
// Fri 2026-05-01 Switched globals to preprocessor constants for immutability.      Version: 00.04
// ------------------------------------------------------------------------------------------------
#ifndef PMAKE_H
#define PMAKE_H

// Indicates whether the user explicitly requested a debug build by passing
// the exact command-line token "-DDEBUG". This flag is defined in pmake.c
// and read by the build logic to ensure that debug mode is controlled only
// through the command line and never by the pmakefile.
extern int debug_from_cli;

// This struct represents the parsed contents of a simple build configuration file. Each field stores
// a relevant directive: the compiler to use, flags to pass, target type (e.g., executable, shared
// library), and paths for source, output, and optional libraries. Designed for clarity and direct use
// in small CLI tools.
typedef struct {
    char *comp;
    char *flags;
    char *target;
    char *project;
    char *bin;
    char *src;
    char *libs;
} Makefile;

// --------------------------------------------------------------------------------
// Parse a build configuration file and return a populated Makefile struct.
// If parsing fails, errmsg will point to a dynamically allocated error message.
//
// @param filename  Path to the config file to parse
// @param errmsg    Pointer to store error message (set to NULL on success)
// @return          Allocated Makefile* on success, NULL on failure
// --------------------------------------------------------------------------------
Makefile *parse(const char *filename, char **errmsg);

// --------------------------------------------------------------------------------
// Execute the build process based on the provided Makefile configuration.
// Translates the struct fields into a compiler command and invokes it.
//
// On failure, errmsg will point to an allocated string describing the issue.
// Caller is responsible for freeing errmsg if set.
//
// @param mf      Parsed build configuration
// @param errmsg  Output pointer for error messages (set to NULL on success)
// --------------------------------------------------------------------------------
void run(const Makefile *mf, char **errmsg);

// --------------------------------------------------------------------------------
// Free all dynamically allocated memory associated with a Makefile struct.
// Safely deallocates each field and then the struct itself.
//
// @param mf  Pointer to a Makefile previously returned by parse()
//            (or NULL, in which case nothing happens)
// --------------------------------------------------------------------------------
void free_makefile(Makefile *mf);

// --------------------------------------------------------------------------------
// Convert a user-supplied filename into a normalized format.
// This may add or adjust extensions, strip leading paths, or enforce consistency
// in naming conventions. Useful for resolving shorthand or ambiguous filenames.
//
// @param input  Raw filename or command-line argument
// @return       Dynamically allocated, normalized filename string (caller frees)
// --------------------------------------------------------------------------------
char *normalize_filename(const char *input);

// -------------------------------------------------------------------------------------------
// manpage_display
//
// Builds and displays the application's integrated manpage. This function assembles all
// sections—name, synopsis, description, options, and license—into a complete help page and
// delegates the final rendering to manpage_init(). The content is constructed dynamically
// using append_format(), which keeps the layout flexible and easy to maintain.
//
// The design mirrors the UNIX manpage structure so developers and system administrators feel
// immediately at home. This function is only invoked when help flags are detected, and its
// output replaces normal program execution.
//
// @note Memory for all dynamically built sections is allocated here and freed before return.
// -------------------------------------------------------------------------------------------
void manpage_display();

/**
 * Removes all exact occurrences of the token "-DDEBUG" from a space‑separated
 * compiler flag string. This ensures that debug mode is controlled exclusively
 * through the command line and never influenced by the pmakefile. The comparison
 * is strict and case‑sensitive, preserving all other flags exactly as provided.
 *
 * @param flags The original flag string from the pmakefile, or NULL if none.
 * @return A newly allocated string with all "-DDEBUG" tokens removed, or NULL
 *         if the input was NULL or memory allocation failed. Caller frees.
 */
char *strip_debug_flag(const char *flags);

// -------------------------------------------------------------------------------------------
// component_display
//
// Prints the tool's identity block.
// Shows name, version, author, contact, repository, license, and build date.
// Used when the --version flag is provided.
// No dynamic memory.
// No side effects beyond stdout.
// Intended as a stable, human‑readable provenance entry point.
// -------------------------------------------------------------------------------------------
void component_display(void);
#endif