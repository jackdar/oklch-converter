#include "colour_convert.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

void print_usage(const char *program_name) {
    printf("Usage: %s <hex_colour>\n", program_name);
    printf("Example: %s \"#ff5500\"\n", program_name);
    printf("Example: %s \"ff5500\"\n", program_name);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        print_usage(argv[0]);
        return 1;
    }

    const char *hex = argv[1];

    // Validate hex
    size_t offset = (hex[0] == '#') ? 1 : 0;
    size_t len = strlen(hex) - offset;

    if (len != 3 && len != 6) {
        fprintf(stderr, "Error: Invalid hex colour format. Must be 3 or 6 hex "
                        "digits with optional # prefix.\n");
        return 1;
    }

    for (size_t i = offset; i < strlen(hex); i++) {
        if (!isxdigit(hex[i])) {
            fprintf(stderr, "Error: Invalid hex digit: %c\n", hex[i]);
            return 1;
        }
    }

    OKLCH oklch = hex_to_oklch(hex);

    printf("Hex: %s\n", hex);
    printf("OKLCH: %.6f %.6f %.6f\n", oklch.l, oklch.c, oklch.h);
    printf("CSS format: oklch(%.2f%% %.4f %.1f)\n", oklch.l * 100.0, oklch.c,
           oklch.h);

    return 0;
}
