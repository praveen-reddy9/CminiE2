#include "preprocessor.h"

#include "comment.h"
#include "macro.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

static char *trim_left(char *text)
{
    while (isspace((unsigned char)*text))
        ++text;
    return text;
}

static int parse_define(const char *line, char *name, size_t name_size,
                        char *value, size_t value_size)
{
    const char *p = line;
    size_t n = 0;

    while (isspace((unsigned char)*p))
        ++p;
    if (*p != '#')
        return 0;
    ++p;

    while (isspace((unsigned char)*p))
        ++p;
    if (strncmp(p, "define", 6) != 0 ||
        (isalnum((unsigned char)p[6]) || p[6] == '_'))
        return 0;
    p += 6;

    while (isspace((unsigned char)*p))
        ++p;
    if (!(isalpha((unsigned char)*p) || *p == '_'))
        return 0;

    while ((isalnum((unsigned char)*p) || *p == '_') && n + 1 < name_size)
        name[n++] = *p++;
    name[n] = '\0';

    /* Function-like macros are intentionally outside this project's scope. */
    if (*p == '(')
        return 0;

    while (*p == ' ' || *p == '\t' || *p == '\r')
        ++p;

    snprintf(value, value_size, "%s", p);

    /* Remove the newline that fgets() kept. */
    value[strcspn(value, "\r\n")] = '\0';
    return 1;
}

int process_file(const char *input_name, const char *output_name)
{
    FILE *input = fopen(input_name, "r");
    FILE *output;
    struct MacroTable *macros;
    char raw[LINE_BUFFER_SIZE];
    char without_comments[WORK_BUFFER_SIZE];
    char expanded[WORK_BUFFER_SIZE];
    int status = 0;
    int in_block_comment = 0;

    if (input == NULL)
    {
        perror(input_name);
        return 1;
    }

    output = fopen(output_name, "w");
    if (output == NULL)
    {
        perror(output_name);
        fclose(input);
        return 1;
    }

    macros = macro_table_create();
    if (macros == NULL)
    {
        fprintf(stderr, "Could not allocate macro table.\n");
        fclose(input);
        fclose(output);
        return 1;
    }

    while (fgets(raw, sizeof(raw), input) != NULL)
    {
        char *line;
        char macro_name[MACRO_NAME_SIZE];
        char macro_value[MACRO_VALUE_SIZE];

        strip_comments(raw, without_comments, sizeof(without_comments), &in_block_comment);
        line = trim_left(without_comments);

        if (parse_define(line, macro_name, sizeof(macro_name),
                         macro_value, sizeof(macro_value)))
        {
            if (!macro_define(macros, macro_name, macro_value))
            {
                fprintf(stderr, "Warning: could not define macro %s\n", macro_name);
                status = 1;
            }
            continue;
        }

        macro_expand_line(macros, without_comments, expanded, sizeof(expanded));
        fputs(expanded, output);
    }

    macro_table_destroy(macros);
    fclose(input);
    fclose(output);
    return status;
}
