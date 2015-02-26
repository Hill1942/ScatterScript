#include <stdlib.h>
#include <string.h>

#include "ssutil.h"
#include "ssbase_type.h"
#include "sstype.h"
#include "sslexeme.h"
#include "ssasm.h"

extern LinkList g_FunctionTable;
extern LinkList g_StringTable;
extern LinkList g_LabelTable;
extern LinkList g_SymbolTable;
extern LinkList g_HostAPICallTable;

extern Lexer  g_Lexer;
extern ScriptHeader g_ScriptHeader;
extern Instr* g_InstrStream;

extern int g_InstrStreamSize;
extern int g_IsSetStackSizeFound;
extern int g_currentInstrIndex;

extern InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

void AssembleSourceFile()
{
    g_ScriptHeader.iStackSize = 0;
    g_ScriptHeader.iIsMainFuncPresent = FALSE;

    g_InstrStreamSize = 0;
    g_IsSetStackSizeFound = FALSE;
    g_ScriptHeader.iGlobalDataSize = 0;

    ResetLexer();

	int isInFunction = FALSE;
	int currentFuncIndex = 0;
	int currentFuncParamCount = 0;
	int currentFuncLocalDataSize = 0;
	char pCurrentFuncName[MAX_INDENT_SIZE];
	FuncNode* pCurrentFunc;
	InstrLookup currentInstr;

    while (TRUE)
    {
        if (GetNextToken() == END_OF_TOKEN_STREAM)
            break;

        switch (g_Lexer.currentToken)
        {
            case TOKEN_TYPE_SETSTACKSIZE:
				{
					if (isInFunction)
						ExitOnCodeError(ERROR_MSG_LOCAL_SETSTACKSIZE);
					if (g_IsSetStackSizeFound)
						ExitOnCodeError(ERROR_MSG_MULTIPLE_SETSTACKSIZE);
					if (GetNextToken() != TOKEN_TYPE_INT)
						ExitOnCodeError(ERROR_MSG_INVALID_STACK_SIZE);

					g_ScriptHeader.iStackSize = atoi(GetCurrentLexeme());
					break;
				}
				

			case TOKEN_TYPE_VAR:
				{
					if (GetNextToken() != TOKEN_TYPE_IDENT)
						ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);

					char pIdentifier[MAX_INDENT_SIZE];
					strcpy(pIdentifier, GetCurrentLexeme());

					int size = 1;
					if (GetLookAheadChar() == '[') 
					{
						if (GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
							ExitOnCharExpectedError('[');
						if (GetNextToken() != TOKEN_TYPE_INT)
							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_SIZE);

						size = atoi(GetCurrentLexeme());

						if (size <= 0)
							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_SIZE);

						if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
							ExitOnCharExpectedError(']');
					}

					int stackIndex;
					if (isInFunction)
						stackIndex = -(currentFuncLocalDataSize + 2);
					else
						stackIndex = g_ScriptHeader.iGlobalDataSize;

					if (AddSymbol(pIdentifier, size, stackIndex, currentFuncIndex) == -1)
						ExitOnCodeError(ERROR_MSG_IDENT_REDEFINITION);

					if (isInFunction)
						currentFuncLocalDataSize += size;
					else 
						g_ScriptHeader.iGlobalDataSize += size;

					break;
				}

			case TOKEN_TYPE_FUNC:
				{
					if (isInFunction)
						ExitOnCodeError(ERROR_MSG_NESTED_FUNC);

					if (GetNextToken() != TOKEN_TYPE_IDENT)
						ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);

					char* pFuncName = GetCurrentLexeme();

					int entryPoint = g_InstrStreamSize;
					int funcIndex = AddFunction(pFuncName, entryPoint);
					if (funcIndex == -1)
						ExitOnCodeError(ERROR_MSG_FUNC_REDEFINITION);
					if (strcmp(pFuncName, MAIN_FUNC_NAME) == 0)
					{
						g_ScriptHeader.iIsMainFuncPresent = TRUE;
						g_ScriptHeader.iMainFuncIndex = funcIndex;
					}

					isInFunction = TRUE;
					strcpy(pCurrentFuncName, pFuncName);
					currentFuncIndex = funcIndex;
					currentFuncParamCount = 0;
					currentFuncLocalDataSize = 0;

					while (GetNextToken() == TOKEN_TYPE_NEWLINE);

					if (g_Lexer.currentToken != TOKEN_TYPE_OPEN_BRACE)
						ExitOnCharExpectedError('{');

					g_InstrStreamSize++;
					break;
				}

			case TOKEN_TYPE_CLOSE_BRACE:
				{
					if (!isInFunction)
						ExitOnCharExpectedError('}');

					SetFunctionInfo(pCurrentFuncName, currentFuncParamCount, currentFuncLocalDataSize);
					isInFunction = FALSE;
					break;
				}

			case TOKEN_TYPE_PARAM:
				{
					if (!isInFunction)
						ExitOnCodeError(ERROR_MSG_GLOBAL_PARAM);

					if (strcmp(pCurrentFuncName, MAIN_FUNC_NAME) == 0)
						ExitOnCodeError(ERROR_MSG_MAIN_PARAM);

					if (GetNextToken() != TOKEN_TYPE_IDENT)
						ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);

					currentFuncParamCount++;

					break;
				}

			case TOKEN_TYPE_INSTR:
				{
					if (!isInFunction)
						ExitOnCodeError(ERROR_MSG_GLOBAL_INSTR);

					g_InstrStreamSize++;

					break;
				}

			case TOKEN_TYPE_IDENT:        //line label
				{
					if (GetLookAheadChar() != ':')
						ExitOnCodeError(ERROR_MSG_INVALID_INSTR);

					if (!isInFunction)
						ExitOnCodeError(ERROR_MSG_GLOBAL_LINE_LABEL);

					char* pIdentifier = GetCurrentLexeme();
					int   targetIndex = g_InstrStreamSize - 1;
					int   funcIndex   = currentFuncIndex;

					if (AddLabel(pIdentifier, targetIndex, funcIndex) == -1)
						ExitOnCodeError(ERROR_MSG_LINE_LABEL_REDEFINITION);

					break;
				}

			default:
				{
					if (g_Lexer.currentToken != TOKEN_TYPE_NEWLINE)
						ExitOnCodeError(ERROR_MSG_INVALID_INPUT);
				}
        }

		if (!SkipToNextLine())
			break;
    }

	g_InstrStream = (Instr*)malloc(g_InstrStreamSize * sizeof(Instr));

	for (int i = 0; i < g_InstrStreamSize; i++)
		g_InstrStream[i].pOplist = NULL;

	g_currentInstrIndex = 0;
	ResetLexer();

	while (TRUE)
	{
		if (GetNextToken() == END_OF_TOKEN_STREAM)
			break;

		switch (g_Lexer.currentToken)
		{
		case TOKEN_TYPE_FUNC:
			{
				GetNextToken();
				pCurrentFunc = GetFunctionByName(GetCurrentLexeme());

				isInFunction = TRUE;
				currentFuncParamCount = 0;
				currentFuncIndex = pCurrentFunc->iIndex;

				while (GetNextToken() == TOKEN_TYPE_NEWLINE);
				break;
			}

		case TOKEN_TYPE_CLOSE_BRACE:
			{
				isInFunction = FALSE;
				if (strcmp(pCurrentFunc->strName, MAIN_FUNC_NAME) == 0)
				{
					g_InstrStream[g_currentInstrIndex].iOpcode = INSTR_EXIT;
					g_InstrStream[g_currentInstrIndex].iOpCount = 1;
					g_InstrStream[g_currentInstrIndex].pOplist = (Op*)malloc(sizeof(Op));
					g_InstrStream[g_currentInstrIndex].pOplist[0].iType = OP_TYPE_INT;
					g_InstrStream[g_currentInstrIndex].pOplist[0].iIntLiteral = 0;
				}
				else
				{
					g_InstrStream[g_currentInstrIndex].iOpcode = INSTR_RET;
					g_InstrStream[g_currentInstrIndex].iOpCount = 0;
					g_InstrStream[g_currentInstrIndex].pOplist = NULL;
				}
				g_currentInstrIndex++;
				break;
			}

		case TOKEN_TYPE_PARAM:
			{
				if (GetNextToken() != TOKEN_TYPE_IDENT)
					ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);

				char* pIdentifier = GetCurrentLexeme();
				int   stackIndex  = -(pCurrentFunc->iLocalDataSize + 2 +
					                  currentFuncParamCount + 1);
				if (AddSymbol(pIdentifier, 1, stackIndex, currentFuncIndex) == -1)
					ExitOnCodeError(ERROR_MSG_IDENT_REDEFINITION);

				currentFuncParamCount++;
				break;
			}

		case TOKEN_TYPE_INSTR:
			{
				GetInstrByMnemonic(GetCurrentLexeme(), &currentInstr);
				g_InstrStream[g_currentInstrIndex].iOpcode = currentInstr.iOpcode;
				g_InstrStream[g_currentInstrIndex].iOpCount = currentInstr.iOpcount;

				Op* pOpList = (Op*) malloc(currentInstr.iOpcount * sizeof(Op));

				for (int i = 0; i < currentInstr.iOpcount; i++)
				{
					OpType currentOpType = currentInstr.pOplist[i];
					Token initOpToken = GetNextToken();
					switch (initOpToken)
					{
					case TOKEN_TYPE_INT:
						{
							if (currentOpType & OP_FLAG_TYPE_INT)
							{
								pOpList[i].iType = OP_TYPE_INT;
								pOpList[i].iIntLiteral = atoi(GetCurrentLexeme());
							}
							else
							{
								ExitOnCodeError(ERROR_MSG_INVALID_OP);
							}
							break;
						}
					case TOKEN_TYPE_FLOAT:
						{
							if (currentOpType & OP_FLAG_TYPE_FLOAT)
							{
								pOpList[i].iType = OP_TYPE_FLOAT;
								pOpList[i].fFloatLiteral = atof(GetCurrentLexeme());
							}
							else
							{
								ExitOnCodeError(ERROR_MSG_INVALID_OP);
							}
							break;
						}
					case TOKEN_TYPE_QUATE:
						{
							if (currentOpType & OP_FLAG_TYPE_STRING)
							{
								GetNextToken();

								switch(g_Lexer.currentToken)
								{
								case TOKEN_TYPE_QUATE:
									{
										//empty string, convert it to the integer 0
										pOpList[i].iType = OP_TYPE_INT;
										pOpList[i].iIntLiteral = 0;
										break;
									}
								case TOKEN_TYPE_STRING:
									{
										char* str = GetCurrentLexeme();
										int   strIndex = AddString(&g_StringTable, str);
										if (GetNextToken() != TOKEN_TYPE_QUATE)
											ExitOnCharExpectedError('\\');

										pOpList[i].iType = OP_TYPE_STRING_INDEX;
										pOpList[i].iStringTableIndex = strIndex;
										break;
									}
								default:
									ExitOnCodeError(ERROR_MSG_INVALID_STRING);
								}
							}
							else 
							{
								ExitOnCodeError(ERROR_MSG_INVALID_OP);
							}
							break;
						}
					case TOKEN_TYPE_REG_RETVAL:
						{
							if (currentOpType & OP_FLAG_TYPE_REG)
							{
								pOpList[i].iType = OP_TYPE_REG;
								pOpList[i].iReg  = 0;
							} 
							else
							{
								ExitOnCodeError(ERROR_MSG_INVALID_OP);
							}
							break;
						}
					case TOKEN_TYPE_IDENT:
						{
							if (currentOpType & OP_FLAG_TYPE_MEM_REF)
							{
								char pIdentifier[MAX_INDENT_SIZE];
								strcpy(pIdentifier, GetCurrentLexeme());
								if (!GetSymbolByIdent(pIdentifier, currentFuncIndex))
									ExitOnCodeError(ERROR_MSG_UNDEFINED_IDENT);

								int baseIndex = GetStackIndexByIdent(pIdentifier, currentFuncIndex);
								if (GetLookAheadChar() != '[')
								{
									if (GetSizeByIdent(pIdentifier, currentFuncIndex) > 1)
										ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_NOT_INDEXED);
									pOpList[i].iType = OP_TYPE_ABS_STACK_INDEX;
									pOpList[i].iIntLiteral = baseIndex;
								}
								else
								{
									if (GetSizeByIdent(pIdentifier, currentFuncIndex) == 1)
										ExitOnCodeError(ERROR_MSG_INVALID_ARRAY);
									if (GetNextToken() != TOKEN_TYPE_OPEN_BRACKET)
										ExitOnCharExpectedError('[');

									Token indexToken = GetNextToken();
									if (indexToken == TOKEN_TYPE_INT)
									{
										int offsetIndex = atoi(GetCurrentLexeme());
										pOpList[i].iType = OP_TYPE_ABS_STACK_INDEX;
										pOpList[i].iStackIndex = baseIndex + offsetIndex;
									} 
									else if (indexToken == TOKEN_TYPE_IDENT)
									{
										char* pStrIndexIdent = GetCurrentLexeme();
										if (!GetSymbolByIdent(pStrIndexIdent, currentFuncIndex))
											ExitOnCodeError(ERROR_MSG_UNDEFINED_IDENT);
										if (GetSizeByIdent(pStrIndexIdent, currentFuncIndex) > 1)
											ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);

										int offsetIndex = GetStackIndexByIdent(pStrIndexIdent, currentFuncIndex);
										pOpList[i].iType = OP_TYPE_REL_STACK_INDEX;
										pOpList[i].iStackIndex = baseIndex;
										pOpList[i].iOffsetIndex = offsetIndex;
									}
									else
									{
										ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
									}
									if (GetNextToken() != TOKEN_TYPE_CLOSE_BRACKET)
										ExitOnCharExpectedError('[');
								}
							}

							if (currentOpType & OP_FLAG_TYPE_LINE_LABEL)
							{
								char* pLabelIdent = GetCurrentLexeme();
								LabelNode* pLabel = GetLabelByIdent(pLabelIdent, currentFuncIndex);
								if (!pLabel)
									ExitOnCodeError(ERROR_MSG_UNDEFINED_LINE_LABEL);

								pOpList[i].iType = OP_TYPE_INSTR_INDEX;
								pOpList[i].iInstrIndex = pLabel->iTargetIndex;
							}

							if (currentOpType & OP_FLAG_TYPE_FUNC_NAME)
							{
								char* strFuncName = GetCurrentLexeme();
								FuncNode* pFunc = GetFunctionByName(strFuncName);
								if (!pFunc)
									ExitOnCodeError(ERROR_MSG_UNDEFINED_FUNC);

								pOpList[i].iType = OP_TYPE_FUNC_INDEX;
								pOpList[i].iFuncIndex = pFunc->iIndex;
							}

							if (currentOpType & OP_FLAG_TYPE_HOST_API_CALL)
							{
								char* strHostAPICall = GetCurrentLexeme();
								int   index = AddString(&g_HostAPICallTable, strHostAPICall);

								pOpList[i].iType = OP_TYPE_HOST_API_CALL_INDEX;
								pOpList[i].iHostAPICallIndex = index;
							}
							break;
						}
					default:
						ExitOnCodeError(ERROR_MSG_INVALID_OP);
						break;
					}
					if (i < currentInstr.iOpcount - 1)
						if (GetNextToken() != TOKEN_TYPE_COMMA)
							ExitOnCharExpectedError(',');
				}
				
				if (GetNextToken() != TOKEN_TYPE_NEWLINE)
					ExitOnCodeError(ERROR_MSG_INVALID_INPUT);

				g_InstrStream[g_currentInstrIndex].pOplist = pOpList;

				g_currentInstrIndex++;

				break;
			}
		}

		if (!SkipToNextLine())
			break;
	}
}
















