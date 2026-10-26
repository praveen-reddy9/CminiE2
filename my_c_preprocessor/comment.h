#ifndef COMMENT_H
#define COMMENT_H

#include <stddef.h>

/* Remove C comments while preserving string/character literals and newlines. */
void strip_comments(const char *input, char *output, size_t output_size, int *in_block_comment);

#endif
