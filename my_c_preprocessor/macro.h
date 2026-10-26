#ifndef MACRO_H
#define MACRO_H

#include <stddef.h>

#define MAX_MACROS 100
#define MACRO_NAME_SIZE 64
#define MACRO_VALUE_SIZE 256

struct MacroTable;

struct MacroTable *macro_table_create(void);
void macro_table_destroy(struct MacroTable *table);
int macro_define(struct MacroTable *table, const char *name, const char *value);
void macro_expand_line(const struct MacroTable *table, const char *input,
                       char *output, size_t output_size);

#endif
