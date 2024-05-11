#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

struct compile_process *compile_process_create(const char *file_name, const char *outfile, int flags)
{
    FILE *file = fopen(file_name, "r");

    if (!file)
    {
        return NULL;
    }

    FILE *file_out = NULL;
    // Optional argument
    if (outfile)
    {
        file_out = fopen(outfile, "w");
        if (!file_out)
        {
            return NULL;
        }
    }
    // allocates the requested memory and returns a pointer to it
    struct compile_process* process = calloc(1, sizeof(struct compile_process));
    process->flags = flags;
    process->cfile.fp = file;
    process->output = outfile;
    return process;
}
