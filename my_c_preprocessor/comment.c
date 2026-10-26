#include "comment.h"

#include <stddef.h>

void strip_comments(const char *input, char *output, size_t output_size, int *in_block_comment)
{
    enum State { CODE, STRING, CHARACTER, LINE_COMMENT, BLOCK_COMMENT };
    enum State state = (in_block_comment != NULL && *in_block_comment) ? BLOCK_COMMENT : CODE;
    size_t i = 0;
    size_t j = 0;

    if (output_size == 0)
        return;

    while (input[i] != '\0' && j + 1 < output_size)
    {
        char ch = input[i];
        char next = input[i + 1];

        if (state == CODE)
        {
            if (ch == '"')
            {
                output[j++] = ch;
                state = STRING;
                i++;
            }
            else if (ch == '\'')
            {
                output[j++] = ch;
                state = CHARACTER;
                i++;
            }
            else if (ch == '/' && next == '/')
            {
                /* Keep the newline, but discard the comment text. */
                state = LINE_COMMENT;
                i += 2;
            }
            else if (ch == '/' && next == '*')
            {
                /* A space prevents tokens on either side joining together. */
                if (j > 0 && output[j - 1] != ' ' && output[j - 1] != '\n')
                    output[j++] = ' ';
                state = BLOCK_COMMENT;
                i += 2;
            }
            else
            {
                output[j++] = ch;
                i++;
            }
        }
        else if (state == STRING)
        {
            output[j++] = ch;
            if (ch == '\\' && next != '\0')
            {
                if (j + 1 >= output_size)
                    break;
                output[j++] = next;
                i += 2;
            }
            else
            {
                if (ch == '"')
                    state = CODE;
                i++;
            }
        }
        else if (state == CHARACTER)
        {
            output[j++] = ch;
            if (ch == '\\' && next != '\0')
            {
                if (j + 1 >= output_size)
                    break;
                output[j++] = next;
                i += 2;
            }
            else
            {
                if (ch == '\'')
                    state = CODE;
                i++;
            }
        }
        else if (state == LINE_COMMENT)
        {
            if (ch == '\n')
            {
                output[j++] = '\n';
                state = CODE;
            }
            i++;
        }
        else /* BLOCK_COMMENT */
        {
            if (ch == '*' && next == '/')
            {
                state = CODE;
                i += 2;
            }
            else
            {
                /* Preserve newlines so line structure remains intact. */
                if (ch == '\n')
                    output[j++] = '\n';
                i++;
            }
        }
    }

    output[j] = '\0';
    if (in_block_comment != NULL)
        *in_block_comment = (state == BLOCK_COMMENT);
}
