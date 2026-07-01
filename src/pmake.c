// -----------------------------------------------------------------------------------------------
// pmake.c - `pmake` isn’t here to replace your entire toolchain or teach you a new language. It
// exists because somewhere along the way, compiling a few lines of C code turned into a ceremony
// — full of build scripts that felt like mini-programs, declarations inside declarations, and
// files that read more like puzzles than instructions. Make is clever, maybe too clever. CMake
// has good intentions, but it's asking you to describe the very idea of a build system before
// touching a single .c file.
//
// This tool is none of that. No DSLs, no abstraction layers, no strange incantations. Just a
// small, direct program that reads your preferences and passes them to your compiler, like a
// polite assistant who doesn’t interrupt. If your project is sprawling, there are bigger hammers.
// But if you just want to compile your work without learning someone else’s language — that’s
// exactly where `pmake` steps in.
//
// It’s not trying to impress you. It’s assisting you in the process of building your software.
// -----------------------------------------------------------------------------------------------
// Author:  Patrik Eigenmann
// eMail:   p.eigenmann72@gmail.com
// GitHub:  https://github.com/PatrikEigenmann72/helloc
// -----------------------------------------------------------------------------------------------
// Change Log:
// Tue 2026-04-20 File created.                                                     Version: 00.01
// Wed 2026-04-29 Added methods display_manpage.                                    Version: 00.02
// Wed 2026-04-29 Added functionallity from parse.c and removed version.c.          Version: 00.03
// Fri 2026-05-01 Cleaned up the display_manpage and made it more consice.          Version: 00.04
// Fri 2026-05-01 Switched globals to preprocessor constants for immutability.      Version: 00.05
// Fri 2026-05-01 Refactored manpage_display to reflect the current state.          Version: 00.06
// Tue 2026-06-30 Integrating OS gnosticism.                                        Version: 00.07
// Tue 2026-06-30 Adding the version number to the binary name.                     Version: 00.08
// Tue 2026-06-30 If pmake is os gnostic, adding the OS name to the binary name.    Version: 00.09
// Tue 2026-06-30 BugFix reading the correct version number.                        Version: 00.10
// -----------------------------------------------------------------------------------------------
#include "pmake.h"

// Standard C headers — the foundations we all lean on. These handle essential tasks like printing
// to the terminal, managing dynamic memory, and working with plain old C strings. Nothing fancy —
// just the timeless utilities that keep everything ticking.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Samael framework imports
#include "samael.huginandmunin.debug.h"
#include "samael.huginandmunin.log.h"
#include "samael.alchemy.manpage.h"
#include "samael.alchemy.stringutility.h"

// ------------------------------------------------------------
// Identity constants (private to pmake)
// ------------------------------------------------------------
#define APP_NAME    "pmake"
#define MAJOR 0
#define MINOR 10

#define AUTHOR      "Patrik Eigenmann"
#define EMAIL       "p.eigenmann72@gmail.com"
#define GIT         "https://www.github.com/PatrikEigenmann72/pmake.git"
#define LICENSE     "GNU GPL 3.0"
#define BUILD_DATE  __DATE__

// Indicates whether the user explicitly requested a debug build by passing
// the exact command-line token "-DDEBUG". This flag is defined in pmake.c
// and read by the build logic to ensure that debug mode is controlled only
// through the command line and never by the pmakefile.
int debug_from_cli = 0;

// -------------------------------------------------------------------------------------------
// manpage_display
//
// Builds and displays the application's integrated manpage. This function assembles all
// sections—name, synopsis, description, options, and license—into a complete help page and
// delegates the final rendering to manpage_init(). The content is constructed dynamically
// using append_format(), which keeps the layout flexible and easy to maintain.
//
// The layout mirrors the UNIX manpage structure so developers and system administrators feel
// immediately at home. This function is only invoked when help flags are detected, and its
// output replaces normal program execution.
//
// @note Memory for all dynamically built sections is allocated here and freed before return.
// -------------------------------------------------------------------------------------------
void manpage_display() {

    log_info("Assembling and displaying the manpage content.");
    debug_info("Assembling and displaying the manpage content.");

    int major = MAJOR;
    log_info("Setting major version to %02d.", major);
    debug_info("Setting major version to %02d.", major);

    int minor = MINOR;
    log_info("Setting minor version to %02d.", minor);
    debug_info("Setting minor version to %02d.", minor);

    // NAME
    char *name = NULL;
    append_format(&name, APP_NAME);
    log_verbose("Appended application name: %s.", name);

    // SYNOPSIS
    char *synopsis = NULL;
    append_format(&synopsis,
        "      %s [makefile] [-DDEBUG] [-h | -H | -help | -Help | -?]",
        name
    );
    log_info("Built synopsis content.");

    // DESCRIPTION
    char *description = NULL;
    append_format(&description,
        "      %s reads a <project>.pmake file and translates its contents into compiler\n"
        "      instructions. There is no scripting language, no hidden behavior, and no\n"
        "      abstraction layer. The file is intentionally simple so the developer stays\n"
        "      in control and always understands what the compiler is doing.\n"
        "\n"
        "      %s is designed for people who prefer clarity over complexity. If you want a\n"
        "      build tool that behaves exactly as it says—nothing more, nothing less—%s\n"
        "      stays out of your way.\n",
        name, name, name
    );
    log_info("Built description content.");

// OPTIONS
    char *options = NULL;
    append_format(&options,
        "      makefile\n"
        "          A project makefile is named <project>.pmake. It contains the compiler\n"
        "          settings for the project in a minimal, human-readable format. There are\n"
        "          no conditionals, loops, or DSL constructs—just straightforward\n"
        "          configuration.\n"
        "\n"
        "      -DDEBUG\n"
        "          Enables debug compilation. This flag is only accepted on the command\n"
        "          line; entries inside <project>.pmake are ignored. Without this flag,\n"
        "          %s produces a release build.\n"
        "\n"
        "      -h, -H, -help, -Help, -?\n"
        "          Displays the integrated manpage. All variants are accepted for\n"
        "          convenience.\n"
        "\n"
        "      --version\n"
        "          Prints the tool's identity block and exits.\n",
        name
    );
    log_info("Built options content.");

    // LICENSE
    char *license = NULL;
    append_format(&license,
        "      Copyright 2024 Free Software Foundation, Inc. License GPLv3+: GNU GPL\n"
        "      version 3 or later <https://gnu.org/licenses/gpl.html>. This is free\n"
        "      software: you are free to change and redistribute it. There is NO WARRANTY,\n"
        "      to the extent permitted by law.\n"
    );
    log_info("Built license content.");

    // Initialize manpage
    log_info("Initializing manpage with assembled content.");
    manpage_init(major, minor, name, synopsis, description, options, license);

    // Cleanup
    free(name);
    free(synopsis);
    free(description);
    free(options);
    free(license);

    log_info("Freed all allocated manpage content.");
}

// --------------------------------------------------------------------------------
// Allocate a duplicate of the given C-string. If the input is NULL, returns NULL
// immediately. Otherwise, dynamically allocates memory and copies the contents
// using strdup(). This function simplifies parsing logic by removing the need for
// repetitive NULL checks around optional fields.
//
// It favors clarity over cleverness — safe, predictable, and in step with the rest
// of the minimalist toolkit you're crafting.
//
// @param s     Pointer to the source string (may be NULL)
// @return      Newly allocated copy of the string, or NULL if input was NULL
// --------------------------------------------------------------------------------
static char *dupstr(const char *s) {
    return s ? strdup(s) : NULL;
}

// --------------------------------------------------------------------------------
// Assign a default value to a field if it is currently NULL. This utility helps
// populate optional configuration fields with fallback values, avoiding scattered
// conditional checks and maintaining cleaner parsing logic. If the provided field
// already points to a value, it is left unchanged. Otherwise, it is dynamically
// allocated using strdup(fallback).
//
// This pattern keeps config loading predictable and reduces coupling between
// user-provided and internally assumed values.
//
// @param field     Pointer to the target field (char**) that may be NULL
// @param fallback  Default string to assign if *field is NULL
// --------------------------------------------------------------------------------
static void set_default_if_null(char **field, const char *fallback) {
    if (*field == NULL) *field = strdup(fallback);
}

// --------------------------------------------------------------------------------
// Parse a build configuration file and return a populated Makefile struct. Opens
// the given file and reads key-value pairs line by line, skipping empty lines and
// comments. Recognized keys include comp, flags (or cflags), target, project, bin,
// src, and libs. If optional fields like comp, bin, or src are not provided, they
// are set to sensible defaults. Unknown keys are ignored silently.
//
// The returned Makefile struct reflects the parsed configuration and can be used to construct
// compiler commands or introspect project metadata. Callers are responsible for freeing the struct
// using free_makefile().
//
// If parsing fails — due to I/O errors or missing required fields (project or target) — the function
// returns NULL and sets errmsg to a heap-allocated message describing the problem.
//
// @param filename  Path to the configuration file
// @param errmsg    Pointer to store an error message if parsing fails (set to NULL on success)
// @return          Pointer to a populated Makefile, or NULL on failure
// --------------------------------------------------------------------------------
Makefile *parse(const char *filename, char **errmsg) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        size_t len = snprintf(NULL, 0, "Could not open file: %s", filename) + 1;
        *errmsg = malloc(len);
        if (*errmsg) snprintf(*errmsg, len, "Could not open file: %s", filename);
        return NULL;
    }

    Makefile *mf = calloc(1, sizeof(Makefile));
    if (!mf) {
        fclose(fp);
        *errmsg = strdup("Memory allocation failed for Makefile structure.");
        return NULL;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\n")] = 0;
        line[strcspn(line, "\r")] = 0;
        debug_info(">>> LINE: '%s'\n", line);
        if (line[0] == '\0' || line[0] == '#') continue;

        if (strncmp(line, "comp=", 5) == 0) {
            mf->comp    = dupstr(line + 5);
            debug_info("Parsed compiler directive as: '%s'\n", mf->comp);
        }
        else if (strncmp(line, "flags=", 6) == 0)           mf->flags = dupstr(line + 6);
        else if (strncmp(line, "cflags=", 7) == 0)          mf->flags = dupstr(line + 7);
        else if (strncmp(line, "target=", 7) == 0)          mf->target  = dupstr(line + 7);
        else if (strncmp(line, "project=", 8) == 0)         mf->project = dupstr(line + 8);
        else if (strncmp(line, "bin=", 4) == 0)             mf->bin     = dupstr(line + 4);
        else if (strncmp(line, "src=", 4) == 0)             mf->src     = dupstr(line + 4);
        else if (strncmp(line, "libs=", 5) == 0)            mf->libs    = dupstr(line + 5);
        // Adding in the os gnosticism field for building the binary name with correct extension.
        else if (strncmp(line, "is_os_gnostic=", 14) == 0)  mf->os_gnostic = dupstr(line + 14);
    }

    fclose(fp);

    set_default_if_null(&mf->comp, "gcc");
    set_default_if_null(&mf->bin, "./bin");
    set_default_if_null(&mf->src, "./src/main.c");

    if (!mf->project || !mf->target) {
        *errmsg = strdup("Missing required field(s): project or target.");
        free_makefile(mf);
        return NULL;
    }

    return mf;
}

// --------------------------------------------------------------------------------
// Construct and execute a build command using the given Makefile configuration. This
// function assembles a full compiler command string by evaluating the fields in the
// Makefile: compiler (comp), flags, target type, source files, libraries, output
// path, and project name. It applies platform-specific behavior to choose
// appropriate output file extensions based on the target type (e.g., .so, .dll,
// .obj, etc.).
//
// Once constructed, the command is executed using system(). If execution fails,
// errmsg is set to a descriptive message allocated on the heap. On success, errmsg
// remains NULL.
//
// This approach favors predictability and portability over shell scripting — it
// gives the build logic a clear C representation, decoupling it from external
// Makefiles or tooling.
//
// @param mf       Pointer to a fully populated Makefile configuration
// @param errmsg   Output parameter to store an error string if the build fails (NULL on success)
// --------------------------------------------------------------------------------
void run(const Makefile *mf, char **errmsg) {
    char *cmd = malloc(1024);
    if (!cmd) {
        *errmsg = strdup("Memory allocation failed for build command.");
        return;
    }

    cmd[0] = '\0';
    snprintf(cmd, 1024, "%s ", mf->comp);

    // Append pmakefile flags (already cleaned of -DDEBUG)
    if (mf->flags) {
        strncat(cmd, mf->flags, 1023 - strlen(cmd));
        strncat(cmd, " ", 1023 - strlen(cmd));
    }

    // Append -DDEBUG only if the user explicitly requested a debug build.
    // The pmakefile never influences debug mode; this ensures that debug
    // builds are always intentional and controlled solely through the CLI.
    if (debug_from_cli) {
        strncat(cmd, "-DDEBUG ", 1023 - strlen(cmd));
    }

    if (strcmp(mf->target, "lib") == 0) {
        strncat(cmd, "-shared ", 1023 - strlen(cmd));
    }

    strncat(cmd, mf->src, 1023 - strlen(cmd));
    strncat(cmd, " ", 1023 - strlen(cmd));

    if (mf->libs) {
        strncat(cmd, mf->libs, 1023 - strlen(cmd));
        strncat(cmd, " ", 1023 - strlen(cmd));
    }

    // ---------------------------------------------------------------------
    // Read version from src/<project>.c
    // ---------------------------------------------------------------------
    char src_path[256];
    snprintf(src_path, sizeof(src_path), "src/%s.c", mf->project);

    FILE *vh = fopen(src_path, "r");
    int vmaj = 0, vmin = 0;

    if (vh) {
        char buf[256];
        while (fgets(buf, sizeof(buf), vh)) {
            if (sscanf(buf, "#define MAJOR %d", &vmaj) == 1) continue;
            if (sscanf(buf, "#define MINOR %d", &vmin) == 1) continue;
        }
        fclose(vh);
    }

    char version_suffix[32];
    snprintf(version_suffix, sizeof(version_suffix), "_v%02d.%02d", vmaj, vmin);

    strncat(cmd, "-o ", 1023 - strlen(cmd));
    strncat(cmd, mf->bin, 1023 - strlen(cmd));
    strncat(cmd, "/", 1023 - strlen(cmd));
    strncat(cmd, mf->project, 1023 - strlen(cmd));

    // Check if the build has to be OS gnostic.
    int os_gnostic = 0;
    if (mf->os_gnostic &&
        (strcmp(mf->os_gnostic, "on") == 0 ||
        strcmp(mf->os_gnostic, "1") == 0))
    {
        os_gnostic = 1;
    }

    // Building the os suffix for the binary name.
    char os_suffix[16];
    os_suffix[0] = '\0';   // default: empty (OS-neutral)

    // Apply the os-specific suffix only if the pmakefile explicitly requests it.
    if (os_gnostic) {
    #ifdef _WIN32
        strcpy(os_suffix, "_win");
    #elif __APPLE__
        strcpy(os_suffix, "_mac");
    #else
        strcpy(os_suffix, "_linux");
    #endif
    }

    // Append the OS suffix to the output binary name if applicable. This ensures that builds
    // targeting different operation systems produce uniquely named artifacts.
    strncat(cmd, os_suffix, 1023 - strlen(cmd));

    // Append version suffix to the output binary name. This ensures that each build
    // produces a uniquely named artifact, allowing multiple versions to coexist.
    strncat(cmd, version_suffix, 1023 - strlen(cmd));

// ---------------------------------------------------------------------
// Append correct extension based on OS and target type
// ---------------------------------------------------------------------
#ifdef _WIN32
    if (strcmp(mf->target, "lib") == 0)
        strncat(cmd, ".dll", 1023 - strlen(cmd));
    else if (strcmp(mf->target, "obj") == 0)
        strncat(cmd, ".obj", 1023 - strlen(cmd));
    else
        strncat(cmd, ".exe", 1023 - strlen(cmd));

#elif __APPLE__
    if (strcmp(mf->target, "lib") == 0)
        strncat(cmd, ".dylib", 1023 - strlen(cmd));
    else if (strcmp(mf->target, "obj") == 0)
        strncat(cmd, ".o", 1023 - strlen(cmd));
    // executables on macOS have no extension

#else   // Linux / Unix
    if (strcmp(mf->target, "lib") == 0)
        strncat(cmd, ".so", 1023 - strlen(cmd));
    else if (strcmp(mf->target, "obj") == 0)
        strncat(cmd, ".o", 1023 - strlen(cmd));
    // executables on Linux have no extension
#endif

    printf("Compiling:\n%s\n", cmd);

    int result = system(cmd);
    if (result != 0) {
        *errmsg = strdup("Build command failed.");
    }

    free(cmd);
}

// --------------------------------------------------------------------------------
// Free all memory associated with a Makefile struct. Releases each dynamically
// allocated field within the struct, followed by the struct itself. This function
// is NULL-safe — if mf is NULL, it returns immediately without error. Intended to
// pair with parse(), ensuring proper cleanup after build configuration usage.
//
// @param mf  Pointer to a Makefile previously returned by parse()
// --------------------------------------------------------------------------------
void free_makefile(Makefile *mf) {
    if (!mf) return;
    free(mf->comp);
    free(mf->flags);
    free(mf->target);
    free(mf->project);
    free(mf->bin);
    free(mf->src);
    free(mf->libs);
    free(mf);
}

// --------------------------------------------------------------------------------
// Normalize a filename by removing its extension and appending ".pmake". This
// utility ensures consistent naming for configuration files by stripping any existing
// extension from the input and replacing it with the standard ".pmake" suffix. If no
// extension is present, the suffix is simply appended.
//
// The returned string is heap-allocated and must be freed by the caller. If allocation
// fails, NULL is returned.
//
// Examples:
//   "build.conf"   → "build.pmake"
//   "project"      → "project.pmake"
//
// @param input   Original filename (must be a valid, null-terminated string)
// @return        Newly allocated normalized string, or NULL on allocation failure
// --------------------------------------------------------------------------------
char *normalize_filename(const char *input) {
    const char *ext = strrchr(input, '.');  // Find last '.'
    size_t len = ext ? (size_t)(ext - input) : strlen(input);

    char *normalized = malloc(len + strlen(".pmake") + 1);
    if (!normalized) return NULL;

    strncpy(normalized, input, len);
    normalized[len] = '\0';
    strcat(normalized, ".pmake");

    return normalized;
}

/**
 * Removes all exact occurrences of the token "-DDEBUG" from a space‑separated
 * compiler flag string. This ensures that debug mode is never influenced by
 * the pmakefile and is instead controlled exclusively through the command line.
 * The function performs a strict, case‑sensitive comparison and preserves all
 * other flags exactly as provided, maintaining predictable and minimalistic
 * behavior aligned with pmake’s design philosophy.
 *
 * @param flags The original flag string from the pmakefile, or NULL if none.
 * @return A newly allocated string with all "-DDEBUG" tokens removed, or NULL
 *         if the input was NULL or memory allocation failed. Caller frees.
 */
char *strip_debug_flag(const char *flags) {
    if (!flags) return NULL;

    char *clean = malloc(strlen(flags) + 1);
    if (!clean) return NULL;

    clean[0] = '\0';

    char *copy = strdup(flags);
    char *token = strtok(copy, " ");

    while (token) {
        if (strcmp(token, "-DDEBUG") != 0) {
            strcat(clean, token);
            strcat(clean, " ");
        }
        token = strtok(NULL, " ");
    }

    free(copy);

    size_t len = strlen(clean);
    if (len > 0 && clean[len - 1] == ' ')
        clean[len - 1] = '\0';

    return clean;
}

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
void component_display(void)
{
    printf("%s - Version %02d.%02d\n", APP_NAME, MAJOR, MINOR);
    printf("Author: %s\n", AUTHOR);
    printf("eMail: %s\n", EMAIL);
    printf("GitHub: %s\n", GIT);
    printf("License: %s\n", LICENSE);
    printf("Build Date: %s\n", BUILD_DATE);
}