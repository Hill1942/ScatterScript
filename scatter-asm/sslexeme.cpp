#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "ssutil.h""
#include "ssbase_type.h"
#include "sstype.h"
#include "sslexeme.h"


extern FILE*  g_SourceFile;
extern char*  g_SourceFileName;
extern int    g_SourceCodeLines;
extern char** g_SourceCode;

extern Lexer  g_Lexer;

int IsCharNumeric(char cChar)
{
    if (cChar >= '0' && cChar <= '9')
        return TRUE;
    else
        return FALSE;
}
int IsCharWhiteSpace(char cChar)
{
    if (cChar == ' ' || cChar == '\t')
        return TRUE;
    else
        return FALSE;
}
int IsCharIdent(char cChar)
{
    if ((cChar >= '0' && cChar <= '9') ||
            (cChar >= 'A' && cChar <= 'Z') ||
            (cChar >= 'a' && cChar <= 'z') ||
            cChar >= '_')
        return TRUE;
    else
        return FALSE;
}
int IsCharDelimiter(char cChar)
{
    if (cChar == ':' || cChar == ',' || cChar == '"' ||
            cChar == '[' || cChar == ']' ||
            cChar == '{' || cChar == '}' ||
            IsCharWhiteSpace(cChar))
        return TRUE;
    else
        return FALSE;
}

int IsStringInt(const char* str)
{
    if (!str)
        return FALSE;

    if (strlen(str) == 0)
        return TRUE;

    for (int i = 0; i < strlen(str); i++)
    {
        if (!IsCharNumeric(str[i]) && str[i] != '-')
            return FALSE;
    }

    for (int i = 1; i < strlen(str); i++)
    {
        if (str[i] == '-')
            return FALSE;
    }
}
int IsStringFloat(const char* str)
{
    if (!str)
        return FALSE;

    if (strlen(str) == 0)
        return FALSE;

    for (int i = 0; i < strlen(str); i++)
    {
        if (!IsCharNumeric(str[i]) && str[i] != '-' && str[i] != '.')
            return FALSE;
    }

    int isRadixPointFound = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '.')
        {
            if (isRadixPointFound)
                return FALSE;
            else
                isRadixPointFound = 1;
        }
    }

    for (int i = 1; i < strlen(str); i++)
    {
        if (str[i] == '-')
            return FALSE;
    }

    if (isRadixPointFound)
        return TRUE;
    else
        return FALSE;
}
int IsStringWhitespace(const char* str)
{
    if (!str)
        return FALSE;

    if (strlen(str) == 0)
        return TRUE;

    for (int i = 0; i < strlen(str); i++)
    {
        if (!IsCharWhiteSpace(str[i]) && str[i] != '\n')
            return FALSE;
    }
    return TRUE;
}
int IsStringIdent(const char* str)
{
    if (!str)
        return FALSE;

    if (strlen(str) == 0)
        return FALSE;

    if (str[0] >= '0' && str[9] <= '9')
        return FALSE;

    for (int i = 0; i < strlen(str); i++)
    {
        if (!IsCharIdent(str[i]))
            return FALSE;
    }

    return TRUE;
}

void StripComments(char* sourceLine)
{
    int isInString = 0;
    for (int i = 0; i < strlen(sourceLine); i++)
    {
        if (sourceLine[i] == '"')
        {
            if (isInString)
                isInString = 0;
            else
                isInString = 1;
        }
        if (sourceLine[i] == ';')
        {
            if (!isInString)
            {
                sourceLine[i]     = '\n';
                sourceLine[i + 1] = '\0';
                break;
            }
        }
    }
}
void TrimWhiteSpace(char* sourceLine)
{
    int strLength    = strlen(sourceLine);
    int currentIndex = 0;
    int padIndex     = 0;

    if (strLength > 1)
    {
        for (int currentIndex = 0; currentIndex < strLength; currentIndex++)
        {
            if (!IsCharWhiteSpace(sourceLine[currentIndex]))
                break;
        }

        padIndex = currentIndex;
        if (padIndex)
        {
            for (currentIndex = padIndex; currentIndex < strLength; currentIndex++)
            {
                sourceLine[currentIndex - padIndex] = sourceLine[currentIndex];
            }
            for (currentIndex = strLength - padIndex; currentIndex < strLength; currentIndex++)
            {
                sourceLine[currentIndex] = ' ';
            }
        }

        for (currentIndex = strLength - 1; currentIndex > 0; currentIndex--)
        {
            if (!IsCharWhiteSpace(sourceLine[currentIndex]))
            {
                sourceLine[currentIndex + 1] = '\n';
                break;
            }
        }
    }
}

void LoadSourceFile()
{
    if (! (g_SourceFile = fopen(g_SourceFileName, "rb")))
        ExitOnError("Could not open source file");

    while (!feof(g_SourceFile))
    {
        if (fgetc(g_SourceFile) == '\n')
            g_SourceCodeLines++;
    }
    g_SourceCodeLines++;

    rewind(g_SourceFile);

    if (! (g_SourceCode = (char**)malloc(g_SourceCodeLines * sizeof(char*))))
        ExitOnError("Could not allocate space for source code");

    for (int i = 0; i < g_SourceCodeLines; i++)
    {
        if (! (g_SourceCode[i] = (char*)malloc(MAX_SOURCE_LINE_SIZE + 1)))
            ExitOnError("Could not allocate space for source code");

        fgets(g_SourceCode[i], MAX_SOURCE_LINE_SIZE, g_SourceFile);

        StripComments(g_SourceCode[i]);
        TrimWhiteSpace(g_SourceCode[i]);

        // Make sure to add a new newline if it was removed by the stripping of the
        // comments and whitespace. We do this by checking the character right before
        // the null terminator to see if it's \n. If not, we move the terminator over
        // by one and add it. We use strlen () to find the position of the newline
        // easily.

        int newLineIdex = strlen(g_SourceCode[i]) - 1;
        if (g_SourceCode[i][newLineIdex] != '\n' )
        {
            g_SourceCode[i][newLineIdex + 1] = '\n';
            g_SourceCode[i][newLineIdex + 2] = '\0';
        }
    }
}

Token GetNextToken()
{
    //When we read the source code line by line, we use two index to indicate the both ends of a source line.
    //At first, the right end index equals to the left end index: they both are 0.
    g_Lexer.iIndex0 = g_Lexer.iIndex1;
    if (g_Lexer.iIndex0 >= strlen(g_SourceCode[g_Lexer.iCurrentSourceLine]))
    {
        if (!SkipToNextLine())
            return END_OF_TOKEN_STREAM;
    }

    if (g_Lexer.iState == LEX_STATE_END_STRING)
        g_Lexer.iState = LEX_STATE_NO_STRING;
    //locate index to the first non-whitespace character of a source code line
    if (g_Lexer.iState != LEX_STATE_IN_STRING)
    {
        while (TRUE)
        {
            if (!IsCharWhiteSpace((g_SourceCode[g_Lexer.iCurrentSourceLine][g_Lexer.iIndex0])))
                break;
            g_Lexer.iIndex0++;
        }
    }
    g_Lexer.iIndex1 = g_Lexer.iIndex0;

    //move iIndex1 to the location of next token end
    while (TRUE)
    {
        if (g_Lexer.iState == LEX_STATE_IN_STRING)
        {
            if (g_Lexer.iIndex1 >= strlen(g_SourceCode[g_Lexer.iCurrentSourceLine]))
            {
                g_Lexer.currentToken = TOKEN_TYPE_INVALID;
                return g_Lexer.currentToken;
            }
            if (g_SourceCode[g_Lexer.iCurrentSourceLine][g_Lexer.iIndex1] == '\\')
            {
                g_Lexer.iIndex1 += 2;
                continue;
            }
            if (g_SourceCode[g_Lexer.iCurrentSourceLine][g_Lexer.iIndex1] == '"')
                break;
            g_Lexer.iIndex1++;
        }
        else
        {
            if (g_Lexer.iIndex1 >= strlen(g_SourceCode[g_Lexer.iCurrentSourceLine]))
                break;
            if (IsCharDelimiter(g_SourceCode[g_Lexer.iCurrentSourceLine][g_Lexer.iIndex1]))
                break;
            g_Lexer.iIndex1++;
        }
    }

    if (g_Lexer.iIndex1 - g_Lexer.iIndex0 == 0)
        g_Lexer.iIndex1++;

    //Get the next lexeme
    unsigned int currentTargetIndex = 0;
    for (int i = g_Lexer.iIndex0; i < g_Lexer.iIndex1; i++)
    {
        if (g_Lexer.iState == LEX_STATE_IN_STRING)
            if (g_SourceCode[g_Lexer.iCurrentSourceLine][i] == '\\')
                i++;
        g_Lexer.pCurrentLexeme[currentTargetIndex] = g_SourceCode[g_Lexer.iCurrentSourceLine][i];
        currentTargetIndex++;
    }
    g_Lexer.pCurrentLexeme[currentTargetIndex] = '\0';
    if (g_Lexer.iState != LEX_STATE_IN_STRING)
        strtoupper(g_Lexer.pCurrentLexeme);

    //Decide which token the lexeme is
    g_Lexer.currentToken = TOKEN_TYPE_INVALID;
    if (strlen(g_Lexer.pCurrentLexeme) > 1 || g_Lexer.pCurrentLexeme[0] != '"')
    {
        if (g_Lexer.iState == LEX_STATE_IN_STRING)
        {
            g_Lexer.currentToken = TOKEN_TYPE_STRING;
            return TOKEN_TYPE_STRING;
        }
    }

    if (strlen(g_Lexer.pCurrentLexeme) == 1)
    {
        switch (g_Lexer.pCurrentLexeme[0])
        {
            case '"':
                switch (g_Lexer.iState)
                {
                    case LEX_STATE_NO_STRING:
                        g_Lexer.iState = LEX_STATE_IN_STRING;
                        break;
                    case LEX_STATE_IN_STRING:
                        g_Lexer.iState = LEX_STATE_END_STRING;
                }
                g_Lexer.currentToken = TOKEN_TYPE_QUATE;
                break;
            case ',':
                g_Lexer.currentToken = TOKEN_TYPE_COMMA;
                break;
            case ':':
                g_Lexer.currentToken = TOKEN_TYPE_COLON;
                break;
            case '[':
                g_Lexer.currentToken = TOKEN_TYPE_OPEN_BRACKET;
                break;
            case ']':
                g_Lexer.currentToken = TOKEN_TYPE_CLOSE_BRACKET;
                break;
            case '{':
                g_Lexer.currentToken = TOKEN_TYPE_OPEN_BRACE;
                break;
            case '}':
                g_Lexer.currentToken = TOKEN_TYPE_CLOSE_BRACE;
                break;
            case '\n':
                g_Lexer.currentToken = TOKEN_TYPE_NEWLINE;
                break;
        }
    }

    if (IsStringInt(g_Lexer.pCurrentLexeme))
        g_Lexer.currentToken = TOKEN_TYPE_INT;

    if (IsStringFloat(g_Lexer.pCurrentLexeme))
        g_Lexer.currentToken = TOKEN_TYPE_FLOAT;

    if (IsStringIdent(g_Lexer.pCurrentLexeme))
        g_Lexer.currentToken = TOKEN_TYPE_IDENT;

    if (strcmp(g_Lexer.pCurrentLexeme, "SETSTACKSIZE") == 0)
        g_Lexer.currentToken = TOKEN_TYPE_SETSTACKSIZE;

    if (strcmp(g_Lexer.pCurrentLexeme, "VAR") == 0)
        g_Lexer.currentToken = TOKEN_TYPE_VAR;

    if (strcmp(g_Lexer.pCurrentLexeme, "FUNC") == 0)
        g_Lexer.currentToken = TOKEN_TYPE_FUNC;

    if (strcmp(g_Lexer.pCurrentLexeme, "PARAM") == 0)
        g_Lexer.currentToken = TOKEN_TYPE_PARAM;

    if (strcmp(g_Lexer.pCurrentLexeme, "_RETVAL") == 0)
        g_Lexer.currentToken = TOKEN_TYPE_REG_RETVAL;

    InstrLookup instrLookup;
    if (GetInstrByMnemonic(g_Lexer.pCurrentLexeme, &instrLookup))
        g_Lexer.currentToken = TOKEN_TYPE_INSTR;

    return g_Lexer.currentToken;
}

char* GetCurrentLexeme()
{
    return g_Lexer.pCurrentLexeme;
}














