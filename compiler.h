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