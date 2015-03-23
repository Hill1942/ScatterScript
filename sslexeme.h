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

namespace _asm_
{
	void LoadSourceFile();
    char GetLookAheadChar();
    int SkipToNextLine();
    Token GetNextToken();
    char* GetCurrentLexeme();
    
    void ResetLexer();
}

namespace _cl
{
	int     IsCharOpChar(char c, int opCharsTableIndex);
	int     GetOpStateIndex(char c, int opCharsTableIndex, int nextStateIndex, int nextStateCount);
	OpState GetOpState(int opCharsTableIndex, int opStateIndex);
	Token   GetNextToken();
	char    GetNextChar();
	char*   GetCurrentLexeme();
	int     GetCurrentOperator();

	void CopyLexerState(LexerState* pDestState, LexerState* pSourceState);
	void RewindTokenStream();
}

#endif