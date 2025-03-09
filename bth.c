/* bth.c - Bytes To Header Utility
 *
 * Copyright (C) 2025 MrR736 <https://github.com/MrR736>
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

#define VERSION "1.0"

void print_usage(const char *progname) {
    printf("Usage: %s [OPTION]...\n\n", progname);
    printf("<OPTION>\n");
    printf("  --help, -h     display this help and exit\n");
    printf("  --version, -V  display version information and exit\n");
    printf("  -c <n>         Count BYTES\n");
    printf("  -f <name>      Name function\n");
    printf("  -i <file>      Input File\n");
    printf("  -o <file>      Output File\n\n");
    printf("HomePage: <https://github.com/MrR736>\n");
}

void print_version(const char *progname) {
    printf("%s (Bytes To Header) %s\n\nCopyright (C) 2022 Free Software Foundation, Inc.\nLicense GPLv3+: GNU GPL version 3 or later <https://gnu.org/licenses/gpl.html>.\nThis is free software: you are free to change and redistribute it.\nThere is NO WARRANTY, to the extent permitted by law.\n\nWritten by MrR736\n\nThis bth program was built with SELinux support.\nSELinux is disabled on this system.\n\nHomePage: <https://github.com/MrR736>.\n", progname, VERSION);
}

void sanitize_function_name(char *name) {
    if (!name || !*name) {
        fprintf(stderr, "Error: Function name cannot be empty.\n");
        exit(1);
    }

    if (!((*name >= 'a' && *name <= 'z') || (*name >= 'A' && *name <= 'Z') || *name == '_')) {
        *name = '_';  // Ensure first character is valid
    }

    for (char *c = name + 1; *c; c++) {
        if (!((*c >= 'a' && *c <= 'z') || (*c >= 'A' && *c <= 'Z') || (*c >= '0' && *c <= '9') || *c == '_')) {
            *c = '_';
        }
    }
}

int main(int argc, char *argv[]) {
    char *input_file = NULL, *output_file = NULL, *function_name = NULL;
    int count = -1;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-V") == 0) {
            print_version(argv[0]);
            exit(0);
        } else if (strcmp(argv[i], "-c") == 0) {
            if (i + 1 < argc) {
                count = atoi(argv[++i]);
                if (count <= 0) {
                    fprintf(stderr, "Error: Count must be a positive integer.\n");
                    exit(1);
                }
            } else {
                fprintf(stderr, "Error: Missing value for -c <n>\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                function_name = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing value for -f <name>\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) {
                input_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing value for -i <file>\n");
                exit(1);
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Error: Missing value for -o <file>\n");
                exit(1);
            }
        } else {
            fprintf(stderr, "Error: Unknown option '%s'\n", argv[i]);
            exit(1);
        }
    }

    if (!input_file || !output_file || !function_name || count <= 0) {
        fprintf(stderr, "%s: invalid option -- 'l'\n", argv[0]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return 1;
    }

    sanitize_function_name(function_name);

    FILE *in = fopen(input_file, "rb");
    if (!in) {
        perror("Error opening input file");
        return 1;
    }

    FILE *out = fopen(output_file, "w");
    if (!out) {
        perror("Error opening output file");
        fclose(in);
        return 1;
    }

    fprintf(out, "/* Generated with Bytes To Header */\n\n");
    fprintf(out, "unsigned char %s[] = {\n  ", function_name);

    unsigned char buffer;
    int bytes_read = 0;

    while (bytes_read < count && fread(&buffer, 1, 1, in) == 1) {
        if (bytes_read > 0) {
            fprintf(out, ", ");
            if (bytes_read % 12 == 0) {
                fprintf(out, "\n  ");
            }
        }
        fprintf(out, "0x%02x", buffer);
        bytes_read++;
    }

    fprintf(out, "\n};\n");

    fclose(in);
    fclose(out);

    printf("Successfully written %d bytes to %s\n", bytes_read, output_file); // load bar
    return 0;
}
