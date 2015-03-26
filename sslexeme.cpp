#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sssystem.h"
#include "sslexeme.h"

extern _asm_::ASM sasm;
extern _cl::Lexer cl_lexer;

_cl::OpState g_OpChars0[MAX_OP_STATE_COUNT] = 
{
	{'+', 0,  2, 0},
	{'-', 2,  2, 1},
	{'*', 4,  1, 2},
	{'/', 5,  1, 3},
	{'%', 6,  1, 4},

	{'&', 7,  2, 20},
	{'|', 9,  2, 21},
	{'^', 11, 2, 22},  //xor
	{'~', 0,  0, 23},

	{'<', 12, 2, 63},
	{'>', 14, 2, 65},
	{'=', 17, 1, 42},

	{'!', 16, 1, 52}
};

_cl::OpState g_OpChars1[MAX_OP_STATE_COUNT] = 
{
	{'+', 0, 0, 40},   // ++  0
	{'=', 0, 0, 10},   // +=  1
	{'-', 0, 0, 41},   // --  2
	{'=', 0, 0, 11},   // -=  3
	{'=', 0, 0, 12},   // *=  4
	{'=', 0, 0, 13},   // /=  5
	{'=', 0, 0, 14},   // %=  6

	{'=', 0, 0, 30},   // &=  7
	{'&', 0, 0, 50},   // &&  8
	{'=', 0, 0, 31},   // |=  9
	{'|', 0, 0, 51},   // ||  10
	{'=', 0, 0, 32},   // ^=  11

	{'=', 0, 0, 64},   // <=  12
	{'<', 0, 1, 24},   // <<  13
	{'=', 0, 0, 66},   // >=  14
	{'>', 0, 1, 25},   // >>  15
	{'=', 0, 0, 62},   // !=  16
	{'=', 0, 0, 61},   // ==  17

};

_cl::OpState g_opChars2[MAX_OP_STATE_COUNT] = 
{
	{'=', 0, 0, 33},   // <<=
	{'=', 0, 0, 34}	   // >>=
};


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

int IsCharWhiteSpace_CL(char cChar)
{
	if (cChar == ' ' || cChar == '\t' || cChar == '\n')
		return TRUE;
	else
		return FALSE;
}
int IsCharReturnOrNewline(char cChar)
{
    if (cChar == '\r' || cChar == '\n')
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
            IsCharWhiteSpace(cChar) || IsCharReturnOrNewline(cChar))
        return TRUE;
    else
        return FALSE;
}

int IsCharDelimiter_CL(char cChar)
{
	if (cChar == ';' || cChar == ',' ||
		cChar == '[' || cChar == ']' ||
		cChar == '{' || cChar == '}' ||
		cChar == '(' || cChar == ')' )
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

    if (str[0] >= '0' && str[0] <= '9')
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
        //calculate the number of white space at left side
        for (currentIndex = 0; currentIndex < strLength; currentIndex++)
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
                sourceLine[currentIndex + 1] = '\0';
                break;
            }
        }
    }
}

namespace _asm_
{
    void LoadSourceFile()
    {
        if (! (sasm.sourceFile = fopen(sasm.sourceFileName, "rb")))
            ExitOnError("Could not open source file");
    
        while (!feof(sasm.sourceFile))
        {
            if (fgetc(sasm.sourceFile) == '\n')
                sasm.sourceCodeLines++;
        }
        sasm.sourceCodeLines++;
    
        rewind(sasm.sourceFile);
    
        if (! (sasm.sourceCode = (char**)malloc(sasm.sourceCodeLines * sizeof(char*))))
            ExitOnError("Could not allocate space for source code");
    
        for (int i = 0; i < sasm.sourceCodeLines; i++)
        {
            if (! (sasm.sourceCode[i] = (char*)malloc(MAX_SOURCE_LINE_SIZE + 1)))
                ExitOnError("Could not allocate space for source code");
    
            fgets(sasm.sourceCode[i], MAX_SOURCE_LINE_SIZE, sasm.sourceFile);
    
            StripComments(sasm.sourceCode[i]);
            TrimWhiteSpace(sasm.sourceCode[i]);
    
            // Make sure to add a new newline if it was removed by the stripping of the
            // comments and whitespace. We do this by checking the character right before
            // the null terminator to see if it's \n. If not, we move the terminator over
            // by one and add it. We use strlen () to find the position of the newline
            // easily.
    
            int newLineIdex = strlen(sasm.sourceCode[i]) - 1;
            if (sasm.sourceCode[i][newLineIdex] != '\n' )
            {
                sasm.sourceCode[i][newLineIdex + 1] = '\n';
                sasm.sourceCode[i][newLineIdex + 2] = '\0';
            }
        }
    }
    
    char GetLookAheadChar()
    {
        int currentSourceLine = sasm.lexer.iCurrentSourceLine;
        unsigned int index    = sasm.lexer.iIndex1;
    
        if (sasm.lexer.iState != ASM_LEX_STATE_IN_STRING)
        {
            while (TRUE)
            {
                if (index >= strlen(sasm.sourceCode[currentSourceLine]))
                {
                    currentSourceLine++;
                    if (currentSourceLine >= sasm.sourceCodeLines)
                        return 0;
                    index = 0;
                }
                if (!IsCharWhiteSpace(sasm.sourceCode[currentSourceLine][index]))
                    break;
                index++;
            }
        }
        return sasm.sourceCode[currentSourceLine][index];
    }
    
    int SkipToNextLine()
    {
        sasm.lexer.iCurrentSourceLine++;
        if (sasm.lexer.iCurrentSourceLine >= sasm.sourceCodeLines)
            return FALSE;
    
        sasm.lexer.iIndex0 = 0;
        sasm.lexer.iIndex1 = 0;
    
        sasm.lexer.iState = ASM_LEX_STATE_NO_STRING;
    
        return TRUE;
    }
    
    Token GetNextToken()
    {
        //When we read the source code line by line, we use two index to indicate the both ends of a source line.
        //At first, the right end index equals to the left end index: they both are 0.
        sasm.lexer.iIndex0 = sasm.lexer.iIndex1;
        if (sasm.lexer.iIndex0 >= strlen(sasm.sourceCode[sasm.lexer.iCurrentSourceLine]))
        {
            if (!SkipToNextLine())
                return ASM_END_OF_TOKEN_STREAM;
        }
    
        if (sasm.lexer.iState == ASM_LEX_STATE_END_STRING)
            sasm.lexer.iState = ASM_LEX_STATE_NO_STRING;
        //locate index to the first non-whitespace character of a source code line
        if (sasm.lexer.iState != ASM_LEX_STATE_IN_STRING)
        {
            while (TRUE)
            {
                if (!IsCharWhiteSpace((sasm.sourceCode[sasm.lexer.iCurrentSourceLine][sasm.lexer.iIndex0])))
                    break;
                sasm.lexer.iIndex0++;
            }
        }
        sasm.lexer.iIndex1 = sasm.lexer.iIndex0;
    
        //move iIndex1 to the location of next token end
        while (TRUE)
        {
            if (sasm.lexer.iState == ASM_LEX_STATE_IN_STRING)
            {
                if (sasm.lexer.iIndex1 >= strlen(sasm.sourceCode[sasm.lexer.iCurrentSourceLine]))
                {
                    sasm.lexer.currentToken = ASM_TOKEN_TYPE_INVALID;
                    return sasm.lexer.currentToken;
                }
                if (sasm.sourceCode[sasm.lexer.iCurrentSourceLine][sasm.lexer.iIndex1] == '\\')
                {
                    sasm.lexer.iIndex1 += 2;
                    continue;
                }
                if (sasm.sourceCode[sasm.lexer.iCurrentSourceLine][sasm.lexer.iIndex1] == '"')
                    break;
                sasm.lexer.iIndex1++;
            }
            else
            {
                if (sasm.lexer.iIndex1 >= strlen(sasm.sourceCode[sasm.lexer.iCurrentSourceLine]))
                    break;
                if (IsCharDelimiter(sasm.sourceCode[sasm.lexer.iCurrentSourceLine][sasm.lexer.iIndex1]))
                    break;
                sasm.lexer.iIndex1++;
            }
        }
    
        if (sasm.lexer.iIndex1 - sasm.lexer.iIndex0 == 0)
            sasm.lexer.iIndex1++;
    
        //Get the next lexeme
        unsigned int currentTargetIndex = 0;
        for (int i = sasm.lexer.iIndex0; i < sasm.lexer.iIndex1; i++)
        {
            if (sasm.lexer.iState == ASM_LEX_STATE_IN_STRING)
                if (sasm.sourceCode[sasm.lexer.iCurrentSourceLine][i] == '\\')
                    i++;
            sasm.lexer.pCurrentLexeme[currentTargetIndex] = sasm.sourceCode[sasm.lexer.iCurrentSourceLine][i];
            currentTargetIndex++;
        }
        sasm.lexer.pCurrentLexeme[currentTargetIndex] = '\0';
    	/*if (sasm.lexer.iState != ASM_LEX_STATE_IN_STRING)
    	strtoupper(sasm.lexer.pCurrentLexeme);*/
    
        //Decide which token the lexeme is
        sasm.lexer.currentToken = ASM_TOKEN_TYPE_INVALID;
        if (strlen(sasm.lexer.pCurrentLexeme) > 1 || sasm.lexer.pCurrentLexeme[0] != '"')
        {
            if (sasm.lexer.iState == ASM_LEX_STATE_IN_STRING)
            {
                sasm.lexer.currentToken = ASM_TOKEN_TYPE_STRING;
                return ASM_TOKEN_TYPE_STRING;
            }
        }
    
        if (IsStringInt(sasm.lexer.pCurrentLexeme))
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_INT;
    
        if (IsStringFloat(sasm.lexer.pCurrentLexeme))
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_FLOAT;
    
        if (IsStringIdent(sasm.lexer.pCurrentLexeme))
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_IDENT;
    
        if (strcmp(sasm.lexer.pCurrentLexeme, ASM_KW_SET_STACK_SIZE) == 0)
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_SETSTACKSIZE;
    
        if (strcmp(sasm.lexer.pCurrentLexeme, ASM_KW_VAR) == 0)
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_VAR;
    
        if (strcmp(sasm.lexer.pCurrentLexeme, ASM_KW_FUNCTION) == 0)
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_FUNC;
    
        if (strcmp(sasm.lexer.pCurrentLexeme, ASM_KW_PARAM) == 0)
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_PARAM;
    
        if (strcmp(sasm.lexer.pCurrentLexeme, ASM_KW_RETVAL) == 0)
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_REG_RETVAL;
    
    	if (strlen(sasm.lexer.pCurrentLexeme) == 1)
    	{
    		switch (sasm.lexer.pCurrentLexeme[0])
    		{
    		case '"':
    			switch (sasm.lexer.iState)
    			{
    			case ASM_LEX_STATE_NO_STRING:
    				sasm.lexer.iState = ASM_LEX_STATE_IN_STRING;
    				break;
    			case ASM_LEX_STATE_IN_STRING:
    				sasm.lexer.iState = ASM_LEX_STATE_END_STRING;
    			}
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_QUATE;
    			break;
    		case ',':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_COMMA;
    			break;
    		case ':':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_COLON;
    			break;
    		case '[':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_OPEN_BRACKET;
    			break;
    		case ']':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_CLOSE_BRACKET;
    			break;
    		case '{':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_OPEN_BRACE;
    			break;
    		case '}':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_CLOSE_BRACE;
    			break;
    		case '\n':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_NEWLINE;
    			break;
    		case '\r':
    			sasm.lexer.currentToken = ASM_TOKEN_TYPE_NEWLINE;
    			break;
    		}
    	}
    
        InstrLookup instrLookup;
        if (GetInstruction(sasm.instrLookup, sasm.lexer.pCurrentLexeme, &instrLookup))
            sasm.lexer.currentToken = ASM_TOKEN_TYPE_INSTR;
    
        return sasm.lexer.currentToken;
    }
    
    char* GetCurrentLexeme()
    {
        return sasm.lexer.pCurrentLexeme;
    }
    
    void ResetLexer()
    {
        sasm.lexer.iCurrentSourceLine = 0;
        sasm.lexer.iIndex0 = 0;
        sasm.lexer.iIndex1 = 0;
    
        sasm.lexer.iState = ASM_LEX_STATE_NO_STRING;
        sasm.lexer.currentToken = ASM_TOKEN_TYPE_INVALID;
    }
    
}

namespace _cl
{
    int IsCharOpChar(char c, int opCharsTableIndex)
    {
    	for (int i = 0; i < MAX_OP_STATE_COUNT; i++)
    	{
    		char opChar;
    		switch (opCharsTableIndex)
    		{
    		case 0:
    			opChar = g_OpChars0[i].cChar;
    			break;
    		case 1:
    			opChar = g_OpChars1[i].cChar;
    			break;
    		case 2:
    			opChar = g_opChars2[i].cChar;
    			break;
    		default:
    			break;
    		}
    
    		if (opChar == c)
    			return TRUE;
    	}
    
    	return FALSE;
    }
    
    int GetOpStateIndex(char c, int opCharsTableIndex, int nextStateIndex, int nextStateCount)
    {
    	int startStateIndex;
    	int endStateIndex;
    
    	if (opCharsTableIndex == 0)
    	{
    		startStateIndex = 0;
    		endStateIndex   = MAX_OF_STATE_COUNT;
    	}
    	else
    	{
    		startStateIndex = nextStateIndex;
    		endStateIndex   = startStateIndex + nextStateCount;
    	}
    
    	for (int i = startStateIndex; i < endStateIndex; i++)
    	{
    		char opChar;
    		switch (opCharsTableIndex)
    		{
    		case 0:
    			opChar = g_OpChars0[i].cChar;
    			break;
    		case 1:
    			opChar = g_OpChars1[i].cChar;
    			break;
    		case 2:
    			opChar = g_opChars2[i].cChar;
    			break;
    		default:
    			break;
    		}
    
    		if (opChar == c)
    			return i;
    	}
    
    	return -1;
    }
    
    OpState GetOpState(int opCharsTableIndex, int opStateIndex)
    {
    	OpState state;
    	switch (opCharsTableIndex)
    	{
    	case 0:
    		state = g_OpChars0[opStateIndex];
    		break;
    	case 1:
    		state = g_OpChars1[opStateIndex];
    		break;
    	case 2:
    		state = g_opChars2[opStateIndex];
    		break;
    	default:
    		break;
    	}
    
    	return state;
    }
    
    Token GetNextToken()
    {
		CopyLexerState(&cl_lexer.prevLexerState, &cl_lexer.currLexerState);
    	cl_lexer.currLexerState.currentLexemeStart = cl_lexer.currLexerState.currentLexemeEnd;
    
		if (cl_lexer.currLexerState.currentLexemeStart >= (int) strlen(cl_lexer.stringSource))
    		return CL_TOKEN_TYPE_END_OF_STREAM;
    
    	int currentLexemeState = CL_LEX_STATE_START;
    
    	int currentOpCharIndex  = 0;
    	int currentOpStateIndex = 0;
    	OpState currentOpState;
    
    	char currentChar;
    
    	int  nextLexemeCharIndex = 0;
    	int  isAddCurrentChar;
    
    	int  isLexemeDone = FALSE;
    	while (TRUE)
    	{
    		currentChar = GetNextChar();
    		if (currentChar == '\0')
    			break;
    
    		isAddCurrentChar = TRUE;
    
    		switch (currentLexemeState)
    		{
    		case CL_LEX_STATE_START:
    			{
    				if (IsCharWhiteSpace_CL(currentChar))
    				{
    					cl_lexer.currLexerState.currentLexemeStart++;
    					isAddCurrentChar = FALSE;
    				}
    				else if (IsCharNumeric(currentChar))
    				{
    					currentLexemeState = CL_LEX_STATE_INT;
    				}
    				else if (currentChar == '.')
    				{
    					currentLexemeState = CL_LEX_STATE_FLOAT;
    				}
    				else if (IsCharIdent(currentChar))
    				{
    					currentLexemeState = CL_LEX_STATE_IDENT;
    				}
    				else if (IsCharDelimiter_CL(currentChar))
    				{
    					currentLexemeState = CL_LEX_STATE_DELIMITER;
    				}
    				else if (IsCharOpChar(currentChar, 0))
    				{
    					currentOpStateIndex = GetOpStateIndex(currentChar, 0, 0, 0);
    					if (currentOpStateIndex == -1)
    						ExitOnInvalidInputError(currentChar);
    					currentOpState = GetOpState(0, currentOpStateIndex);
    
    					currentOpCharIndex = 1;
    					cl_lexer.currLexerState.currentOp = currentOpState.iIndex;
    					currentLexemeState = CL_LEX_STATE_OP;
    				}
    				else if (currentChar ==  '"')
    				{
    					isAddCurrentChar = FALSE;
    					currentLexemeState = CL_LEX_STATE_STRING;
    				}
    				else
    				{
    					ExitOnInvalidInputError(currentChar);
    				}
    
    				break;
    			}
    
    		case CL_LEX_STATE_INT:
    			{
    				if (IsCharNumeric(currentChar))
    					currentLexemeState = CL_LEX_STATE_INT;
    				else if (currentChar == '.')
    					currentLexemeState = CL_LEX_STATE_FLOAT;
    				else if (IsCharWhiteSpace_CL(currentChar) || IsCharDelimiter_CL(currentChar))
    				{
    					isAddCurrentChar = FALSE;
    					isLexemeDone = TRUE;
    				}
    				else
    					ExitOnInvalidInputError(currentChar);
    
    				break;
    			}
    
    		case CL_LEX_STATE_FLOAT:
    			{
    				if (IsCharNumeric(currentChar))
    					currentLexemeState = CL_LEX_STATE_FLOAT;
    				else if (IsCharWhiteSpace_CL(currentChar) || IsCharDelimiter_CL(currentChar))
    				{
    					isAddCurrentChar = FALSE;
    					isLexemeDone = TRUE;
    				}
    				else
    					ExitOnInvalidInputError(currentChar);
    
    				break;
    			}
    
    		case CL_LEX_STATE_IDENT:
    			{
    				if (IsCharIdent(currentChar))
    					currentLexemeState = CL_LEX_STATE_IDENT;
    				else if (IsCharWhiteSpace_CL(currentChar) || 
						     IsCharDelimiter_CL(currentChar)  ||
							 IsCharOpChar(currentChar, 0))
    				{
    					isAddCurrentChar = FALSE;
    					isLexemeDone     = TRUE;
    				}
    				else
    					ExitOnInvalidInputError(currentChar);
    
    				break;
    			}
    
    		case CL_LEX_STATE_OP:
    			{
    				if (currentOpState.iSubStateCount == 0)
    				{
    					isAddCurrentChar = FALSE;
    					isLexemeDone = TRUE;
    					break;
    				}
    
    				if (IsCharOpChar(currentChar, currentOpCharIndex))
    				{
    					currentOpStateIndex = GetOpStateIndex(currentChar, 
    						                                  currentOpCharIndex,
    														  currentOpState.iSubStateIndex,
    														  currentOpState.iSubStateCount);
    					if (currentOpStateIndex == -1)
    						ExitOnInvalidInputError(currentChar);
    
    					currentOpState = GetOpState(currentOpCharIndex, currentOpStateIndex);
    
    					currentOpCharIndex++;
    					cl_lexer.currLexerState.currentOp = currentOpState.iIndex;
    				}
    				else
    				{
    					isAddCurrentChar = FALSE;
    					isLexemeDone = TRUE;
    					break;
    				}
    				
    
    				break;
    			}
    
    		case CL_LEX_STATE_DELIMITER:
    			{
    				isAddCurrentChar = FALSE;
    				isLexemeDone = TRUE;
    				break;
    			}
    
    		case CL_LEX_STATE_STRING:
    			{
    				if (currentChar == '"')
    				{
    					isAddCurrentChar = FALSE;
    					currentLexemeState = CL_LEX_STATE_STRING_CLOSE_QUOTE;
    				}
    				else if (currentChar == '\\')
    				{
    					isAddCurrentChar = FALSE;
    					currentLexemeState = CL_LEX_STATE_STRING_ESCAPE;
    				}
    
    				break;
    			}
    
    		case CL_LEX_STATE_STRING_ESCAPE:
    			{
    				currentLexemeState = CL_LEX_STATE_STRING;
    
    				break;
    			}
    
    		case CL_LEX_STATE_STRING_CLOSE_QUOTE:
    			{
    				isAddCurrentChar = FALSE;
    				isLexemeDone = TRUE;
    
    				break;
    			}
    
    		default:
    			break;
    		}
    
    		if (isAddCurrentChar)
    		{
    			cl_lexer.currLexerState.currentLexeme[nextLexemeCharIndex] = currentChar;
    			nextLexemeCharIndex++;
    		}
    
    		if (isLexemeDone)
    			break;
    	}
    
    	cl_lexer.currLexerState.currentLexeme[nextLexemeCharIndex] = '\0';
    	cl_lexer.currLexerState.currentLexemeEnd--;
    
    
    	Token tokenType;
    	switch (currentLexemeState)
    	{
    	case CL_LEX_STATE_INT:
    		{
    			tokenType = CL_TOKEN_TYPE_INT;
    			break;
    		}
    	case CL_LEX_STATE_FLOAT:
    		{
    			tokenType = CL_TOKEN_TYPE_FLOAT;
    			break;
    		}
    	case CL_LEX_STATE_IDENT:
    		{
    			tokenType = CL_TOKEN_TYPE_IDENT;
    
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_VAR) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_VAR;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_TRUE) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_TRUE;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_FALSE) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_FALSE;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_IF) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_IF;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_ELSE) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_ELSE;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_BREAK) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_BREAK;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_CONTINUE) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_CONTINUE;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_FOR) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_FOR;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_WHILE) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_WHILE;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_FUNC) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_FUNC;
    			if (strcmp(cl_lexer.currLexerState.currentLexeme, CL_KW_RETURN) == 0)
    				tokenType = CL_TOKEN_TYPE_KEYWORD_RETURN;
    
    			break;
    		}
    	case CL_LEX_STATE_DELIMITER:
    		{
    			switch (cl_lexer.currLexerState.currentLexeme[0])
    			{
    			case ',':
    				tokenType = CL_TOKEN_TYPE_DELIM_COMMA;
    				break;
    			case '(':
    				tokenType = CL_TOKEN_TYPE_DELIM_OPEN_PAREN;
    				break;
    			case ')':
    				tokenType = CL_TOKEN_TYPE_DELIM_CLOSE_PAREN;
    				break;
    			case '[':
    				tokenType = CL_TOKEN_TYPE_DELIM_OPEN_BRACE;
    				break;
    			case ']':
    				tokenType = CL_TOKEN_TYPE_DELIM_CLOSE_BRACE;
    				break;
    			case '{':
    				tokenType = CL_TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE;
    				break;
    			case '}':
    				tokenType = CL_TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE;
    				break;
    			case ';':
    				tokenType = CL_TOKEN_TYPE_DELIM_SEMICOLON;
    				break;
    			}
    
    			break;
    		}
    	case CL_LEX_STATE_OP:
    		{
    			tokenType = CL_TOKEN_TYPE_OPERATOR;
    			break;
    		}
    	case CL_LEX_STATE_STRING_CLOSE_QUOTE:
    		{
    			tokenType = CL_TOKEN_TYPE_STRING;
    			break;
    		}
    	default:
    		tokenType = CL_TOKEN_TYPE_END_OF_STREAM;
    		break;
    	}
    
    	return tokenType;
    
    }
    
    char GetNextChar()
    {
		return cl_lexer.stringSource[cl_lexer.currLexerState.currentLexemeEnd++];
    }

	char* GetCurrentLexeme()
	{
		return cl_lexer.currLexerState.currentLexeme;
	}

	int GetCurrentLexemeStartIndex()
	{
		return cl_lexer.currLexerState.currentLexemeStart;
	}

	char* GetCurrentSourceLine()
	{
		if (cl_lexer.currLexerState.pCurrentLine)
			return (char*)cl_lexer.currLexerState.pCurrentLine->pData;
		else
			return NULL;
	}

	int GetCurrentSourceLineIndex()
	{
		return cl_lexer.currLexerState.iCurrentLineIndex;
	}

	int GetCurrentOperator()
	{
		return cl_lexer.currLexerState.currentOp;
	}

	void CopyLexerState(LexerState* pDestState, LexerState* pSourceState)
	{
		pDestState->iCurrentLineIndex  = pSourceState->iCurrentLineIndex;
		pDestState->pCurrentLine       = pSourceState->pCurrentLine;
		pDestState->currentToken       = pSourceState->currentToken;
		pDestState->currentLexemeStart = pSourceState->currentLexemeStart;
		pDestState->currentLexemeEnd   = pSourceState->currentLexemeEnd;
		pDestState->currentOp          = pSourceState->currentOp;

		strcpy(pDestState->currentLexeme, pSourceState->currentLexeme);
	}
	void RewindTokenStream()
	{
		CopyLexerState(&cl_lexer.currLexerState, &cl_lexer.prevLexerState);
	}
}
















