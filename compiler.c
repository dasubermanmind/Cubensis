
#include "compiler.h"

struct lex_process_fns compiler_lex_fns =
    {
        .next_char = compile_process_next_char,
        .peek_char = compile_process_peek_char,
        .push_char = compile_process_push_char};

int compile_file(const char *file_name, const char *out_file, int flags)
{
    struct compile_process *process = compile_process(file_name, out_file, flags);
    if (!process)
    {
        return COMPILER_FILE_FAILED;
    }

    // TODO:
    // Lex Analysis
    struct lex_process *lex_process = lex_process_create(process, &compiler_lex_fns, NULL);
    // Parse

    // Code Gen
    return COMPILER_FILE_OK;
}
