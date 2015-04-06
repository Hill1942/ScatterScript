#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "sspre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sssystem.h"
#include "sslexeme.h"
#include "ssasm.h"

extern _asm_::ASM sasm;

namespace _asm_ 
{
    void AssembleSourceFile()
    {
		sasm.header.iStackSize = 0;
        sasm.header.iIsMainFuncPresent = FALSE;
    	sasm.header.iGlobalDataSize = 0;

        sasm.instrStreamSize = 0;
        sasm.isSetStackSizeFound = FALSE;
        
        ResetLexer();
    
    	int isInFunction = FALSE;
    	int currentFuncIndex = 0;
    	int currentFuncParamCount = 0;
    	int currentFuncLocalDataSize = 0;
    	char pCurrentFuncName[MAX_INDENT_SIZE];
    	FuncNode* pCurrentFunc = NULL;
    	InstrLookup currentInstr;
    
        while (TRUE)
        {
            if (GetNextToken() == ASM_END_OF_TOKEN_STREAM)
                break;
    
            switch (sasm.lexer.currentToken)
            {
                case ASM_TOKEN_TYPE_SETSTACKSIZE:
    				{
    					if (isInFunction)
    						ExitOnCodeError(ERROR_MSG_LOCAL_SETSTACKSIZE);
    					if (sasm.isSetStackSizeFound)
    						ExitOnCodeError(ERROR_MSG_MULTIPLE_SETSTACKSIZE);
    					if (GetNextToken() != ASM_TOKEN_TYPE_INT)
    						ExitOnCodeError(ERROR_MSG_INVALID_STACK_SIZE);
    
    					sasm.header.iStackSize = atoi(GetCurrentLexeme());
    					break;
    				}
    				
    
    			case ASM_TOKEN_TYPE_VAR:
    				{
    					if (GetNextToken() != ASM_TOKEN_TYPE_IDENT)
    						ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
    
    					char pIdentifier[MAX_INDENT_SIZE];
    					strcpy(pIdentifier, GetCurrentLexeme());
    
    					int size = 1;
    					if (GetLookAheadChar() == '[') 
    					{
    						if (GetNextToken() != ASM_TOKEN_TYPE_OPEN_BRACKET)
    							ExitOnCharExpectedError('[');
    						if (GetNextToken() != ASM_TOKEN_TYPE_INT)
    							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_SIZE);
    
    						size = atoi(GetCurrentLexeme());
    
    						if (size <= 0)
    							ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_SIZE);
    
    						if (GetNextToken() != ASM_TOKEN_TYPE_CLOSE_BRACKET)
    							ExitOnCharExpectedError(']');
    					}
    
    					int stackIndex;
    					if (isInFunction)
    						stackIndex = -(currentFuncLocalDataSize + 2);
    					else
    						stackIndex = sasm.header.iGlobalDataSize;
    
    					if (AddSymbol(&sasm.symbolTable, pIdentifier, size, stackIndex, currentFuncIndex) == -1)
    						ExitOnCodeError(ERROR_MSG_IDENT_REDEFINITION);
    
    					if (isInFunction)
    						currentFuncLocalDataSize += size;
    					else 
    						sasm.header.iGlobalDataSize += size;
    
    					break;
    				}
    
    			case ASM_TOKEN_TYPE_FUNC:
    				{
    					if (isInFunction)
    						ExitOnCodeError(ERROR_MSG_NESTED_FUNC);
    
    					if (GetNextToken() != ASM_TOKEN_TYPE_IDENT)
    						ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
    
    					char* pFuncName = GetCurrentLexeme();
    
    					int entryPoint = sasm.instrStreamSize;
						int funcIndex = AddFunction(&sasm.functionTable, pFuncName, entryPoint);
    					if (funcIndex == -1)
    						ExitOnCodeError(ERROR_MSG_FUNC_REDEFINITION);
    					if (strcmp(pFuncName, ASM_KW_MAIN_FUNC_NAME) == 0)
    					{
    						sasm.header.iIsMainFuncPresent = TRUE;
    						sasm.header.iMainFuncIndex = funcIndex;
    					}
    
    					isInFunction = TRUE;
    					strcpy(pCurrentFuncName, pFuncName);
    					currentFuncIndex = funcIndex;
    					currentFuncParamCount = 0;
    					currentFuncLocalDataSize = 0;
    
    					while (GetNextToken() == ASM_TOKEN_TYPE_NEWLINE);
    
    					if (sasm.lexer.currentToken != ASM_TOKEN_TYPE_OPEN_BRACE)
    						ExitOnCharExpectedError('{');
    
    					//sasm.instrStreamSize++;
    					break;
    				}
    
    			case ASM_TOKEN_TYPE_CLOSE_BRACE:
    				{
    					if (!isInFunction)
    						ExitOnCharExpectedError('}');
    
    					SetFunctionInfo(pCurrentFuncName, currentFuncParamCount, currentFuncLocalDataSize);
    					isInFunction = FALSE;
    					break;
    				}
    
    			case ASM_TOKEN_TYPE_PARAM:
    				{
    					if (!isInFunction)
    						ExitOnCodeError(ERROR_MSG_GLOBAL_PARAM);
    
    					if (strcmp(pCurrentFuncName, ASM_KW_MAIN_FUNC_NAME) == 0)
    						ExitOnCodeError(ERROR_MSG_MAIN_PARAM);
    
    					if (GetNextToken() != ASM_TOKEN_TYPE_IDENT)
    						ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
    
    					currentFuncParamCount++;
    
    					break;
    				}
    
    			case ASM_TOKEN_TYPE_INSTR:
    				{
    					if (!isInFunction)
    						ExitOnCodeError(ERROR_MSG_GLOBAL_INSTR);
    
    					sasm.instrStreamSize++;
    
    					break;
    				}
    
    			case ASM_TOKEN_TYPE_IDENT:        //line label
    				{
    					if (GetLookAheadChar() != ':')
    						ExitOnCodeError(ERROR_MSG_INVALID_INSTR);
    
    					if (!isInFunction)
    						ExitOnCodeError(ERROR_MSG_GLOBAL_LINE_LABEL);
    
    					char* pIdentifier = GetCurrentLexeme();
    					//int   targetIndex = sasm.instrStreamSize - 1;
						int   targetIndex = sasm.instrStreamSize;
    					int   funcIndex   = currentFuncIndex;
    
    					if (AddLabel(&sasm.labelTable, pIdentifier, targetIndex, funcIndex) == -1)
    						ExitOnCodeError(ERROR_MSG_LINE_LABEL_REDEFINITION);
    
    					break;
    				}
    
    			default:
    				{
    					if (sasm.lexer.currentToken != ASM_TOKEN_TYPE_NEWLINE)
    						ExitOnCodeError(ERROR_MSG_INVALID_INPUT);
    				}
            }
    
    		if (!SkipToNextLine())
    			break;
        }
    
    	sasm.instrStream = (Instruction*)malloc(sasm.instrStreamSize * sizeof(Instruction));
    
    	for (int i = 0; i < sasm.instrStreamSize; i++)
    		sasm.instrStream[i].pOplist = NULL;
    
    	sasm.currentInstrIndex = 0;
    	ResetLexer();
    
    	while (TRUE)
    	{
    		if (GetNextToken() == ASM_END_OF_TOKEN_STREAM)
    			break;
    
    		switch (sasm.lexer.currentToken)
    		{
    		case ASM_TOKEN_TYPE_FUNC:
    			{
    				GetNextToken();
    				pCurrentFunc = GetFunctionByName(&sasm.functionTable, GetCurrentLexeme());
    
    				isInFunction = TRUE;
    				currentFuncParamCount = 0;
    				currentFuncIndex = pCurrentFunc->iIndex;
    
    				while (GetNextToken() == ASM_TOKEN_TYPE_NEWLINE);
    				break;
    			}
    
    		case ASM_TOKEN_TYPE_CLOSE_BRACE:
    			{
    				isInFunction = FALSE;
    				/*if (strcmp(pCurrentFunc->strName, ASM_KW_MAIN_FUNC_NAME) == 0)
    				{
    					sasm.instrStream[sasm.currentInstrIndex].iOpcode = ASM_INSTR_EXIT;
    					sasm.instrStream[sasm.currentInstrIndex].iOpCount = 1;
    					sasm.instrStream[sasm.currentInstrIndex].pOplist = (Oprand*)malloc(sizeof(Oprand));
    					sasm.instrStream[sasm.currentInstrIndex].pOplist[0].iType = ASM_OPRAND_TYPE_INT;
    					sasm.instrStream[sasm.currentInstrIndex].pOplist[0].iIntLiteral = 0;
    				}
    				else
    				{
    					sasm.instrStream[sasm.currentInstrIndex].iOpcode = ASM_INSTR_RET;
    					sasm.instrStream[sasm.currentInstrIndex].iOpCount = 0;
    					sasm.instrStream[sasm.currentInstrIndex].pOplist = NULL;
    				}
    				sasm.currentInstrIndex++;*/
    				break;
    			}
    
    		case ASM_TOKEN_TYPE_PARAM:
    			{
    				if (GetNextToken() != ASM_TOKEN_TYPE_IDENT)
    					ExitOnCodeError(ERROR_MSG_IDENT_EXPECTED);
    
    				char* pIdentifier = GetCurrentLexeme();
    				int   stackIndex  = -(pCurrentFunc->iLocalDataSize + 2 +
    					                  currentFuncParamCount + 1);
    				if (AddSymbol(&sasm.symbolTable, pIdentifier, 1, stackIndex, currentFuncIndex) == -1)
    					ExitOnCodeError(ERROR_MSG_IDENT_REDEFINITION);
    
    				currentFuncParamCount++;
    				break;
    			}
    
    		case ASM_TOKEN_TYPE_INSTR:
    			{
    				GetInstruction(sasm.instrLookup, GetCurrentLexeme(), &currentInstr);
    				sasm.instrStream[sasm.currentInstrIndex].iOpcode = currentInstr.iOpcode;
    				sasm.instrStream[sasm.currentInstrIndex].iOpCount = currentInstr.iOpcount;
    
    				Oprand* pOpList = (Oprand*) malloc(currentInstr.iOpcount * sizeof(Oprand));
    
    				for (int i = 0; i < currentInstr.iOpcount; i++)
    				{
    					OpType currentOpType = currentInstr.pOplist[i];
    					Token initOpToken = GetNextToken();
    					switch (initOpToken)
    					{
    					case ASM_TOKEN_TYPE_INT:
    						{
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_INT)
    							{
    								pOpList[i].iType = ASM_OPRAND_TYPE_INT;
    								pOpList[i].iIntLiteral = atoi(GetCurrentLexeme());
    							}
    							else
    							{
    								ExitOnCodeError(ERROR_MSG_INVALID_OP);
    							}
    							break;
    						}
    					case ASM_TOKEN_TYPE_FLOAT:
    						{
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_FLOAT)
    							{
    								pOpList[i].iType = ASM_OPRAND_TYPE_FLOAT;
    								pOpList[i].fFloatLiteral = atof(GetCurrentLexeme());
    							}
    							else
    							{
    								ExitOnCodeError(ERROR_MSG_INVALID_OP);
    							}
    							break;
    						}
    					case ASM_TOKEN_TYPE_QUATE:
    						{
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_STRING)
    							{
    								GetNextToken();
    
    								switch(sasm.lexer.currentToken)
    								{
    								case ASM_TOKEN_TYPE_QUATE:
    									{
    										//empty string, convert it to the integer 0
    										pOpList[i].iType = ASM_OPRAND_TYPE_INT;
    										pOpList[i].iIntLiteral = 0;
    										break;
    									}
    								case ASM_TOKEN_TYPE_STRING:
    									{
    										char* str = GetCurrentLexeme();
    										int   strIndex = AddString(&sasm.stringTable, str);
    										if (GetNextToken() != ASM_TOKEN_TYPE_QUATE)
    											ExitOnCharExpectedError('\\');
    
    										pOpList[i].iType = ASM_OPRAND_TYPE_STRING_INDEX;
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
    					case ASM_TOKEN_TYPE_REG_RETVAL:
    						{
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_REG)
    							{
    								pOpList[i].iType = ASM_OPRAND_TYPE_REG;
    								pOpList[i].iReg  = 0;
    							} 
    							else
    							{
    								ExitOnCodeError(ERROR_MSG_INVALID_OP);
    							}
    							break;
    						}
    					case ASM_TOKEN_TYPE_IDENT:
    						{
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_MEM_REF)
    							{
    								char pIdentifier[MAX_INDENT_SIZE];
    								strcpy(pIdentifier, GetCurrentLexeme());
    								if (!GetSymbol(&sasm.symbolTable, pIdentifier, currentFuncIndex))
    									ExitOnCodeError(ERROR_MSG_UNDEFINED_IDENT);
    
    								int baseIndex = GetStackIndexByIdent(pIdentifier, currentFuncIndex);
    								if (GetLookAheadChar() != '[')
    								{
    									if (GetSizeByIdent(pIdentifier, currentFuncIndex) > 1)
    										ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_NOT_INDEXED);
    									pOpList[i].iType = ASM_OPRAND_TYPE_ABS_STACK_INDEX;
    									pOpList[i].iIntLiteral = baseIndex;
    								}
    								else
    								{
										int arraySize = GetSizeByIdent(pIdentifier, currentFuncIndex);
    									//if (GetSizeByIdent(pIdentifier, currentFuncIndex) == 1)
    										//ExitOnCodeError(ERROR_MSG_INVALID_ARRAY);
    									if (GetNextToken() != ASM_TOKEN_TYPE_OPEN_BRACKET)
    										ExitOnCharExpectedError('[');
    
    									Token indexToken = GetNextToken();
    									if (indexToken == ASM_TOKEN_TYPE_INT)
    									{
    										int offsetIndex = atoi(GetCurrentLexeme());
    										pOpList[i].iType = ASM_OPRAND_TYPE_ABS_STACK_INDEX;

											//must not be global , because in global, baseIndex is positive
    										pOpList[i].iStackIndex = baseIndex - offsetIndex;
    									} 
    									else if (indexToken == ASM_TOKEN_TYPE_IDENT)
    									{
    										char* pStrIndexIdent = GetCurrentLexeme();
    										if (!GetSymbol(&sasm.symbolTable, pStrIndexIdent, currentFuncIndex))
    											ExitOnCodeError(ERROR_MSG_UNDEFINED_IDENT);
											//if size > 1, then the index is also an array, which is not supported
    										if (GetSizeByIdent(pStrIndexIdent, currentFuncIndex) > 1)
    											ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
    
    										int offsetIndex = GetStackIndexByIdent(pStrIndexIdent, currentFuncIndex);
    										pOpList[i].iType = ASM_OPRAND_TYPE_REL_STACK_INDEX;
    										pOpList[i].iStackIndex = baseIndex;
    										pOpList[i].iOffsetIndex = offsetIndex;
											pOpList[i].iSize = arraySize;
    									}
    									else
    									{
    										ExitOnCodeError(ERROR_MSG_INVALID_ARRAY_INDEX);
    									}
    									if (GetNextToken() != ASM_TOKEN_TYPE_CLOSE_BRACKET)
    										ExitOnCharExpectedError('[');
    								}
    							}
    
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_LINE_LABEL)
    							{
    								char* pLabelIdent = GetCurrentLexeme();
    								LabelNode* pLabel = GetLabel(&sasm.labelTable, pLabelIdent, currentFuncIndex);
    								if (!pLabel)
    									ExitOnCodeError(ERROR_MSG_UNDEFINED_LINE_LABEL);
    
    								pOpList[i].iType = ASM_OPRAND_TYPE_INSTR_INDEX;
    								pOpList[i].iInstrIndex = pLabel->iTargetIndex;
    							}
    
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_FUNC_NAME)
    							{
    								char* strFuncName = GetCurrentLexeme();
    								FuncNode* pFunc = GetFunctionByName(&sasm.functionTable, strFuncName);
    								if (!pFunc)
    									ExitOnCodeError(ERROR_MSG_UNDEFINED_FUNC);
    
    								pOpList[i].iType = ASM_OPRAND_TYPE_FUNC_INDEX;
    								pOpList[i].iFuncIndex = pFunc->iIndex;
    							}
    
    							if (currentOpType & ASM_OPRAND_FLAG_TYPE_HOST_API_CALL)
    							{
    								char* strHostAPICall = GetCurrentLexeme();
    								int   index = AddString(&sasm.hostAPICallTable, strHostAPICall);
    
    								pOpList[i].iType = ASM_OPRAND_TYPE_HOST_API_CALL_INDEX;
    								pOpList[i].iHostAPICallIndex = index;
    							}
    							break;
    						}
    					default:
    						ExitOnCodeError(ERROR_MSG_INVALID_OP);
    						break;
    					}
    					if (i < currentInstr.iOpcount - 1)
    						if (GetNextToken() != ASM_TOKEN_TYPE_COMMA)
    							ExitOnCharExpectedError(',');
    				}
    				
    				if (GetNextToken() != ASM_TOKEN_TYPE_NEWLINE)
    					ExitOnCodeError(ERROR_MSG_INVALID_INPUT);
    
    				sasm.instrStream[sasm.currentInstrIndex].pOplist = pOpList;
    
    				sasm.currentInstrIndex++;
    
    				break;
    			}
    		}
    
    		if (!SkipToNextLine())
    			break;
    	}
    }
    
    void BuildSSE() 
    {
    	FILE* pExeFile = fopen(sasm.exeFileName, "wb");
    	if (! pExeFile)
    		ExitOnError("Could not open executable for output");
    	
    	fwrite(SSE_ID_STRING, 4, 1, pExeFile);
#ifdef LOG_ON
    	printf("sse_id_string: %s\n", SSE_ID_STRING);
#endif    
    	char versionMajor = VERSION_MAJOR;
    	char versionMinor = VERSION_MINOR;
    
    	fwrite(&versionMajor, 1, 1, pExeFile);
    	fwrite(&versionMinor, 1, 1, pExeFile);
#ifdef LOG_ON
    	printf("version: %d.%d\n", versionMajor, versionMinor);
    
    	printf("-------------------------------Global Info--------------------------------\n");
#endif     
    	fwrite(&sasm.header.iStackSize, 4, 1, pExeFile);
    	fwrite(&sasm.header.iGlobalDataSize, 4, 1, pExeFile);
#ifdef LOG_ON
    	printf("stackSize: %d\n", sasm.header.iStackSize);
    	printf("globalDataSize: %d\n", sasm.header.iGlobalDataSize);
#endif
    	char isMainExist = 0;
    	if (sasm.header.iIsMainFuncPresent)
    		isMainExist = 1;
    
    	fwrite(&isMainExist, 1, 1, pExeFile);
    	fwrite(&sasm.header.iMainFuncIndex, 4, 1, pExeFile);
#ifdef LOG_ON
    	if (isMainExist)
    		printf("main func exists, and the main func index is: %d\n", sasm.header.iMainFuncIndex);
    	else
    		printf("main func not exists\n");
    
    	printf("-------------------------------Instr Info--------------------------------\n");
#endif    
    	fwrite(&sasm.instrStreamSize, 4, 1, pExeFile);
    	for (int i = 0; i < sasm.instrStreamSize; i++)
    	{
    		short opCode = sasm.instrStream[i].iOpcode;
    		fwrite(&opCode, 2, 1, pExeFile);
#ifdef LOG_ON
    		printf("opCode: %d\n", opCode);
#endif    
    		char opCount = sasm.instrStream[i].iOpCount;
    		fwrite(&opCount, 1, 1, pExeFile);
#ifdef LOG_ON
    		printf("opCount: %d\n", opCount);
#endif
    
    		for (int j = 0; j < opCount; j++)
    		{
    			Oprand currentOp = sasm.instrStream[i].pOplist[j];
    
    			char opType = currentOp.iType;
    			fwrite(&opType, 1, 1, pExeFile);
#ifdef LOG_ON
    			printf("opType: %d\n", opType);
#endif
    
    			switch (currentOp.iType)
    			{
    			case ASM_OPRAND_TYPE_INT:
    				fwrite(&currentOp.iIntLiteral, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opIntLiteral: %d\n", currentOp.iIntLiteral);
#endif
    				break;
    
    			case ASM_OPRAND_TYPE_FLOAT:
    				fwrite(&currentOp.fFloatLiteral, sizeof(float), 1, pExeFile);
#ifdef LOG_ON
    				printf("opFloatLiteral: %f\n", currentOp.fFloatLiteral);
#endif
    				break;
    				
    			case ASM_OPRAND_TYPE_STRING_INDEX:
    				fwrite(&currentOp.iStringTableIndex, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opStringIndex: %d\n", currentOp.iStringTableIndex);
#endif
    				break;
    
    			case ASM_OPRAND_TYPE_INSTR_INDEX:
    				fwrite(&currentOp.iInstrIndex, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opInstrIndex: %d\n", currentOp.iInstrIndex);
#endif
    				break;
    
    			case ASM_OPRAND_TYPE_ABS_STACK_INDEX:
    				fwrite(&currentOp.iStackIndex, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opStackIndex: %d\n", currentOp.iStackIndex);
#endif
    				break;
    
    			case ASM_OPRAND_TYPE_REL_STACK_INDEX:
    				fwrite(&currentOp.iStackIndex, sizeof(int), 1, pExeFile);
    				fwrite(&currentOp.iOffsetIndex, sizeof(int), 1, pExeFile);
					fwrite(&currentOp.iSize, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opStackIndex: %d\n", currentOp.iStackIndex);
    				printf("opOffsetIndex: %d\n", currentOp.iOffsetIndex);
					printf("opSize: %d\n", currentOp.iSize);
#endif
    				break;
    
    			case ASM_OPRAND_TYPE_FUNC_INDEX:
    				fwrite(&currentOp.iFuncIndex, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opFuncIndex: %d\n", currentOp.iFuncIndex);
#endif
    				break;
    
    			case ASM_OPRAND_TYPE_HOST_API_CALL_INDEX:
    				fwrite(&currentOp.iHostAPICallIndex, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opHostApiIndex: %d\n", currentOp.iHostAPICallIndex);
#endif
    				break;;
    
    			case ASM_OPRAND_TYPE_REG:
    				fwrite(&currentOp.iReg, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    				printf("opReg: %d\n", currentOp.iReg);
#endif
    				break;
    
    			default:
    				break;
    			}
    		}
#ifdef LOG_ON
    		printf("\n");
#endif
    	}
#ifdef LOG_ON   
    	printf("-------------------------------String Info--------------------------------\n");
#endif    
    	//int currentNode;
    	LinkListNode* pNode;
    
    	fwrite(&sasm.stringTable.iNodeCount, 4, 1, pExeFile);
#ifdef LOG_ON
    	printf("string number: %d\n\n", sasm.stringTable.iNodeCount);
#endif
    	pNode = sasm.stringTable.pHead;
    
    	char paramCount;
    
    	for (int i = 0; i < sasm.stringTable.iNodeCount; i++)
    	{
    		char* currentString = (char*) pNode->pData;
    		int   currentStrLen = strlen(currentString);
    
    		fwrite(&currentStrLen, 4, 1, pExeFile);
    		fwrite(currentString, currentStrLen, 1, pExeFile);
#ifdef LOG_ON
    		printf("strlen: %d\n", currentStrLen);
    		printf("string: %s\n", currentString);
    
    		printf("\n");
#endif    
    		pNode = pNode->pNext;
    	}
#ifdef LOG_ON
    	printf("-------------------------------Func Info--------------------------------\n");
#endif    
    	fwrite(&sasm.functionTable.iNodeCount, 4, 1, pExeFile);
#ifdef LOG_ON
    	printf("function number: %d\n\n", sasm.functionTable.iNodeCount);
#endif    
    	pNode = sasm.functionTable.pHead;
    
    	for (int i = 0; i < sasm.functionTable.iNodeCount; i++)
    	{
    		FuncNode* pFunc = (FuncNode*) pNode->pData;
    		fwrite(&pFunc->iEntryPoint, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    		printf("func entry point: %d\n", pFunc->iEntryPoint);
#endif    
    		paramCount = pFunc->iParamCount;
    		fwrite(&paramCount, 1, 1, pExeFile);
#ifdef LOG_ON
    		printf("func paramCount: %d\n", paramCount);
#endif    
    		fwrite(&pFunc->iLocalDataSize, sizeof(int), 1, pExeFile);
#ifdef LOG_ON
    		printf("func localDataSize: %d\n", pFunc->iLocalDataSize);
#endif    
    		pNode = pNode->pNext;
#ifdef LOG_ON    
    		printf("\n");
#endif
    	}
#ifdef LOG_ON    
    	printf("-------------------------------HostAPI Info--------------------------------\n");
#endif    
    	fwrite(&sasm.hostAPICallTable.iNodeCount, 4, 1, pExeFile);
#ifdef LOG_ON
    	printf("host api number: %d\n\n", sasm.hostAPICallTable.iNodeCount);
#endif    
    	pNode = sasm.hostAPICallTable.pHead;
    
    	for (int i = 0; i < sasm.hostAPICallTable.iNodeCount; i++)
    	{
    		char* currentHostAPICall = (char*) pNode->pData;
    		char  currentHostAPICallLen = strlen(currentHostAPICall);
    
    		fwrite(&currentHostAPICallLen, 1, 1, pExeFile);
    		fwrite(currentHostAPICall, currentHostAPICallLen, 1, pExeFile);
#ifdef LOG_ON
    		printf("currentHostAPICallLen: %d\n", currentHostAPICallLen);
    		printf("currentHostAPICall: %s\n", currentHostAPICall);
    		printf("\n");
#endif
			pNode = pNode->pNext;
    	}
    
    	fclose(pExeFile);
    }
    
    void BuildSSE_Info() 
    {
    	/*char exeFilename[MAX_FILENAME_SIZE];
    	char* exeDir = "bin\\";
    	strcpy(exeFilename, exeDir);
    	strcat(exeFilename, g_ExeInfoFilename);*/
    
    	FILE* pExeFile = fopen(sasm.exeInfoFilename, "w");
    	if (! pExeFile)
    		ExitOnError("Could not open executable for output");
    	
    	char buf1[MAX_INFO_LINE_SIZE];
    	char buf2[MAX_INFO_LINE_SIZE];
    	
    	swrite(SSE_ID_STRING, 4, 1, buf1);
    	fprintf(pExeFile, "%-20s; magic number: %s\n", buf1, SSE_ID_STRING);
    
    	char versionMajor = VERSION_MAJOR;
    	char versionMinor = VERSION_MINOR;
    	char version[4];
    	version[0] = versionMajor + 48;
    	version[1] = '.';
    	version[2] = versionMinor + 48;
    	version[3] = '\0';
    
    	swrite(&versionMajor, 1, 1, buf1);
    	swrite(&versionMinor, 1, 1, buf2);
    	fprintf(pExeFile, "%-20s; version: %s\n", strcat(buf1, buf2), version);
    
    	fprintf(pExeFile, "\n;-------------------------------Global Info--------------------------------\n");
    
    	swrite(&sasm.header.iStackSize,      4, 1, buf1);
    	swrite(&sasm.header.iGlobalDataSize, 4, 1, buf2);
    	fprintf(pExeFile, "%-20s; stack size: %d\n", buf1, sasm.header.iStackSize);
    	fprintf(pExeFile, "%-20s; global data size: %d\n", buf2, sasm.header.iGlobalDataSize);
    
    	char isMainExist = 0;
    	if (sasm.header.iIsMainFuncPresent)
    		isMainExist = 1;
    
    	swrite(&isMainExist, 1, 1, buf1);
    	swrite(&sasm.header.iMainFuncIndex, 4, 1, buf2);
    	fprintf(pExeFile, "%-20s; is main exist: %c\n", buf1, isMainExist + 48);
    	fprintf(pExeFile, "%-20s; main function index: %d\n", buf1, sasm.header.iMainFuncIndex);
    
    	fprintf(pExeFile, "\n;-------------------------------Instr Info--------------------------------\n");
    
    	swrite(&sasm.instrStreamSize, 4, 1, buf1);
    	fprintf(pExeFile, "%-20s; instr stream size: %d\n", buf1, sasm.instrStreamSize);
    	for (int i = 0; i < sasm.instrStreamSize; i++)
    	{
    		short opCode = sasm.instrStream[i].iOpcode;
    		swrite(&opCode, sizeof(opCode), 1, buf1);
    		fprintf(pExeFile, "%-20s; opCode: %d\n", buf1, opCode);
    
    		char opCount = sasm.instrStream[i].iOpCount;
    		swrite(&opCount, sizeof(opCount), 1, buf1);
    		fprintf(pExeFile, "%-20s; opCount: %d\n", buf1, opCount);
    
    		for (int j = 0; j < opCount; j++)
    		{
    			Oprand currentOp = sasm.instrStream[i].pOplist[j];
    
    			char opType = currentOp.iType;
    			swrite(&opType, sizeof(opType), 1, buf1);
    			fprintf(pExeFile, "%-20s; opType: %d\n", buf1, opType);
    
    			switch (currentOp.iType)
    			{
    			case ASM_OPRAND_TYPE_INT:
    				swrite(&currentOp.iIntLiteral, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opIntLiteral: %d\n", buf1, currentOp.iIntLiteral);
    				break;
    
    			case ASM_OPRAND_TYPE_FLOAT:
    				swrite(&currentOp.fFloatLiteral, sizeof(float), 1, buf1);
    				fprintf(pExeFile, "%-20s; opFloatLiteral: %f\n", buf1, currentOp.fFloatLiteral);
    				break;
    
    			case ASM_OPRAND_TYPE_STRING_INDEX:
    				swrite(&currentOp.iStringTableIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opStringIndex: %d\n", buf1, currentOp.iStringTableIndex);
    				break;
    
    			case ASM_OPRAND_TYPE_INSTR_INDEX:
    				swrite(&currentOp.iInstrIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opInstrIndex: %d\n", buf1, currentOp.iInstrIndex);
    				break;
    
    			case ASM_OPRAND_TYPE_ABS_STACK_INDEX:
    				swrite(&currentOp.iStackIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opStackIndex: %d\n", buf1, currentOp.iStackIndex);
    				break;
    
    			case ASM_OPRAND_TYPE_REL_STACK_INDEX:
    				swrite(&currentOp.iStackIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opStackIndex: %d\n", buf1, currentOp.iStackIndex);
    				swrite(&currentOp.iOffsetIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opOffsetIndex: %d\n", buf1, currentOp.iOffsetIndex);
					swrite(&currentOp.iSize, sizeof(int), 1, buf1);
					fprintf(pExeFile, "%-20s; opSize: %d\n", buf1, currentOp.iSize);
    				break;
    
    			case ASM_OPRAND_TYPE_FUNC_INDEX:
    				swrite(&currentOp.iFuncIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opFuncIndex: %d\n", buf1, currentOp.iFuncIndex);
    				break;
    
    			case ASM_OPRAND_TYPE_HOST_API_CALL_INDEX:
    				swrite(&currentOp.iHostAPICallIndex, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opHostApiIndex: %d\n", buf1, currentOp.iHostAPICallIndex);
    				break;
    
    			case ASM_OPRAND_TYPE_REG:
    				swrite(&currentOp.iReg, sizeof(int), 1, buf1);
    				fprintf(pExeFile, "%-20s; opReg: %d\n", buf1, currentOp.iReg);
    				break;
    
    			default:
    				break;
    			}
    		}
    
    		fprintf(pExeFile, "\n");
    	}
    
    	fprintf(pExeFile, "\n;-------------------------------String Info--------------------------------\n");
    
    	//int currentNode;
    	LinkListNode* pNode;
    
    	swrite(&sasm.stringTable.iNodeCount, sizeof(int), 1, buf1);
    	fprintf(pExeFile, "%-20s; string numbers: %d\n", buf1, sasm.stringTable.iNodeCount);
    
    	pNode = sasm.stringTable.pHead;
    
    	char paramCount;
    
    	for (int i = 0; i < sasm.stringTable.iNodeCount; i++)
    	{
    		char* currentString = (char*) pNode->pData;
    		int   currentStrLen = strlen(currentString);
    
    		swrite(&currentStrLen, sizeof(int), 1, buf1);
    		fprintf(pExeFile, "%-20s; strlen: %d\n", buf1, currentStrLen);
    
    		swrite(currentString, currentStrLen, 1, buf1);
    		fprintf(pExeFile, "%s; string: %s\n", buf1, currentString);
    
    		fprintf(pExeFile, "\n");
    
    		pNode = pNode->pNext;
    	}
    
    	fprintf(pExeFile, "\n;-------------------------------Func Info--------------------------------\n");
    
    	swrite(&sasm.functionTable.iNodeCount, sizeof(int), 1, buf1);
    	fprintf(pExeFile, "%-20s; function numbers: %d\n", buf1, sasm.functionTable.iNodeCount);
    
    	pNode = sasm.functionTable.pHead;
    
    	for (int i = 0; i < sasm.functionTable.iNodeCount; i++)
    	{
    		FuncNode* pFunc = (FuncNode*) pNode->pData;
    		swrite(&pFunc->iEntryPoint, sizeof(int), 1, buf1);
    		fprintf(pExeFile, "%-20s; func entry point: %d\n", buf1, pFunc->iEntryPoint);
    
    		paramCount = pFunc->iParamCount;
    		swrite(&paramCount, 1, 1, buf1);
    		fprintf(pExeFile, "%-20s; func paramCount: %d\n", buf1, paramCount);
    
    		swrite(&pFunc->iLocalDataSize, sizeof(int), 1, buf1);
    		fprintf(pExeFile, "%-20s; func localDataSize: %d\n", buf1, pFunc->iLocalDataSize);
    
    		pNode = pNode->pNext;
    
    		fprintf(pExeFile, "\n");
    	}
    
    	fprintf(pExeFile, "\n;-------------------------------HostAPI Info--------------------------------\n");
    
    	swrite(&sasm.hostAPICallTable.iNodeCount, sizeof(int), 1, buf1);
    	fprintf(pExeFile, "%-20s; host api numbers: %d\n", buf1, sasm.hostAPICallTable.iNodeCount);
    
    	pNode = sasm.hostAPICallTable.pHead;
    
    	for (int i = 0; i < sasm.hostAPICallTable.iNodeCount; i++)
    	{
    		char* currentHostAPICall = (char*) pNode->pData;
    		char  currentHostAPICallLen = strlen(currentHostAPICall);
    
    		swrite(&currentHostAPICallLen, 1, 1, buf1);
    		fprintf(pExeFile, "%-20s; currentHostAPICallLen: %d\n", buf1, currentHostAPICallLen);
    
    		swrite(currentHostAPICall, currentHostAPICallLen, 1, buf1);
    		fprintf(pExeFile, "%s; currentHostAPICall: %s\n", buf1, currentHostAPICall);
    
    		pNode = pNode->pNext;
    		fprintf(pExeFile, "\n");
    	}
    
    	fclose(pExeFile);
    }
}


















