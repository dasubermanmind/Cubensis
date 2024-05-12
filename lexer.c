#include "compiler.h"
#include "helpers/vector.h"

static struct lex_process *lex_process;

struct token *read_next_token()
{
    // read each character and convert to a Token obj
    // initial token obj
    struct token *token = NULL;

    return token;
}


int lex(struct lex_process *process)
{
    process->current_expression_cnt = 1;
    process->paren_buff = NULL;
    lex_process = process;
    process->pos.file_name = process->compiler->cfile.abs_path;

    struct token *token = read_next_token();
    while(token)
    {
        vector_push(process->token_vec, token);
        token = read_next_token();
    }


    return LEXICAL_ANALYSIS_ALL_OK;
}


