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

Token GetNextToken_CL()
{
	g_CurrentLexemeStart_CL = g_CurrentLexemeEnd_CL;

	if (g_CurrentLexemeStart_CL >= (int) strlen(g_StringSource_CL))
		return;

	int currentLexemeState = LEX_STATE_START;

	char currentChar;
	int  nextLexemeCharIndex;
	int  isAddCurrentChar;

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
					isAddCurrentChar = TRUE;
				}
				else if (IsCharNumeric(currentChar))
				{
					currentLexemeState = LEX_STATE_INT;
				}
				else if (currentChar = '.')
				{
					currentLexemeState = LEX_STATE_FLOAT;
				}
				else
				{

				}

				break;
			}

		default:
			break;
		}
	}


}

char GetNextChar()
{
	return g_StringSource_CL[g_CurrentLexemeEnd_CL++];
}
