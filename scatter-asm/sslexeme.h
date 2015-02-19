#ifndef SSTYPE_H
#define SSTYPE_H

#define TOKEN_TYPE_INT
#define TOKEN_TYPE_FLOAT
#define TOKEN_TYPE_STRING
#define TOKEN_TYPE_QUATE
#define TOKEN_TYPE_IDENT
#define TOKEN_TYPE_COLON
#define TOKEN_TYPE_OPEN_BRACKET
#define TOKEN_TYPE_CLOSE_BRACKET
#define TOKEN_TYPE_COMMA
#define TOKEN_TYPE_OPEN_BRACE
#define TOKEN_TYPE_CLOSE_BRACE
#define TOKEN_TYPE_NEWLINE
#define TOKEN_TYPE_INSTR
#define TOKEN_TYPE_SETSTACKSIZE
#define TOKEN_TYPE_VAR
#define TOKEN_TYPE_FUNC
#define TOKEN_TYPE_PARAM
#define TOKEN_TYPE_REG_RETVAL
#define TOKEN_TYPE_INVALID
#define END_OF_TOKEN_STREAM

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
#endif