#include "macro.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Macro {
    char name[MACRO_NAME_SIZE];
    char value[MACRO_VALUE_SIZE];
};

struct MacroTable {
    struct Macro items[MAX_MACROS];
    size_t count;
};

struct MacroTable *macro_table_create(void)
{
    struct MacroTable *table = calloc(1, sizeof(*table));
    return table;
}

void macro_table_destroy(struct MacroTable *table)
{
    free(table);
}

static int valid_identifier(const char *name)
{
    size_t i;

    if (name == NULL || name[0] == '\0')
        return 0;

    if (!(isalpha((unsigned char)name[0]) || name[0] == '_'))
        return 0;

    for (i = 1; name[i] != '\0'; ++i)
    {
        if (!(isalnum((unsigned char)name[i]) || name[i] == '_'))
            return 0;
    }
    return 1;
}

int macro_define(struct MacroTable *table, const char *name, const char *value)
{
    size_t i;

    if (table == NULL || !valid_identifier(name) || value == NULL)
        return 0;

    for (i = 0; i < table->count; ++i)
    {
        if (strcmp(table->items[i].name, name) == 0)
        {
            snprintf(table->items[i].value, MACRO_VALUE_SIZE, "%s", value);
            return 1;
        }
    }

    if (table->count >= MAX_MACROS)
        return 0;

    snprintf(table->items[table->count].name, MACRO_NAME_SIZE, "%s", name);
    snprintf(table->items[table->count].value, MACRO_VALUE_SIZE, "%s", value);
    table->count++;
    return 1;
}

static const char *find_macro(const struct MacroTable *table,
                              const char *word, size_t length)
{
    size_t i;

    for (i = 0; i < table->count; ++i)
    {
        if (strlen(table->items[i].name) == length &&
            strncmp(table->items[i].name, word, length) == 0)
            return table->items[i].value;
    }
    return NULL;
}

static void append_text(char *out, size_t out_size, size_t *used,
                        const char *text)
{
    while (*text != '\0' && *used + 1 < out_size)
        out[(*used)++] = *text++;
    if (out_size > 0)
        out[*used] = '\0';
}

void macro_expand_line(const struct MacroTable *table, const char *input,
                       char *output, size_t output_size)
{
    enum State { CODE, STRING, CHARACTER } state = CODE;
    size_t i = 0;
    size_t used = 0;

    if (output_size == 0)
        return;
    output[0] = '\0';

    while (input[i] != '\0' && used + 1 < output_size)
    {
        char ch = input[i];

        if (state == STRING || state == CHARACTER)
        {
            output[used++] = ch;
            if (ch == '\\' && input[i + 1] != '\0' && used + 1 < output_size)
            {
                output[used++] = input[i + 1];
                i += 2;
                continue;
            }
            if ((state == STRING && ch == '"') ||
                (state == CHARACTER && ch == '\''))
                state = CODE;
            ++i;
            continue;
        }

        if (ch == '"')
        {
            output[used++] = ch;
            state = STRING;
            ++i;
        }
        else if (ch == '\'')
        {
            output[used++] = ch;
            state = CHARACTER;
            ++i;
        }
        else if (isalpha((unsigned char)ch) || ch == '_')
        {
            size_t start = i;
            const char *replacement;

            do
                ++i;
            while (isalnum((unsigned char)input[i]) || input[i] == '_');

            replacement = find_macro(table, input + start, i - start);
            if (replacement != NULL)
                append_text(output, output_size, &used, replacement);
            else
            {
                size_t k;
                for (k = start; k < i && used + 1 < output_size; ++k)
                    output[used++] = input[k];
            }
        }
        else
        {
            output[used++] = ch;
            ++i;
        }
    }

    output[used] = '\0';
}
