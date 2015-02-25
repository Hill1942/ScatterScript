#ifndef SSLEXWME_H
#define SSLEXWME_H

#define OP_FLAG_TYPE_INT           1
#define OP_FLAG_TYPE_FLOAT         2
#define OP_FLAG_TYPE_STRING        4
#define OP_FLAG_TYPE_MEM_REF       8
#define OP_FLAG_TYPE_LINE_LABEL    16
#define OP_FLAG_TYPE_FUNC_NAME     32
#define OP_FLAG_TYPE_HOST_API_CALL 64
#define OP_FLAG_TYPE_REG           128

#define TOKEN_TYPE_INT           0
#define TOKEN_TYPE_FLOAT         1
#define TOKEN_TYPE_STRING        2
#define TOKEN_TYPE_QUATE         3
#define TOKEN_TYPE_IDENT         4
#define TOKEN_TYPE_COLON         5
#define TOKEN_TYPE_OPEN_BRACKET  6
#define TOKEN_TYPE_CLOSE_BRACKET 7
#define TOKEN_TYPE_COMMA         8
#define TOKEN_TYPE_OPEN_BRACE    9
#define TOKEN_TYPE_CLOSE_BRACE   10
#define TOKEN_TYPE_NEWLINE       11
#define TOKEN_TYPE_INSTR         12
#define TOKEN_TYPE_SETSTACKSIZE  13
#define TOKEN_TYPE_VAR           14
#define TOKEN_TYPE_FUNC          15
#define TOKEN_TYPE_PARAM         16
#define TOKEN_TYPE_REG_RETVAL    17
#define TOKEN_TYPE_INVALID       18
#define END_OF_TOKEN_STREAM      19

#define LEX_STATE_NO_STRING  0
#define LEX_STATE_IN_STRING  1
#define LEX_STATE_END_STRING 2

#define MAX_SOURCE_LINE_SIZE 4096

int IsCharNumeric(char cChar);
int IsCharWhiteSpace(char cChar);
int IsCharIdent(char cChar);
int IsCharDelimiter(char cChar);

int IsStringInt(const char* str);
int IsStringFloat(const char* str);
int IsStringWhitespace(const char* str);
int IsStringIdent(const char* str);

void StrpComments(char* sourceLine);
void TrimWhiteSpace(char* sourceLine);

void LoadSourceFile();
char GetLookAheadChar();
int SkipToNextLine();
Token GetNextToken();
char* GetCurrentLexeme();

void ResetLexer();
#endif