#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sslexeme.h"
#include "sscl.h"

extern _cl::Compiler compiler;

namespace _cl
{
	void LoadScriptSource()
	{
		FILE* pScriptFile = fopen(compiler.lexer.scriptSourceFile, "rb");

		if (pScriptFile == NULL)
		{
			printf("Can not load script file!\n");
			exit(0);
		}

		while(!feof(pScriptFile))
		{
			char* strSourceLine = (char*) malloc(MAX_SOURCE_LINE_SIZE + 1);
			fgets(strSourceLine, MAX_SOURCE_LINE_SIZE, pScriptFile);
			AddNode(&compiler.sourceCode, strSourceLine);
		}

		fclose(pScriptFile);
	}

	void PreProcessSourceCode()
	{
		int isInBlockComment = FALSE;
		int isInString = FALSE;

		LinkListNode* pNode = compiler.sourceCode.pHead;

		while (TRUE)
		{
			char* currentSourceLine = (char*)pNode->pData;
			for (int i = 0; i < (uint) strlen(currentSourceLine); i++)
			{
				if (currentSourceLine[i] == '"')
				{
					if (isInString)
						isInString = FALSE;
					else
						isInString = TRUE;
				}

				if (currentSourceLine[i] == '/' &&
					currentSourceLine[i + 1] == '/' &&
					!isInString &&
					!isInBlockComment)
				{
					currentSourceLine[i] = '\n';
					currentSourceLine[i + 1] = '\0';
					break;
				}

				if (currentSourceLine[i] == '/' &&
					currentSourceLine[i + 1] == '*' &&
					!isInString &&
					!isInBlockComment)
				{
					isInBlockComment = TRUE;
				}

				if (currentSourceLine[i] == '*' &&
					currentSourceLine[i + 1] == '/' &&
					isInBlockComment)
				{
					currentSourceLine[i] = ' ';
					currentSourceLine[i + 1] = ' ';
					isInBlockComment = FALSE;
				}

				if (isInBlockComment)
				{
					if (currentSourceLine[i] != '\n')
						currentSourceLine[i] = ' ';
				}
			}

			pNode = pNode->pNext;

			if (pNode == NULL)
				break;
		}
	}


	void TestLexer()
	{
		Token currentToken;
		int tokenCount = 0;
		char strToken[128];

		while(TRUE)
		{
			currentToken = GetNextToken();

			if (currentToken == CL_TOKEN_TYPE_END_OF_STREAM)
				break;

			switch (currentToken)
			{
			case CL_TOKEN_TYPE_INT:
				strcpy(strToken, "Integer");
				break;
			case CL_TOKEN_TYPE_FLOAT:
				strcpy(strToken, "Float");
				break;
			case CL_TOKEN_TYPE_IDENT:
				strcpy (strToken, "Identifier" );
				break;
			case CL_TOKEN_TYPE_KEYWORD_VAR:
				strcpy(strToken, "var");
				break;
			case CL_TOKEN_TYPE_KEYWORD_TRUE:
				strcpy(strToken, "true");
				break;
			case CL_TOKEN_TYPE_KEYWORD_FALSE:
				strcpy(strToken, "false");
				break;
			case CL_TOKEN_TYPE_KEYWORD_IF:
				strcpy(strToken, "if");
				break;
			case CL_TOKEN_TYPE_KEYWORD_ELSE:
				strcpy(strToken, "else");
				break;
			case CL_TOKEN_TYPE_KEYWORD_BREAK:
				strcpy(strToken, "break");
				break;
			case CL_TOKEN_TYPE_KEYWORD_CONTINUE:
				strcpy(strToken, "continue");
				break;
			case CL_TOKEN_TYPE_KEYWORD_FOR:
				strcpy(strToken, "for");
				break;
			case CL_TOKEN_TYPE_KEYWORD_WHILE:
				strcpy(strToken, "while");
				break;
			case CL_TOKEN_TYPE_KEYWORD_FUNC:
				strcpy(strToken, "func");
				break;
			case CL_TOKEN_TYPE_KEYWORD_RETURN:
				strcpy(strToken, "return");
				break;

			case CL_TOKEN_TYPE_OPERATOR:
				sprintf(strToken, "Operator %d", GetCurrentOperator());
				break;

			case CL_TOKEN_TYPE_DELIM_COMMA:
				strcpy(strToken, "comma");
				break;
			case CL_TOKEN_TYPE_DELIM_OPEN_PAREN:
				strcpy(strToken, "open paren");
				break;
			case CL_TOKEN_TYPE_DELIM_CLOSE_PAREN:
				strcpy(strToken, "close paren");
				break;
			case CL_TOKEN_TYPE_DELIM_OPEN_BRACE:
				strcpy(strToken, "open brace");
				break;
			case CL_TOKEN_TYPE_DELIM_CLOSE_BRACE:
				strcpy(strToken, "close brace");
				break;
			case CL_TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE:
				strcpy(strToken, "open curly brace");
				break;
			case CL_TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE:
				strcpy(strToken, "close curly brace");
				break;
			case CL_TOKEN_TYPE_DELIM_SEMICOLON:
				strcpy(strToken, "semicolon");
				break;

			case CL_TOKEN_TYPE_STRING:
				strcpy(strToken, "String");
				break;
			default:
				break;
			}

			printf("%d: Token: %-16s Lexeme: %s\n", tokenCount, strToken, GetCurrentLexeme());

			tokenCount++;
		}

		printf("\n");
		printf("Total token count: %d\n", tokenCount);


	}

}