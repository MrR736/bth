/* bth.c - Bytes To Header Utility
 *
 * Copyright (C) 2025 MrR736 <MrR736@users.github.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * The complete text of the GNU General Public License
 * can be found in /usr/share/common-licenses/GPL-3 file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/stat.h>

#include "bth.h"

void print_usage(const char *progname) {
    vprint("Usage: %s -i <input> -o <output> -f <name> [-c <count>] [-v] [-s] [-t] [-m, --ms-sys]\n\n"
            "<Options>\n"
            "  --help, -h       Display this help and exit\n"
            "  --version, -V    Display version information and exit\n"
            "  -i <input>       Input file\n"
            "  -o <output>      Output file\n"
            "  -f <name>        Name of generated array\n"
            "  -c <count>       Number of bytes to read (Max: %s)\n"
            "  -v               Verbose output\n"
            "  -s               Silent mode\n"
            "  -t               Text mode\n"
            "  -m, --ms-sys     MS-SYS mode\n"
            "  -e               Tests mode\n"
            "  -h               Show this help message", progname, TEXT_MAX);
}

void print_version(const char *progname) {
    vprint("%s (Bytes To Header) %s\n\nWritten by MrR736.", progname, VERSION);
}

char *sanitize_name(const char *name) {
    if (!name || !*name) return NULL;
    char *sanitized = strdup(name);
    if (!sanitized) return NULL;
    if (!isalpha(sanitized[0]) && sanitized[0] != '_') sanitized[0] = '_';
    for (char *c = sanitized + 1; *c; c++) {
        if (!isalnum(*c) && *c != '_') *c = '_';
    }
    return sanitized;
}

char *uppercase_name(const char *name) {
    size_t len = strlen(name);
    char *upper = malloc(len + 1);
    if (!upper) return NULL;
    for (size_t i = 0; i < len; i++) upper[i] = toupper((unsigned char)name[i]);
    upper[len] = '\0';
    return upper;
}

const char* basename(const char *path) {
    if (!path || !*path) return "unknown";
    const char *base = strrchr(path, '/');
#ifdef _WIN32
    const char *win_base = strrchr(path, '\\');
    if (win_base && (!base || win_base > base)) base = win_base;
#endif
    return base ? base + 1 : path;
}

int main(int argc, char *argv[]) {
    char *input_file = NULL, *output_file = NULL, *function_name = NULL;
    int count = -1;
    bool verbose = false, struct_mode = false, text_mode = false, ms_sys_mode = false, test_mode = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0) {
            print_version(argv[0]);
            return EXIT_SUCCESS;
        } else if (strcmp(argv[i], "-c") == 0 && i + 1 < argc) {
            count = atoi(argv[++i]);
            if (count <= 0) {
                bdebug("Invalid count value");
                return EXIT_FAILURE;
            }
        } else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            function_name = argv[++i];
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            input_file = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_file = argv[++i];
        } else if (strcmp(argv[i], "-v") == 0) {
            verbose = true;
        } else if (strcmp(argv[i], "-s") == 0) {
            struct_mode = true;
        } else if (strcmp(argv[i], "-t") == 0) {
            text_mode = true;
        } else if (strcmp(argv[i], "-e") == 0) {
            test_mode = true;
        } else if (strcmp(argv[i], "--ms-sys") == 0 || strcmp(argv[i], "-m") == 0) {
            ms_sys_mode = true;
        } else {
            bdebug("Unknown option '%s'", argv[i]);
            return EXIT_FAILURE;
        }
    }

    #ifdef ENABLED_BUFFER_MAX
    if (count > BUFFER_MAX) {
        bdebug("Maximum Size is %d", BUFFER_MAX);
        return EXIT_FAILURE;
    }
    #endif

    if (!input_file) {
        bdebug("Missing required options");
        return EXIT_FAILURE;
    }

    if (!output_file) {
        output_file = "file.h";
    }

    if (!function_name) {
        function_name = "function";
    }

    char *sanitized = sanitize_name(function_name);
    char *upper = uppercase_name(sanitized);
    if (!sanitized || !upper) {
        bdebug("Memory allocation failed");
        if (sanitized) free(sanitized);
        if (upper) free(upper);
        return EXIT_FAILURE;
    }

    FILE *in = (strcmp(input_file, "-") == 0) ? stdin : fopen(input_file, "rb");
    if (!in) {
        bdebug("Opening input file '%s': %s", input_file, strerror(errno));
        if (sanitized) free(sanitized);
        if (upper) free(upper);
        return EXIT_FAILURE;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        bdebug("opening output file '%s': %s", output_file, strerror(errno));
        if (in != stdin) fclose(in);
        if (sanitized) free(sanitized);
        if (upper) free(upper);
        return EXIT_FAILURE;
    }

    size_t memory_bytes = 0;

    #ifdef ENABLED_BUFFER_MAX
    unsigned char buffer[BUFFER_MAX];
    #else
    unsigned char buffer[count];
    #endif

    size_t read_bytes;

    while ((read_bytes = fread(buffer, 1, (count > 0 ? (size_t)(count - memory_bytes) : sizeof(buffer)), in)) > 0) {
        memory_bytes += read_bytes;
    }

    fprintf(out,
    "/*\n"
    " * Generated with Bytes To Header\n"
    " * Version: %s\n"
    " * Input File: %s\n"
    " * Output File: %s\n"
    " * Bytes: %zu\n"
    " */\n\n",
    VERSION, basename(input_file), basename(output_file), memory_bytes);
    if (!ms_sys_mode) {
        fprintf(out, "#ifndef %s_H\n#define %s_H\n\n", upper, upper);
        fprintf(out, "#ifdef __cplusplus\nextern \"C\" {\n#endif\n\n");
        fprintf(out, "%s", text_mode ? "" : "#include <stdint.h>\n\n");
        fprintf(out, "#define %s_SIZE %zu\n\n", upper, memory_bytes);
    }

    rewind(in); // Reset file pointer for second pass.

    if (ms_sys_mode) {
        fprintf(out, "unsigned char %s[%zu] = {", sanitized, memory_bytes);
    } else if (struct_mode) {
        fprintf(out, "struct __attribute__((aligned(%s_SIZE))) %s_struct {\n  unsigned char data[%s_SIZE];\n};\n\n", upper, sanitized, upper);
        fprintf(out, "static const struct %s_struct %s = {\n  .data = {", sanitized, sanitized);
    } else {
        fprintf(out, "%s %s[%s_SIZE] = {", text_mode ? "static const uint8_t" : "static const unsigned char", sanitized, upper);
    }

    size_t total_bytes = 0;
    while ((read_bytes = fread(buffer, 1, (count > 0 ? (size_t)(count - total_bytes) : sizeof(buffer)), in)) > 0) {
        for (size_t i = 0; i < read_bytes; i++) {
            if (total_bytes > 0) fprintf(out, ", ");
            if (total_bytes % BYTES_PER_LINE == 0) fprintf(out, "\n%s", struct_mode ? "    " : "  ");
            fprintf(out, "0x%02x", buffer[i]);
            total_bytes++;
        }
    }

    if (ferror(in)) {
        bdebug("reading input file");
        fclose(in);
        fclose(out);
        free(sanitized);
        free(upper);
        return EXIT_FAILURE;
    }

    if (struct_mode) {
        fprintf(out, "\n  }\n};\n\n");
    } else {
        fprintf(out, "\n};\n\n");
    }

    if (!ms_sys_mode) {
        fprintf(out, "#ifdef __cplusplus\n}\n#endif\n\n#endif // %s_H\n\n", upper);
    }

    if (test_mode) {
        fprintf(out,
        " /*\n"
        " * Tests\n"
        " * Bytes: %zu\n"
        " */",
        total_bytes);
    }

    fclose(in);
    fclose(out);
    free(sanitized);
    free(upper);

    if (verbose) {
        vprint("Successfully written %zu bytes to %s", memory_bytes, output_file);
    }

    return EXIT_SUCCESS;
}
