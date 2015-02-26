#ifndef SSLEXWME_H
#define SSLEXWME_H

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