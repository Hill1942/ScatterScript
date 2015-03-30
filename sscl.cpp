#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sslexeme.h"
#include "ssil.h"
#include "sssystem.h"
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

	void ReadToken(Token token);

	void ParseSourceCode()
	{
		ResetLexer();

		InitStack(&compiler.loopStack);
		compiler.currentScope = SCOPE_GLOBAL;

		while (TRUE) 
		{
			ParseStatement();
			if (GetNextToken() == CL_TOKEN_TYPE_END_OF_STREAM)
				break;
			else 
				RewindTokenStream();
		}
		
		FreeStack(&compiler.loopStack);
	}

	void ParseStatement()
	{
		if (GetLookAheadChar() == ';')
		{
			ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);
			return;
		}

		Token initToken = GetNextToken();
		switch (initToken)
		{
		case CL_TOKEN_TYPE_END_OF_STREAM:
			ExitOnCodeError("Unexpected end of file");
			break;

		case CL_TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE:
			ParseBlock();
			break;

		case CL_TOKEN_TYPE_KEYWORD_VAR:
			ParseVariable();
			break;;

		case CL_TOKEN_TYPE_KEYWORD_FUNC:
			ParseFunction();
			break;

		/*case CL_TOKEN_TYPE_KEYWORD_HOST:
			ParseHost();
			break; */

		case CL_TOKEN_TYPE_KEYWORD_IF:
			ParseIf();
			break;

		case CL_TOKEN_TYPE_KEYWORD_WHILE:
			ParseWhile();
			break;

		case CL_TOKEN_TYPE_KEYWORD_FOR:
			ParseFor();
			break;

		case CL_TOKEN_TYPE_KEYWORD_BREAK:
			ParseBreak();
			break;

		case CL_TOKEN_TYPE_KEYWORD_CONTINUE:
			ParseContinue();
			break;

		case CL_TOKEN_TYPE_KEYWORD_RETURN:
			ParseReturn();
			break;

		case CL_TOKEN_TYPE_IDENT:
			{
				if (GetSymbol(&compiler.symbolTable, GetCurrentLexeme(), compiler.currentScope))
				{
					ParseAssign();
				}
				else if (GetFunction(&compiler.functionTable, GetCurrentLexeme()))
				{
					_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope, GetCurrentSourceLine());
					ParseFunctionCall();

					ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);
				}
				else
				{
					ExitOnCodeError("Invalid identifier");
				}
				break;
			}
		default:
			ExitOnCodeError("Unexpected input");
			break;
		}
	}
	void ParseBlock()
	{
		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("Code blocks illegal in global scope");

		while (GetLookAheadChar() != '}')
		{
			ParseStatement();
		}

		ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE);
	}
	void ParseVariable();
	void ParseHost();
	void ParseFunction()
	{
		if (compiler.currentScope != SCOPE_GLOBAL)
			ExitOnCodeError("Nested functions illegal");

		ReadToken(CL_TOKEN_TYPE_IDENT);

		int funcIndex = AddFunction(&compiler.functionTable, FALSE, GetCurrentLexeme());
		if (funcIndex == -1)
			ExitOnCodeError("Function redefinition");

		compiler.currentScope == funcIndex;

		ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_PAREN);

		if (GetLookAheadChar() != ')') 
		{
			if (compiler.scriptHeader.iIsMainFuncExist && 
				compiler.scriptHeader.iMainFuncIndex == funcIndex)
				ExitOnCodeError("main function cannot accept parameters");

			int paramCount = 0;
			char paramList[MAX_FUNC_PARAM_COUNT][MAX_INDENT_SIZE];

			while (TRUE)
			{
				ReadToken(CL_TOKEN_TYPE_IDENT);
				_cl::CopyCurrentLexeme(paramList[paramCount]);
				paramCount++;

				if (GetLookAheadChar() == ')')
					break;

				ReadToken(CL_TOKEN_TYPE_DELIM_COMMA);
			}

			SetFuncParamCount(&compiler.functionTable, compiler.currentScope, paramCount);

			while (paramCount > 0)
			{
				paramCount--;
				AddSymbol(&compiler.symbolTable, 1, compiler.currentScope,
					SYMBOL_TYPE_PARAM, paramList[paramCount]);
			}
		}

		ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_PAREN);
		ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE);
		ParseBlock();

		compiler.currentScope = SCOPE_GLOBAL;
	}
	void ParseExpression();
	void ParseSubExpression();
	void ParseTerm();
	void ParseFactor();

	void ParseIf();
	void ParseWhile();
	void ParseBreak();
	void ParseFor();
	void ParseContinue();
	void ParseReturn();
	void ParseAssign()
	{
		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("Assigenment illegal in global scope");

		int instrIndex;
		int assignOp;
		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope, GetCurrentSourceLine());
		
		SymbolNode* pSymbol = GetSymbol(&compiler.symbolTable, GetCurrentLexeme(), compiler.currentScope);
		int isArray = FALSE;
		if (GetLookAheadChar() == '[')
		{
			if (pSymbol->iSize == 1)
				ExitOnCodeError("Invalid array");

			ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_BRACE);

			if (GetLookAheadChar() == ']')
				ExitOnCodeError("Invalid expression");

			ParseExpression();

			ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_BRACE);

			isArray = TRUE;
		}
		else
		{
			if (pSymbol->iSize > 1)
				ExitOnCodeError("Arrays must be indexed");
		}

		if ( GetNextToken() != CL_TOKEN_TYPE_OPERATOR && 
			(GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN && 
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_ADD &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_SUB &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_MUL &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_DIV &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_MOD &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_AND &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_OR &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_XOR &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_SHIFT_LEFT &&
			 GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN_SHIFT_RIGHT))
			ExitOnCodeError("Illegal assignment operator");
		else
			assignOp = GetCurrentOperator();

		ParseExpression();

		ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);

		instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope, IL_INSTR_POP);
		_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
			instrIndex, compiler.tempVar0SymbolIndex);
		
	}
	void ParseFunctionCall();

}