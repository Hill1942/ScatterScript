#include <string.h>
#include "../sspre.h"
#include "../ssutil.h"
#include "sslexeme_cl.h"

extern char* g_StringSource_CL;
extern char g_CurrentLexeme_CL[MAX_LEXEME_SIZE];
extern int g_CurrentLexemeStart_CL;
extern int g_CurrentLexemeEnd_CL;
extern int g_CurrentOp_CL;

void CL_InitLexer()
{
	g_CurrentLexemeStart_CL = 0;
	g_CurrentLexemeEnd_CL  = 0;
}

OpState g_OpChars0[MAX_OP_STATE_COUNT] = 
{
	{'+', 0, 2, 0},
	{'-', 0, 2, 1},
	{'*', 0, 2, 2},
	{'/', 0, 2, 3},
	{'%', 0, 2, 4},

	{'&', 0, 2, 20},
	{'|', 0, 2, 21},
	{'~', 0, 2, 23},
	{'^', 0, 2, 22},  //xor

	{'<', 0, 2, 63},
	{'>', 0, 2, 65},
	{'=', 0, 2, 61},

	{'!', 0, 2, 52}
};

OpState g_OpChars1[MAX_OP_STATE_COUNT] = 
{
	{'+', 0, 2, 40},   // ++
	{'=', 0, 2, 10},   // +=
	{'-', 0, 2, 41},   // --
	{'=', 0, 2, 11},   // -=
	{'=', 0, 2, 12},   // *=
	{'=', 0, 2, 13},   // /=
	{'=', 0, 2, 14},   // %=

	{'=', 0, 2, 30},   // &=
	{'&', 0, 2, 50},   // &&
	{'=', 0, 2, 31},   // |=
	{'|', 0, 2, 51},   // ||
	{'=', 0, 2, 32},   // ^=  
					   
	{'=', 0, 2, 64},   // <=
	{'<', 0, 2, 24},   // <<
	{'=', 0, 2, 66},   // >=
	{'>', 0, 2, 25},   // >>
	{'=', 0, 2, 62}	   // !=
};

OpState g_opChars2[MAX_OP_STATE_COUNT] = 
{
	{'=', 0, 2, 33},	  // <<=
	{'=', 0, 2, 34}	  // >>=
};

Token GetNextToken_CL()
{
	g_CurrentLexemeStart_CL = g_CurrentLexemeEnd_CL;

	if (g_CurrentLexemeStart_CL >= (int) strlen(g_StringSource_CL))
		return TOKEN_TYPE_END_OF_STREAM;

	int currentLexemeState = LEX_STATE_START;

	int currentOpCharIndex  = 0;
	int currentOpStateIndex = 0;
	OpState currentOpState;

	char currentChar;

	int  nextLexemeCharIndex;
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
		case LEX_STATE_START:
			{
				if (IsCharWhiteSpace(currentChar))
				{
					g_CurrentLexemeStart_CL++;
					isAddCurrentChar = FALSE;
				}
				else if (IsCharNumeric(currentChar))
				{
					currentLexemeState = LEX_STATE_INT;
				}
				else if (currentChar = '.')
				{
					currentLexemeState = LEX_STATE_FLOAT;
				}
				else if (IsCharIdent(currentChar))
				{
					currentLexemeState = LEX_STATE_IDENT;
				}
				else if (IsCharDelimiter(currentChar))
				{
					currentLexemeState = LEX_STATE_DELIMITER;
				}
				else if (IsCharOpChar(currentChar, 0))
				{
					currentOpStateIndex = GetOpStateIndex(currentChar, 0, 0, 0);
					if (currentOpStateIndex == -1)
						ExitOnInvalidInputError(currentChar);
					currentOpState = GetOpState(0, currentOpStateIndex);

					currentOpCharIndex = 1;
					g_CurrentOp_CL = currentOpState.iIndex;
					currentLexemeState = LEX_STATE_OP;
				}
				else if (currentChar =  '"')
				{
					isAddCurrentChar = FALSE;
					currentLexemeState = LEX_STATE_STRING;
				}
				else
				{
					ExitOnInvalidInputError(currentChar);
				}

				break;
			}

		case LEX_STATE_INT:
			{
				if (IsCharNumeric(currentChar))
					currentLexemeState = LEX_STATE_INT;
				else if (currentChar == '.')
					currentLexemeState = LEX_STATE_FLOAT;
				else if (IsCharWhiteSpace(currentChar) || IsCharDelimiter(currentChar))
				{
					isAddCurrentChar = FALSE;
					isLexemeDone = TRUE;
				}
				else
					ExitOnInvalidInputError(currentChar);

				break;
			}

		case LEX_STATE_FLOAT:
			{
				if (IsCharNumeric(currentChar))
					currentLexemeState = LEX_STATE_FLOAT;
				else if (IsCharWhiteSpace(currentChar) || IsCharDelimiter(currentChar))
				{
					isAddCurrentChar = FALSE;
					isLexemeDone = TRUE;
				}
				else
					ExitOnInvalidInputError(currentChar);

				break;
			}

		case LEX_STATE_IDENT:
			{
				if (IsCharIdent(currentChar))
					currentLexemeState = LEX_STATE_IDENT;
				else if (IsCharWhiteSpace(currentChar) || IsCharDelimiter(currentChar))
				{
					isAddCurrentChar = FALSE;
					isLexemeDone = TRUE;
				}
				else
					ExitOnInvalidInputError(currentChar);

				break;
			}

		case LEX_STATE_OP:
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
					g_CurrentOp_CL = currentOpState.iIndex;
				}
				else
				{
					isAddCurrentChar = FALSE;
					isLexemeDone = TRUE;
					break;
				}
				

				break;
			}

		case LEX_STATE_DELIMITER:
			{
				isAddCurrentChar = FALSE;
				isLexemeDone = TRUE;
				break;
			}

		case LEX_STATE_STRING:
			{
				if (currentChar = '"')
				{
					isAddCurrentChar = FALSE;
					currentLexemeState = LEX_STATE_STRING_CLOSE_QUOTE;
				}
				else if (currentChar = '\\')
				{
					isAddCurrentChar = FALSE;
					currentLexemeState = LEX_STATE_STRING_ESCAPE;
				}

				break;
			}

		case LEX_STATE_STRING_ESCAPE:
			{
				currentLexemeState = LEX_STATE_STRING;

				break;
			}

		case LEX_STATE_STRING_CLOSE_QUOTE:
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
			g_CurrentLexeme_CL[nextLexemeCharIndex] = currentChar;
			nextLexemeCharIndex++;
		}

		if (isLexemeDone)
			break;
	}

	g_CurrentLexeme_CL[nextLexemeCharIndex] = '\0';
	g_CurrentLexemeEnd_CL--;


	Token tokenType;
	switch (currentLexemeState)
	{
	case LEX_STATE_INT:
		{
			tokenType = TOKEN_TYPE_INT;
			break;
		}
	case LEX_STATE_FLOAT:
		{
			tokenType = TOKEN_TYPE_FLOAT;
			break;
		}
	case LEX_STATE_IDENT:
		{
			tokenType = TOKEN_TYPE_IDENT;

			if (strcmp(g_CurrentLexeme_CL, "var") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_VAR;
			if (strcmp(g_CurrentLexeme_CL, "true") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_TRUE;
			if (strcmp(g_CurrentLexeme_CL, "false") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_FALSE;
			if (strcmp(g_CurrentLexeme_CL, "if") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_IF;
			if (strcmp(g_CurrentLexeme_CL, "else") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_ELSE;
			if (strcmp(g_CurrentLexeme_CL, "break") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_BREAK;
			if (strcmp(g_CurrentLexeme_CL, "continue") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_CONTINUE;
			if (strcmp(g_CurrentLexeme_CL, "for") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_FOR;
			if (strcmp(g_CurrentLexeme_CL, "while") == 0)
				tokenType = TOKEN_TYPE_KEYWORD_WHILE;
			if (strcmp(g_CurrentLexeme_CL, "func") == 0)
				tokenType = TOKEN_TYPE_FUNC;
			if (strcmp(g_CurrentLexeme_CL, "return") == 0)
				tokenType = TOKEN_TYPE_RETURN;

			break;
		}
	case LEX_STATE_DELIMITER:
		{
			switch (g_CurrentLexeme_CL[0])
			{
			case ',':
				tokenType = TOKEN_TYPE_DELIM_COMMA;
				break;
			case '(':
				tokenType = TOKEN_TYPE_DELIM_OPEN_PAREN;
				break;
			case ')':
				tokenType = TOKEN_TYPE_DELIM_CLOSE_PAREN;
				break;
			case '[':
				tokenType = TOKEN_TYPE_DELIM_OPEN_BRACE;
				break;
			case ']':
				tokenType = TOKEN_TYPE_DELIM_CLOSE_BRACE;
				break;
			case '{':
				tokenType = TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE;
				break;
			case '}':
				tokenType = TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE;
				break;
			case ';':
				tokenType = TOKEN_TYPE_DELIM_SEMICOLON;
				break;
			}

			break;
		}
	case LEX_STATE_OP:
		{
			tokenType = TOKEN_TYPE_OP;
			break;
		}
	case LEX_STATE_STRING_CLOSE_QUOTE:
		{
			tokenType = TOKEN_TYPE_STRING;
			break;
		}
	default:
		tokenType = TOKEN_TYPE_END_OF_STREAM;
		break;
	}

	return tokenType;

}

char GetNextChar()
{
	return g_StringSource_CL[g_CurrentLexemeEnd_CL++];
}
