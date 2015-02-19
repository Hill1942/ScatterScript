#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ssutil.h"

extern FILE*  g_SourceFile;
extern char*  g_SourceFileName;
extern int    g_SourceCodeLines;
extern char** g_SourceCode;
char* strtoupper(char* str)
{
    char* p = str;

    *p = toupper(*p);
    while (*p)
        p++;

    return str;
}

void ExitOnError(char* errorMessage)
{
    printf("Fatal Error: %s.\n", errorMessage);
    Exit();
}

void Exit()
{
    ShutDown();
    exit(0);
}

void ShutDown()
{
    for (int i = 0; i < g_SourceCodeLines; i++)
    {
        free(g_SourceCode[i]);
    }
    free(g_SourceCode);
}