#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>

#define LINE_BUFFER_SIZE 4096
#define WORK_BUFFER_SIZE 8192

int process_file(const char *input_name, const char *output_name);

#endif
