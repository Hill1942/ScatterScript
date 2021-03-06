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
#include "ssfuncs.h"

extern _cl::Compiler compiler;

namespace _cl
{
	void LoadScriptSource()
	{
		FILE* pScriptFile = fopen(compiler.scriptSourceFile, "rb");

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

	void ReadToken(Token token)
	{
		if (GetNextToken() != token)
		{
			char errorMsg[256];
			switch (token)
			{
			case CL_TOKEN_TYPE_INT:
				strcpy(errorMsg, "Integer");
				break;
			case CL_TOKEN_TYPE_FLOAT:
				strcpy(errorMsg, "Float");
				break;
			case CL_TOKEN_TYPE_IDENT:
				strcpy(errorMsg, "Identifier");
				break;
			case CL_TOKEN_TYPE_KEYWORD_VAR:
				strcpy(errorMsg, "var");
				break;
			case CL_TOKEN_TYPE_KEYWORD_TRUE:
				strcpy(errorMsg, "true");
				break;
			case CL_TOKEN_TYPE_KEYWORD_FALSE:
				strcpy(errorMsg, "false");
				break;
			case CL_TOKEN_TYPE_KEYWORD_IF:
				strcpy(errorMsg, "if");
				break;
			case CL_TOKEN_TYPE_KEYWORD_ELSE:
				strcpy(errorMsg, "else");
				break;
			case CL_TOKEN_TYPE_KEYWORD_BREAK:
				strcpy(errorMsg, "break");
				break;
			case CL_TOKEN_TYPE_KEYWORD_CONTINUE:
				strcpy(errorMsg, "continue");
				break;
			case CL_TOKEN_TYPE_KEYWORD_FOR:
				strcpy(errorMsg, "for");
				break;
			case CL_TOKEN_TYPE_KEYWORD_WHILE:
				strcpy(errorMsg, "while");
				break;
			case CL_TOKEN_TYPE_KEYWORD_FUNC:
				strcpy(errorMsg, "func");
				break;
			case CL_TOKEN_TYPE_KEYWORD_RETURN:
				strcpy(errorMsg, "return");
				break;
			case CL_TOKEN_TYPE_OPERATOR:
				strcpy(errorMsg, "operator");
				break;
			case CL_TOKEN_TYPE_DELIM_COMMA:
				strcpy(errorMsg, ",");
				break;
			case CL_TOKEN_TYPE_DELIM_OPEN_PAREN:
				strcpy(errorMsg, "(");
				break;
			case CL_TOKEN_TYPE_DELIM_CLOSE_PAREN:
				strcpy(errorMsg, ")");
				break;
			case CL_TOKEN_TYPE_DELIM_OPEN_BRACE:
				strcpy(errorMsg, "[");
				break;
			case CL_TOKEN_TYPE_DELIM_CLOSE_BRACE:
				strcpy(errorMsg, "]");
				break;
			case CL_TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE:
				strcpy(errorMsg, "{");
				break;
			case CL_TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE:
				strcpy(errorMsg, "}");
				break;
			case CL_TOKEN_TYPE_DELIM_SEMICOLON:
				strcpy(errorMsg, ";");
				break;
			case CL_TOKEN_TYPE_STRING:
				strcpy(errorMsg, "string");
				break;


			default:
				break;
			}

			strcat(errorMsg, " expected");
			ExitOnCodeError(errorMsg);
		}
	}

	int IsOprandRelational(int opType)
	{
		if (opType != CL_OPERATOR_TYPE_EQUAL &&
			opType != CL_OPERATOR_TYPE_NOT_EQUAL &&
			opType != CL_OPERATOR_TYPE_LESS &&
			opType != CL_OPERATOR_TYPE_LESS_EQUAL &&
			opType != CL_OPERATOR_TYPE_GREATER &&
			opType != CL_OPERATOR_TYPE_GREATER_EQUAL)
			return FALSE;
		else
			return TRUE;
	}

	int IsOprandLogical(int opType)
	{
		if (opType != CL_OPERATOR_TYPE_LOGICAL_AND && 
			opType != CL_OPERATOR_TYPE_LOGICAL_OR && 
			opType != CL_OPERATOR_TYPE_LOGICAL_NOT)
			return FALSE;
		else
			return TRUE;
	}


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
				else if (GetBuiltInFunc(&compiler.builInFuncTable, GetCurrentLexeme()))
				{
					_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope, GetCurrentSourceLine());

					ParseBuildInFuncCall();

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
	void ParseVariable()
	{
		ReadToken(CL_TOKEN_TYPE_IDENT);

		char strIdentifier[MAX_INDENT_SIZE];
		CopyCurrentLexeme(strIdentifier);

		int size = 1;

		if (GetLookAheadChar() == '[') 
		{
			ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_BRACE);
			ReadToken(CL_TOKEN_TYPE_INT);

			size = atoi(GetCurrentLexeme());

			ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_BRACE);

			if (AddSymbol(&compiler.symbolTable, size, compiler.currentScope,
				SYMBOL_TYPE_VAR, strIdentifier) == -1)
				ExitOnCodeError("identifier redefinition");
		}
		else
		{
			if (AddSymbol(&compiler.symbolTable, size, compiler.currentScope,
				SYMBOL_TYPE_VAR, strIdentifier) == -1)
				ExitOnCodeError("identifier redefinition");

			SymbolNode* pSymbol = GetSymbol(&compiler.symbolTable, GetCurrentLexeme(), compiler.currentScope);

			if (GetLookAheadChar() == '=') 
			{
				_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope, GetCurrentSourceLine());

				if ( GetNextToken() != CL_TOKEN_TYPE_OPERATOR || 
					GetCurrentOperator() != CL_OPERATOR_TYPE_ASSIGN )
					ExitOnCodeError("\"=\" expected");

				ParseExpression();

				ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);

				//pop t0 (get the value of expression)
				int instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope, IL_INSTR_POP);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
					instrIndex, compiler.tempVar0SymbolIndex);

				

				//move (var), t0
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
					compiler.currentScope, IL_INSTR_MOV);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope, 
					instrIndex, pSymbol->iIndex);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
					instrIndex, compiler.tempVar0SymbolIndex);
			}
			else
			{
				ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);
			}
		}	
	}
	void ParseFunction()
	{
		if (compiler.currentScope != SCOPE_GLOBAL)
			ExitOnCodeError("Nested functions illegal");

		ReadToken(CL_TOKEN_TYPE_IDENT);

		int funcIndex = AddFunction(&compiler.functionTable, FALSE, GetCurrentLexeme());
		if (funcIndex == -1)
			ExitOnCodeError("Function redefinition");

		compiler.currentScope = funcIndex;

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

	void ParseExpression()
	{
		int instrIndex;
		int opType;

		ParseSubExpression();

		while (TRUE)
		{
			if (GetNextToken() != CL_TOKEN_TYPE_OPERATOR || 
				(!IsOprandLogical(GetCurrentOperator()) && 
				 !IsOprandRelational(GetCurrentOperator())))
			{
				RewindTokenStream();
				break;
			}

			opType = GetCurrentOperator();

			ParseSubExpression();
			//pop t1
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar1SymbolIndex);
			//pop t0
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);

			if (IsOprandRelational(opType))
			{
				int trueJumpTargetIndex = _IL::GetNextJumpTargetIndex();
				int exitJumpTargetIndex = _IL::GetNextJumpTargetIndex();

				switch (opType)
				{
				case CL_OPERATOR_TYPE_EQUAL:
					instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
						compiler.currentScope, IL_INSTR_JE);
					break;
				case CL_OPERATOR_TYPE_NOT_EQUAL:
					instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
						compiler.currentScope, IL_INSTR_JNE);
					break;
				case CL_OPERATOR_TYPE_GREATER:
					instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
						compiler.currentScope, IL_INSTR_JG);
					break;
				case CL_OPERATOR_TYPE_GREATER_EQUAL:
					instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
						compiler.currentScope, IL_INSTR_JGE);
					break;
				case CL_OPERATOR_TYPE_LESS:
					instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
						compiler.currentScope, IL_INSTR_JL);
					break;
				case CL_OPERATOR_TYPE_LESS_EQUAL:
					instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
						compiler.currentScope, IL_INSTR_JLE);
					break;
				default:
					break;
				}

				_IL::AddILCodeOprand_Variable(&compiler.functionTable,
					compiler.currentScope, instrIndex, compiler.tempVar0SymbolIndex);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable,
					compiler.currentScope, instrIndex, compiler.tempVar1SymbolIndex);
				_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, 
					compiler.currentScope, instrIndex, trueJumpTargetIndex);

				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_PUSH);
				_IL::AddILCodeOprand_Int(&compiler.functionTable,
					compiler.currentScope, instrIndex, 0);

				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_JMP);
				_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable,
					compiler.currentScope, instrIndex, exitJumpTargetIndex);

				_IL::AddILCodeJumpTarget(&compiler.functionTable,
					compiler.currentScope, trueJumpTargetIndex);

				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_PUSH);
				_IL::AddILCodeOprand_Int(&compiler.functionTable,
					compiler.currentScope, instrIndex, 1);

				_IL::AddILCodeJumpTarget(&compiler.functionTable,
					compiler.currentScope, exitJumpTargetIndex);

				/************************************************************************
				    The asm code generated by the above code:
					
					pop t0
					pop t1
					je  t0, t1, TRUE (or jne, jl, jg, ...)
					push 0
					jmp EXIT
				  TRUE:
				    push 1
				  EXIT:

				*************************************************************************/
			}
			else
			{
				switch (opType)
				{
				case CL_OPERATOR_TYPE_LOGICAL_AND:
					{
						int falseJumpTargetIndex = _IL::GetNextJumpTargetIndex();
						int exitJumpTargetIndex  = _IL::GetNextJumpTargetIndex();

						//je t0, 0, FALSE
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_JE);
						_IL::AddILCodeOprand_Variable(&compiler.functionTable,
							compiler.currentScope, instrIndex, compiler.tempVar0SymbolIndex);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 0);
						_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, 
							compiler.currentScope, instrIndex, falseJumpTargetIndex);

						//je t1, 0, FALSE
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_JE);
						_IL::AddILCodeOprand_Variable(&compiler.functionTable,
							compiler.currentScope, instrIndex, compiler.tempVar1SymbolIndex);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 0);
						_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, 
							compiler.currentScope, instrIndex, falseJumpTargetIndex);

						//push 1
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_PUSH);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 1);

						//jmp EXIT
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_JMP);
						_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable,
							compiler.currentScope, instrIndex, exitJumpTargetIndex);
						 
						//FALSE: 
						_IL::AddILCodeJumpTarget(&compiler.functionTable,
							compiler.currentScope, falseJumpTargetIndex);

						//push 0
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_PUSH);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 0);

						//EXIT: 
						_IL::AddILCodeJumpTarget(&compiler.functionTable,
							compiler.currentScope, exitJumpTargetIndex); 

						/************************************************************************
						   The asm code generated by the above code:

						    pop t0
						    pop t1
						    je  t0, 0, FALSE
							je  t1, 0, FALSE
						    push 1
						    jmp EXIT
				          FALSE:
				            push 0
				          EXIT:

				       *************************************************************************/
						break;
					}

				case CL_OPERATOR_TYPE_LOGICAL_OR:
					{
						int trueJumpTargetIndex  = _IL::GetNextJumpTargetIndex();
						int exitJumpTargetIndex  = _IL::GetNextJumpTargetIndex();

						//jne t0, 0, TRUE
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_JNE);
						_IL::AddILCodeOprand_Variable(&compiler.functionTable,
							compiler.currentScope, instrIndex, compiler.tempVar0SymbolIndex);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 0);
						_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, 
							compiler.currentScope, instrIndex, trueJumpTargetIndex);

						//jne t1, 0, TRUE
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_JNE);
						_IL::AddILCodeOprand_Variable(&compiler.functionTable,
							compiler.currentScope, instrIndex, compiler.tempVar1SymbolIndex);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 0);
						_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, 
							compiler.currentScope, instrIndex, trueJumpTargetIndex);

						//push 0
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_PUSH);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 1);

						//jmp EXIT
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_JMP);
						_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable,
							compiler.currentScope, instrIndex, exitJumpTargetIndex);
						 
						//TRUE: 
						_IL::AddILCodeJumpTarget(&compiler.functionTable,
							compiler.currentScope, trueJumpTargetIndex);

						//push 1
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
							compiler.currentScope, IL_INSTR_PUSH);
						_IL::AddILCodeOprand_Int(&compiler.functionTable,
							compiler.currentScope, instrIndex, 1);

						//EXIT: 
						_IL::AddILCodeJumpTarget(&compiler.functionTable,
							compiler.currentScope, exitJumpTargetIndex); 

						/************************************************************************
						   The asm code generated by the above code:

						    pop t0
						    pop t1
						    jne t0, 0, TRUE
							jne t1, 0, TRUE
						    push 0
						    jmp EXIT
				          TRUE:
				            push 1
				          EXIT:

				       *************************************************************************/
						break;
					}

				default:
					break;
				}
			}
		}
	}
	void ParseSubExpression()
	{
		int instrIndex;
		int opType;

		ParseTerm();

		while (TRUE)
		{
			if ( GetNextToken()!= CL_TOKEN_TYPE_OPERATOR ||
				(GetCurrentOperator() != CL_OPERATOR_TYPE_ADD && 
				 GetCurrentOperator() != CL_OPERATOR_TYPE_SUB))
			{
				RewindTokenStream();
				break;
			}

			opType = GetCurrentOperator();

			ParseTerm();

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar1SymbolIndex);

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);

			int ILInstr;
			switch (opType)
			{
			case CL_OPERATOR_TYPE_ADD:
				ILInstr = IL_INSTR_ADD;
				break;
			case CL_OPERATOR_TYPE_SUB:
				ILInstr = IL_INSTR_SUB;
				break;
			default:
				break;
			}

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				ILInstr);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar1SymbolIndex);

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_PUSH);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);
		}
	}
	void ParseTerm()
	{
		int instrIndex;
		int opType;

		ParseFactor();

		while(TRUE)
		{
			if ( GetNextToken() != CL_TOKEN_TYPE_OPERATOR     ||
				(GetCurrentOperator() != CL_OPERATOR_TYPE_MUL && 
				 GetCurrentOperator() != CL_OPERATOR_TYPE_DIV &&
				 GetCurrentOperator() != CL_OPERATOR_TYPE_MOD &&
				 GetCurrentOperator() != CL_OPERATOR_TYPE_AND &&
				 GetCurrentOperator() != CL_OPERATOR_TYPE_OR  &&
				 GetCurrentOperator() != CL_OPERATOR_TYPE_SHIFT_LEFT &&
				 GetCurrentOperator() != CL_OPERATOR_TYPE_SHIFT_RIGHT))
			{
				RewindTokenStream();
				break;
			}

			opType = GetCurrentOperator();

			ParseFactor();

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar1SymbolIndex);

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex); 

			int ILInstr;
			switch (opType)
			{
			case CL_OPERATOR_TYPE_MUL:
				ILInstr = IL_INSTR_MUL;
				break;
			case CL_OPERATOR_TYPE_DIV:
				ILInstr = IL_INSTR_DIV;
				break;
			case CL_OPERATOR_TYPE_MOD:
				ILInstr = IL_INSTR_MOD;
				break;
			case CL_OPERATOR_TYPE_AND:
				ILInstr = IL_INSTR_AND;
				break;
			case CL_OPERATOR_TYPE_OR:
				ILInstr = IL_INSTR_OR;
				break;
			case CL_OPERATOR_TYPE_XOR:
				ILInstr = IL_INSTR_XOR;
				break;
			case CL_OPERATOR_TYPE_SHIFT_LEFT:
				ILInstr = IL_INSTR_SHL;
				break;
			case CL_OPERATOR_TYPE_SHIFT_RIGHT:
				ILInstr = IL_INSTR_SHR;
				break;
			default:
				break;
			}

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				ILInstr);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar1SymbolIndex);

			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_PUSH);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);
		}
	}
	void ParseFactor()
	{
		int instrIndex;
		int isUnaryOperatorExist = FALSE;
		int opType;

		if ( GetNextToken() == CL_TOKEN_TYPE_OPERATOR && 
			(GetCurrentOperator() == CL_OPERATOR_TYPE_ADD ||
			 GetCurrentOperator() == CL_OPERATOR_TYPE_SUB ||
			 GetCurrentOperator() == CL_OPERATOR_TYPE_NOT ||
			 GetCurrentOperator() == CL_OPERATOR_TYPE_LOGICAL_NOT ))
		{
			isUnaryOperatorExist = TRUE;
			opType = GetCurrentOperator();
		}
		else
		{
			RewindTokenStream();
		}

		switch (GetNextToken())
		{
		case CL_TOKEN_TYPE_KEYWORD_TRUE:
		case CL_TOKEN_TYPE_KEYWORD_FALSE:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_PUSH);
			_IL::AddILCodeOprand_Int(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.currLexerState.currentToken ==
				CL_TOKEN_TYPE_KEYWORD_TRUE ? 1 : 0);
			break;

		case CL_TOKEN_TYPE_INT:
			{

			
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_PUSH);
			int a = 	atoi(GetCurrentLexeme());
			_IL::AddILCodeOprand_Int(&compiler.functionTable, compiler.currentScope,
				instrIndex, a);
			break;
			}

		case CL_TOKEN_TYPE_FLOAT:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_PUSH);
			_IL::AddILCodeOprand_Float(&compiler.functionTable, compiler.currentScope,
				instrIndex, (float) atof(GetCurrentLexeme()));
			break;

		case CL_TOKEN_TYPE_STRING:
			{
				int stringIndex = AddString(&compiler.stringTable, GetCurrentLexeme());
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
					IL_INSTR_PUSH);
				_IL::AddILCodeOprand_String(&compiler.functionTable, compiler.currentScope,
					instrIndex, stringIndex);
				break;
			}

		case CL_TOKEN_TYPE_IDENT:
			{
				SymbolNode* pSymbol = GetSymbol(&compiler.symbolTable, GetCurrentLexeme(),
					compiler.currentScope);
				if (pSymbol) 
				{
					if (GetLookAheadChar() == '[')
					{
						//if (pSymbol->iSize == 1)
							//ExitOnCodeError("Invalid array");

						ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_BRACE);

						if (GetLookAheadChar() == ']')
							ExitOnCodeError("Invalid expression");

						ParseExpression();

						ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_BRACE);

						//pop t0
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
							IL_INSTR_POP);
						_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
							instrIndex, compiler.tempVar0SymbolIndex);

						//push t0
						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
							IL_INSTR_PUSH);
						_IL::AddILCodeOprand_RelArrayIndex(&compiler.functionTable, compiler.currentScope,
							instrIndex, pSymbol->iIndex, compiler.tempVar0SymbolIndex);
					}
					else
					{
						if (pSymbol->iSize == 1)
						{
							instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
								IL_INSTR_PUSH);
							_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
								instrIndex, pSymbol->iIndex); 
						}
						else
						{
							ExitOnCodeError("Array must be indexed");
						}
					}
				}
				else
				{
					if (GetFunction(&compiler.functionTable, GetCurrentLexeme()))
					{
						ParseFunctionCall();

						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
							IL_INSTR_PUSH);
						_IL::AddILCodeOprand_Reg(&compiler.functionTable, compiler.currentScope,
							instrIndex, REG_CODE_RETVAL);
					}
					else if (GetBuiltInFunc(&compiler.builInFuncTable, GetCurrentLexeme())) 
					{
						ParseBuildInFuncCall();

						instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
							IL_INSTR_PUSH);
						_IL::AddILCodeOprand_Reg(&compiler.functionTable, compiler.currentScope,
							instrIndex, REG_CODE_RETVAL);
					}
					else
					{
						ExitOnCodeError("non-defined function");
					}
				}
				break;
			}

		case CL_TOKEN_TYPE_DELIM_OPEN_PAREN:
			ParseExpression();
			ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_PAREN);
			break;

		default:
			ExitOnCodeError("Invalid input");
			break;
		}

		if (isUnaryOperatorExist)
		{
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);

			if (opType == CL_OPERATOR_TYPE_LOGICAL_NOT)
			{
				int trueJumpTargetIndex = _IL::GetNextJumpTargetIndex();
				int exitJumpTargetIndex  = _IL::GetNextJumpTargetIndex();

				//je t0, 0, TRUE
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_JE);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable,
					compiler.currentScope, instrIndex, compiler.tempVar0SymbolIndex);
				_IL::AddILCodeOprand_Int(&compiler.functionTable,
					compiler.currentScope, instrIndex, 0);
				_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, 
					compiler.currentScope, instrIndex, trueJumpTargetIndex);

				//push 0
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_PUSH);
				_IL::AddILCodeOprand_Int(&compiler.functionTable,
					compiler.currentScope, instrIndex, 0);

				//jmp EXIT
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_JMP);
				_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable,
					compiler.currentScope, instrIndex, exitJumpTargetIndex);
				 
				//TRUE: 
				_IL::AddILCodeJumpTarget(&compiler.functionTable,
					compiler.currentScope, trueJumpTargetIndex);

				//push 1
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable,
					compiler.currentScope, IL_INSTR_PUSH);
				_IL::AddILCodeOprand_Int(&compiler.functionTable,
					compiler.currentScope, instrIndex, 1);

				//EXIT: 
				_IL::AddILCodeJumpTarget(&compiler.functionTable,
					compiler.currentScope, exitJumpTargetIndex); 

				/************************************************************************
				   The asm code generated by the above code:

				      pop t0
				      je  t0, 0, FALSE
				      push 0
				      jmp EXIT
				    TRUE:
				      push 1
				    EXIT:

				 ************************************************************************/
			}
			else
			{
				int ILInstr;
				switch (opType)
				{
				case CL_OPERATOR_TYPE_SUB:
					ILInstr = IL_INSTR_NEG;
					break;
				case CL_OPERATOR_TYPE_NOT:
					ILInstr = IL_INSTR_NOT;
					break;
				default:
					break;
				}

				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
					ILInstr);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
					instrIndex, compiler.tempVar0SymbolIndex);

				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
					IL_INSTR_PUSH);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
					instrIndex, compiler.tempVar0SymbolIndex);
			}
		}
	}

	void ParseIf()
	{
		int instrIndex;

		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("if illegal in global scope");

		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope, GetCurrentSourceLine());

		int falseJumpTargetIndex = _IL::GetNextJumpTargetIndex();

		ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_PAREN);

		ParseExpression();

		ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_PAREN);

		//pop t0
		instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_POP);
		_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope, 
			instrIndex, compiler.tempVar0SymbolIndex);

		//je t0, 0, FALSE
		instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_JE);
		_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
			instrIndex, compiler.tempVar0SymbolIndex);
		_IL::AddILCodeOprand_Int(&compiler.functionTable, compiler.currentScope,
			instrIndex, 0);
		_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, compiler.currentScope,
			instrIndex, falseJumpTargetIndex);

		ParseStatement();

		if (GetNextToken() == CL_TOKEN_TYPE_KEYWORD_ELSE)
		{
			int skipFalseJumpTargetIndex = _IL::GetNextJumpTargetIndex();
			//jmp SKIP_FALSE
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_JMP);
			_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, compiler.currentScope, 
				instrIndex, skipFalseJumpTargetIndex);

			//FALSE:
			_IL::AddILCodeJumpTarget(&compiler.functionTable, compiler.currentScope,
				falseJumpTargetIndex);

			ParseStatement();

			//SKIP_FALSE:
			_IL::AddILCodeJumpTarget(&compiler.functionTable, compiler.currentScope,
				skipFalseJumpTargetIndex);
		}
		else
		{
			RewindTokenStream();

			//FALSE:
			_IL::AddILCodeJumpTarget(&compiler.functionTable, compiler.currentScope,
				falseJumpTargetIndex);
		}
	}
	void ParseWhile()
	{
		int instrIndex;

		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("while illega in global scope");

		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope,
			GetCurrentSourceLine());

		int startTargetIndex = _IL::GetNextJumpTargetIndex();
		int endTargetIndex = _IL::GetNextJumpTargetIndex();

		_IL::AddILCodeJumpTarget(&compiler.functionTable, compiler.currentScope, 
			startTargetIndex);

		ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_PAREN);

		ParseExpression();

		ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_PAREN);

		//pop t0
		instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_POP);
		_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope, 
			instrIndex, compiler.tempVar0SymbolIndex);

		//je t0, 0, END
		instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_JE);
		_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
			instrIndex, compiler.tempVar0SymbolIndex);
		_IL::AddILCodeOprand_Int(&compiler.functionTable, compiler.currentScope,
			instrIndex, 0);
		_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, compiler.currentScope,
			instrIndex, endTargetIndex); 

		Loop* loop = (Loop*) malloc(sizeof(Loop));
		loop->iStartTargetIndex = startTargetIndex;
		loop->iEndTargetIndex   = endTargetIndex;

		Push(&compiler.loopStack, loop);

		ParseStatement();

		Pop(&compiler.loopStack);

		//jmp START
		instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_JMP);
		_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, compiler.currentScope,
			instrIndex, startTargetIndex);

		//END:
		_IL::AddILCodeJumpTarget(&compiler.functionTable, compiler.currentScope,
			endTargetIndex);
	}
	void ParseFor()
	{
		int instrIndex;

		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("for illegal in global scope");

		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope,
			GetCurrentSourceLine());
	}
	void ParseBreak()
	{
		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("break illegal in global scope");

		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope,
			GetCurrentSourceLine());

		ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);

		int targetIndex = ((Loop*)Peek(&compiler.loopStack))->iEndTargetIndex;

		int instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_JMP);
		_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, compiler.currentScope,
			instrIndex, targetIndex);
	}
	void ParseContinue()
	{
		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("continue illegal in global scope");

		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope,
			GetCurrentSourceLine());

		ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);

		int targetIndex = ((Loop*)Peek(&compiler.loopStack))->iStartTargetIndex;

		int instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_JMP);
		_IL::AddILCodeOprand_JumpTarget(&compiler.functionTable, compiler.currentScope,
			instrIndex, targetIndex);
	}
	void ParseReturn()
	{
		int instrIndex;

		if (compiler.currentScope == SCOPE_GLOBAL)
			ExitOnCodeError("return illegal in global scope");

		_IL::AddILCodeSourceLine(&compiler.functionTable, compiler.currentScope,
			GetCurrentSourceLine());

		if (GetLookAheadChar() != ';')
		{
			ParseExpression();

			if (compiler.scriptHeader.iIsMainFuncExist &&
				compiler.scriptHeader.iMainFuncIndex == compiler.currentScope)
			{
				//pop t0
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
					IL_INSTR_POP);
				_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
					instrIndex, compiler.tempVar0SymbolIndex);
			}
			else
			{
				//pop t0
				instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
					IL_INSTR_POP);
				_IL::AddILCodeOprand_Reg(&compiler.functionTable, compiler.currentScope,
					instrIndex, REG_CODE_RETVAL);
			}
		}
		else
		{
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_MOV);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);
			_IL::AddILCodeOprand_Int(&compiler.functionTable, compiler.currentScope,
				instrIndex, 0);
		}

		if (compiler.scriptHeader.iIsMainFuncExist &&
			compiler.scriptHeader.iMainFuncIndex == compiler.currentScope)
		{
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_EXIT);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar0SymbolIndex);
		}
		else
		{
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
				IL_INSTR_RET);
		}

		ReadToken(CL_TOKEN_TYPE_DELIM_SEMICOLON);
	}
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
			//if (pSymbol->iSize == 1)
				//ExitOnCodeError("Invalid array");

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

		if (isArray)
		{
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope, IL_INSTR_POP);
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
				instrIndex, compiler.tempVar1SymbolIndex);
		}

		switch (assignOp)
		{
		case CL_OPERATOR_TYPE_ASSIGN:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_MOV);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_ADD:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_ADD);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_SUB:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_SUB);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_MUL:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_MUL);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_DIV:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_DIV);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_MOD:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_MOD);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_AND:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_AND);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_OR:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_OR);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_XOR:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_XOR);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_SHIFT_LEFT:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_SHL);
			break;
		case CL_OPERATOR_TYPE_ASSIGN_SHIFT_RIGHT:
			instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, 
				compiler.currentScope, IL_INSTR_SHR);
			break;
		default:
			break;
		}

		if (isArray)
			_IL::AddILCodeOprand_RelArrayIndex(&compiler.functionTable, compiler.currentScope,
				instrIndex, pSymbol->iIndex, compiler.tempVar1SymbolIndex);
		else
			_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope, 
				instrIndex, pSymbol->iIndex);

		_IL::AddILCodeOprand_Variable(&compiler.functionTable, compiler.currentScope,
			instrIndex, compiler.tempVar0SymbolIndex);
		
	}
	void ParseFunctionCall()
	{
		_cl::FuncNode* pFunction = _cl::GetFunction(&compiler.functionTable, GetCurrentLexeme());
		
		int paramCount = 0;

		ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_PAREN);

		while(TRUE) 
		{
			if (GetLookAheadChar() != ')')
			{
				ParseExpression();

				paramCount++;

				if (!pFunction->isHostAPI && paramCount > pFunction->iParamCount)
					ExitOnCodeError("Too many parameters");

				if (GetLookAheadChar() != ')')
					ReadToken(CL_TOKEN_TYPE_DELIM_COMMA);
			}
			else
			{
				break;
			}
		}

		ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_PAREN);

		if (!pFunction->isHostAPI && paramCount < pFunction->iParamCount)
			ExitOnCodeError("Too few parameters");

		int callInstr = IL_INSTR_CALL;
		if (pFunction->isHostAPI)
			callInstr = IL_INSTR_CALLHOST;

		//call (func name)
		int instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			callInstr);
		_IL::AddILCodeOprand_Func(&compiler.functionTable, compiler.currentScope,
			instrIndex, pFunction->iIndex);
	}
	void ParseBuildInFuncCall()
	{
		BuiltInFunc* pFunction = GetBuiltInFunc(&compiler.builInFuncTable, GetCurrentLexeme());

		int paramCount = 0;

		ReadToken(CL_TOKEN_TYPE_DELIM_OPEN_PAREN);

		while(TRUE) 
		{
			if (GetLookAheadChar() != ')')
			{
				ParseExpression();

				paramCount++;

				if (paramCount > pFunction->iParamCount)
					ExitOnCodeError("Too many parameters");

				if (GetLookAheadChar() != ')')
					ReadToken(CL_TOKEN_TYPE_DELIM_COMMA);
			}
			else
			{
				break;
			}
		}

		ReadToken(CL_TOKEN_TYPE_DELIM_CLOSE_PAREN);

		if (paramCount < pFunction->iParamCount)
			ExitOnCodeError("Too few parameters");

		//call (func name)
		int instrIndex = _IL::AddILCodeInstr(&compiler.functionTable, compiler.currentScope,
			IL_INSTR_CALLHOST);
		_IL::AddILCodeOprand_BuiltInFunc(&compiler.functionTable, compiler.currentScope,
			instrIndex, pFunction->iIndex);
	}

}