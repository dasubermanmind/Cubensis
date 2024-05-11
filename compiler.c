
#include "compiler.h"


int compile_file(const char *file_name, const char *out_file, int flags)
{
    struct compile_process *process = compile_process(file_name, out_file, flags);
    if(!process)
    {
        return COMPILER_FILE_FAILED;
    }
    
    // TODO:
    // Lex Analysis

    // Parse

    // Code Gen
    return COMPILER_FILE_OK;
}


