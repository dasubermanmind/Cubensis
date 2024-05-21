#include "compiler.h"
#include "helpers/vector.h"
#include "helpers/buffer.h"
#include <string.h>
#include <assert.n>

#define LEX_GETC_IF(buffer, c, exp)     \
    for (c = peekc(); exp; c = peekc()) \
    {                                   \
        buffer_write(buffer, c);        \
        nextc();                        \
    }

struct token *read_next_token();

static struct lex_process *lex_process;

static struct token temp_token;

static struct token *make_string_token(char start, char end)
{
    assert(nextc() == start);

    struct buffer *buffer = buffer_create();
    char ch = nextc();

    for(;ch !=end && ch!=EOF; ch =nextc())
    {
        // escape
        if(ch == '\\'){}
        buffer_write(buffer, ch);
    }
    buffer_write(buffer, 0x00);
    return token_create(&(struct token){.type=TOKEN_TYPE_STRING, .sval=buffer_ptr(buffer)});
}


static char peekc()
{
    return lex_process->functions->peek_char(lex_process);
}

static char nextc()
{
    char ch = lex_process->functions->next_char(lex_process);
    lex_process->pos.col += 1;
    if (ch == '\n')
    {
        lex_process->pos.line += 1;
        lex_process->pos.col = 1;
    }
    return ch;
}

static void pushc(char c)
{
    lex_process->functions->push_char(lex_process, c);
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

static struct token *lexer_last_token()
{
    return vector_back_or_null(lex_process->token_vec);
}

static struct token *handle_whitespace()
{
    struct token *last_token = lexer_last_token();
    if (last_token)
    {
        last_token->whitespace = true;
    }

    nextc();
    return read_next_token();
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

unsigned long long read_number()
{
    const char *s = read_number_str();
    return atoll(s);
}

struct token *token_make_number_for_value(unsigned long number)
{
    return token_create(&(struct token){.type = TOKEN_TYPE_NUMBER, .llnum = number});
}

struct token *token_make_number()
{
    return token_make_number_for_value(read_number());
}

// handles join operators
static bool op_treated_as_one(char operator)
{
    return operator == '(' || operator == '[' || operator == '.' || operator == ',' || operator == '*' || operator == '?';
}

const char *read_op()
{
    bool single_operator = true;
    char op = nextc();
    struct buffer *buffer = buffer_create();
    buffer_write(buffer, op);

    if (!op_treated_as_one(op))
    {
        op = peekc();
        if (is_single_operator(op))
        {
            buffer_write(buffer, op);
            nextc();
            single_operator = false;
        }
    }

    // NULL TERMINATOR
    buffer_write(buffer, 0x00);
    char *ptr = buffer_ptr(buffer);
    if (!single_operator)
    {
        if (!op_valid(ptr))
        {
            read_op_flush_back_keep_first(buffer);
            ptr[1] = 0x00;
        }
    }
    else if(!op_valid(ptr))
    {
        compiler_error(lex_process->compiler, "The operator %s is not valid\n", ptr);
    }
}

static bool is_single_operator(char op)
{
    return op == '+' ||
           op == '-' ||
           op == '/' ||
           op == '*' ||
           op == '=' ||
           op == '>' ||
           op == '<' ||
           op == '|' ||
           op == '&' ||
           op == '^' ||
           op == '%' ||
           op == '!' ||
           op == '(' ||
           op == '[' ||
           op == ',' ||
           op == '.' ||
           op == '~' ||
           op == '?';
}

bool op_valid(const char *op)
{
    return S_EQ(op, "+") ||
           S_EQ(op, "-") ||
           S_EQ(op, "*") ||
           S_EQ(op, "/") ||
           S_EQ(op, "!") ||
           S_EQ(op, "^") ||
           S_EQ(op, "+=") ||
           S_EQ(op, "-=") ||
           S_EQ(op, "*=") ||
           S_EQ(op, "/=") ||
           S_EQ(op, ">>") ||
           S_EQ(op, "<<") ||
           S_EQ(op, ">=") ||
           S_EQ(op, "<=") ||
           S_EQ(op, ">") ||
           S_EQ(op, "<") ||
           S_EQ(op, "||") ||
           S_EQ(op, "&&") ||
           S_EQ(op, "|") ||
           S_EQ(op, "&") ||
           S_EQ(op, "++") ||
           S_EQ(op, "--") ||
           S_EQ(op, "=") ||
           S_EQ(op, "!=") ||
           S_EQ(op, "==") ||
           S_EQ(op, "->") ||
           S_EQ(op, "(") ||
           S_EQ(op, "[") ||
           S_EQ(op, ",") ||
           S_EQ(op, ".") ||
           S_EQ(op, "...") ||
           S_EQ(op, "~") ||
           S_EQ(op, "?") ||
           S_EQ(op, "%");
}

void read_op_flush_back_keep_first(struct buffer* buffer)
{
    const char* data = buffer_ptr(buffer);
    int len = buffer->len;
    for (int i = len-1; i>= 1; i--)
    {
        if (data[i] == 0x00)
        {
            continue;
        }

        pushc(data[i]);
    }
}

// TODO: Implement next
static struct token *token_make_operator_or_string()
{
    return NULL;
}

struct token *read_next_token()
{
    struct token *token = NULL;
    char c = peekc();
    switch (c)
    {
    NUMERIC_CASE:
        token = token_make_number();
        break;
    OPERATOR_CASE_EXCLUDING_DIVISION:
        token = token_make_operator_or_string();
        break;
    // We don't care about whitespace ignore them
    case ' ':
    case '\t':
        token = handle_whitespace();
        break;
    case '"':
        token = make_string_token('"', '"');
    case EOF:
        // We have finished lexical analysis on the file
        break;

    default:
        compiler_error(lex_process->compiler, "Unexpected token\n");
    }
    return token;
}

int lex(struct lex_process *process)
{

    process->current_expression_cnt = 0;
    lex_process = process;
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
