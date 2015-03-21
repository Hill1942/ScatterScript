#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sslexeme.h"
#include "sscl.h"


namespace _cl
{
	void LoadScriptSource(char* filename, char* stringSource)
	{
		FILE* scriptFile = fopen(filename, "rb");

		if (scriptFile == NULL)
		{
			printf("Can not load script file!\n");
			exit(0);
		}

		fseek(scriptFile, 0, SEEK_END);
		int scriptSize = ftell(scriptFile);
		fseek(scriptFile, 0, SEEK_SET);

		stringSource = (char*) malloc(scriptSize + 1);

		char currentChar;
		for (int i = 0; i < scriptSize; i++)
		{
			currentChar = fgetc(scriptFile);
			if (currentChar == 13)	// '13' means return
			{
				fgetc(scriptFile);	// jump the next char
				scriptSize--;
				stringSource[i] = '\n';
			}
			else
			{
				stringSource[i] = currentChar;
			}
		}

		stringSource[scriptSize] = '\0';

		fclose(scriptFile);
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

			printf("%d: Token: %s. Lexeme: %s\n", tokenCount, strToken, GetCurrentLexeme());

			tokenCount++;
		}

		printf("\n");
		printf("Total token count: %d\n", tokenCount);


	}

}