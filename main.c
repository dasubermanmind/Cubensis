#include <stdio.h> 
#include "helpers/vector.h"
#include "compiler.h"

int main()
{
    int result = compile_file("./test.c", "./test", 0);
    if(result == COMPILER_FILE_OK)
    {
        printf("Compiled Everything without errors");
    }
    else if(result == COMPILER_FAILED_WITH_ERRORS)
    {
        printf("Compiled Failed with errors");
    }
    else
    {
        printf("Compiled Failed with Unknown errors");
    }

    return 0;
}



