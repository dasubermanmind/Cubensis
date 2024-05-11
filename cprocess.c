#include <stdio.h>

struct compile_process *compile_process_create(const char *file_name, const char *outfile, int flags)
{
    FILE *file = fopen(file_name, "r");

    if(!file)
    {
        return NULL;
    }
    // Optional argument
    if(outfile)
    {
        FILE *file_out = fopen(outfile, "w");
        if(!file_out)
        {
            return NULL;
        }
    }
}

