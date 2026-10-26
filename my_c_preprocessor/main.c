#include "preprocessor.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char output_name[1024];

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s input.c\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* The output name follows the input name: source.c -> source.i. */
    {
        const char *input = argv[1];
        const char *dot = strrchr(input, '.');
        size_t prefix = (dot != NULL) ? (size_t)(dot - input) : strlen(input);

        if (prefix + 3 >= sizeof(output_name))
        {
            fprintf(stderr, "Input path is too long.\n");
            return EXIT_FAILURE;
        }
        snprintf(output_name, sizeof(output_name), "%.*s.i", (int)prefix, input);
    }

    if (process_file(argv[1], output_name) != 0)
        return EXIT_FAILURE;

    printf("Preprocessing complete: %s\n", output_name);
    return EXIT_SUCCESS;
}
