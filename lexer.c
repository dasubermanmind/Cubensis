#include "compiler.h"
#include "helpers/vector.h"
#include "helpers/buffer.h"
#include <string.h>

#define LEX_GETC_IF(buffer, c, exp)      \
    for (c = peekc(); exp; c = peekc()) \
    {                                    \
        buffer_write(buffer, c);         \
        nextc();                         \
    }



static struct token temp_token;
static struct lex_process *lex_process;

static char peekc()
{
    return lex_process->functions->peek_char(lex_process);
}

static char nextc()
{
    char ch = lex_process->functions->peek_char(lex_process);
    lex_process->pos.col += 1;
    if (ch == '\n')
    {
        lex_process->pos.line += 1;
        lex_process->pos.col += 1;
    }
    return ch;
}

static void pushc(char c)
{
    lex_process->functions->push_char(lex_process, c);
}

const char *read_number_str()
{
    const char *num = NULL;
    struct buffer *buffer = buffer_create();
    char ch = peekc();
    LEX_GETC_IF(buffer, ch, (ch >= '0' && ch <= '9'));

    buffer_write(buffer, 0x00); // null term
    return buffer_ptr(buffer); 
}

static struct pos lex_file_position()
{
    return lex_process->pos;
}

struct token *token_create(struct token *_token)
{
    memcpy(&temp_token, _token, sizeof(struct token));
    temp_token.pos = lex_file_position();
    return &temp_token;
}


unsigned long long read_number()
{
    const char *s = read_number_str();
    return atoll(s);
}

struct token *token_make_number_for_value(unsigned long number)
{
    return token_create(&(struct token){.type=TOKEN_TYPE_NUMBER,.llnum=number});
}

struct token *token_make_number()
{
    unsigned long number = read_number();
    return token_make_number(number);
}

struct token *read_next_token()
{
    // read each character and convert to a Token obj
    // initial token obj
    struct token *token = NULL;
    char ch = peekc();
    switch (ch)
    {
        NUMERIC_CASE:
        token = token_make_number();
        break;

    case EOF:
        break;

    default:
        printf("Unsupported unicode");
        compiler_error(lex_process->compiler, "Unknown error");
    }

    return token;
}

int lex(struct lex_process *process)
{
    lex_process = process;
    process->current_expression_cnt = 1;
    process->paren_buff = NULL;

    process->pos.file_name = process->compiler->cfile.abs_path;
    // grabs the first token
    struct token *token = read_next_token();
    while (token)
    {
        vector_push(process->token_vec, token);
        // enumerate through all of them.
        token = read_next_token();
    }

    return LEXICAL_ANALYSIS_ALL_OK;
}
