#include "compiler.h"
#include <stdarg.h>
#include <stdlib.h>

struct lex_process_functions compiler_lex_functions =
    {
        .next_char = compile_process_next_char,
        .peek_char = compile_process_peek_char,
        .push_char = compile_process_push_char};

int compile_file(const char *file_name, const char *out_file, int flags)
{
    struct compile_process *process = compile_process_create(file_name, out_file, flags);
    printf("Process returns");
    printf("%c", process);
    if (!process)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }

    // TODO:
    // Lex Analysis
    struct lex_process *lex_process = lex_process_create(process, &compiler_lex_functions, NULL);
    if (!lex_process)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }
    if (lex(lex_process) != LEXICAL_ANALYSIS_ALL_OK)
    {
        return COMPILER_FAILED_WITH_ERRORS;
    }
    // Parse
    // Optimize    
    // Code Gen
    return COMPILER_FILE_OK;
}

void compiler_error(struct compile_process *compiler, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "on line %i, on col %i, in file %i % s\n", compiler->pos.line, compiler->pos.col, compiler->pos.file_name);
    exit(-1);
}

void compiler_warning(struct compile_process *compiler, const char *msg, ...)
{
    va_list args;
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "on line %i, on col %i, in file %i % s\n", compiler->pos.line, compiler->pos.col, compiler->pos.file_name);
}
