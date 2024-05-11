#ifndef CUBENSIS_H
#define CUBENSIS_H
// keeps all function/class definitions
#include <stdio.h>
#include <stdbool.h>

struct pos
{
    int line;
    int col;

    const char *file_name;
};

// enums get set to token->type
enum
{
    TOKE_TYPE_IDENTIFIER,
    TOKE_TYPE_KEYWORD,
    TOKE_TYPE_OPERATOR,
    TOKE_TYPE_SYMBOL,
    TOKE_TYPE_NUMBER,
    TOKE_TYPE_STRING,
    TOKE_TYPE_COMMENT,
    TOKE_TYPE_NEWLINE
};

struct token
{
    int type;
    int flags;
    // share mem
    union
    {
        char cval;
        const char *sval;
        unsigned int inum;
        unsigned long lnum;
        unsigned long long llnum;
        void *any;
    };

    bool whitespace;

    const char *between_brackets;
};

struct lex_process;
typedef char (*LEX_PROCESS_NEXT_CHAR)(struct lex_process *process);
typedef char (*LEX_PROCESS_PEEK_CHAR)(struct lex_process *process);
typedef void (*LEX_PROCESS_PUSH_CHAR)(struct lex_process *process, char c);
struct lex_process_fns
{
    LEX_PROCESS_NEXT_CHAR next_char;
    LEX_PROCESS_PEEK_CHAR peek_char;
    LEX_PROCESS_PUSH_CHAR push_char;
};

struct lex_process
{
    struct pos pos;
    struct vector *toke_vec;
    struct compile_process *compiler;

    int current_expression_cnt;
    struct buffer *paren_buff;
    struct lex_process_fns *functions;
    void *private;
};

enum
{
    COMPILER_FILE_OK,
    COMPILER_FILE_FAILED
};

struct compile_process
{
    // how to compile file
    int flags;
    struct pos pos;
    struct compile_process_input_file
    {
        FILE *fp;
        const char *abs_path;
    } cfile;

    FILE *output;
};

struct compile_process *compile_process_create(const char *file_name, const char *outfile, int flags);

int compile_file(const char *file_name, const char *out_file, int flags);

char compile_process_next_char(struct lex_process *lex_process);

char compile_process_peek_char(struct lex_process *lex_process);

char compile_process_push_char(struct lex_process *lex_process, char ch);

void lex_process_free(struct lex_process *process);

void *lex_process_private(struct lex_process *process);

struct vector *lex_process_tokens(struct lex_process *process);

#endif