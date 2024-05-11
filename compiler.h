#ifndef CUBENSIS_H
#define CUBENSIS_H
// keeps all function/class definitions
#include <stdio.h>

enum
{
    COMPILER_FILE_OK, 
    COMPILER_FILE_FAILED
};

struct compile_process
{
    // how to compile file
    int flags;

    struct compile_process_input_file
    {
        FILE *fp;
        const char *abs_path;
    }cfile;

    FILE *output;
};

struct compile_process *compile_process_create(const char *file_name, const char *outfile, int flags);

int compile_file(const char *file_name, const char *out_file, int flags);

#endif