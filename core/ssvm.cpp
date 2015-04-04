#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <windows.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "ssvm.h"

extern _vm::Script vm_script;


namespace _vm
{
	int LoadScript(char* filename)
    {
    	/*char exeFilename[MAX_FILENAME_SIZE];
    	char* exeDir = "bin\\";
    	strcpy(exeFilename, exeDir);
    	strcat(exeFilename, filename);*/
    
    	FILE* pScriptFile = fopen(filename, "rb");
    	if (pScriptFile == NULL)
    		return VM_LOAD_ERROR_FAIL_FILE_OPEN;
    
    	char* magicNumStr = (char*) malloc(5);
    	fread(magicNumStr, 4, 1, pScriptFile);
    	magicNumStr[strlen(SSE_ID_STRING)] = '\0';
    
    	if (strcmp(magicNumStr, SSE_ID_STRING) != 0)
    		return VM_LOAD_ERROR_INVALID_SSE;
    
    	free(magicNumStr);
    
    	int majorVersion = 0;
    	int minorVersion = 0;
    
    	fread(&majorVersion, 1, 1, pScriptFile);
    	fread(&minorVersion, 1, 1, pScriptFile);
    
    	if (majorVersion != VERSION_MAJOR || minorVersion != VERSION_MINOR)
    		return VM_LOAD_ERROR_UNSOPPORTED_VERSION;
    
    	fread(&vm_script.stack.iSize, 4, 1, pScriptFile);
    	if (vm_script.stack.iSize == 0)
    		vm_script.stack.iSize = DEFAULT_STACK_SIZE;
    
    	vm_script.stack.pElement = (Value*) malloc(vm_script.stack.iSize * sizeof(Value));
    
    	fread(&vm_script.iGlobalDataSize, 4, 1, pScriptFile);
    	fread(&vm_script.iIsMainFuncExist, 1, 1, pScriptFile);
    	fread(&vm_script.iMainFuncIndex, 4, 1, pScriptFile);
    
    	fread(&vm_script.instrStream.iSize, 4, 1, pScriptFile);
    	vm_script.instrStream.pInstr = (Instruction*) malloc(vm_script.instrStream.iSize * sizeof(Instruction));
    
    	for (int i = 0; i < vm_script.instrStream.iSize; i++)
    	{
    		vm_script.instrStream.pInstr[i].iOpcode  = 0;
    		fread(&vm_script.instrStream.pInstr[i].iOpcode,  2, 1, pScriptFile);
    		vm_script.instrStream.pInstr[i].iOpCount = 0;
    		fread(&vm_script.instrStream.pInstr[i].iOpCount, 1, 1, pScriptFile);
    
    		int opCount = vm_script.instrStream.pInstr[i].iOpCount;
    
    		Value* pOplist = (Value*) malloc(opCount * sizeof(Value));
    
    		for (int j = 0; j < opCount; j++)
    		{
    			pOplist[j].iType = 0;
    			fread(&pOplist[j].iType, 1, 1, pScriptFile);
    
    			switch (pOplist[j].iType)
    			{
    			case ASM_OPRAND_TYPE_INT:
    				fread(&pOplist[j].iIntLiteral, sizeof(int), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_FLOAT:
    				fread(&pOplist[j].fFloatLiteral, sizeof(float), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_STRING_INDEX:
    				fread(&pOplist[j].iIntLiteral, sizeof(int), 1, pScriptFile);
    				pOplist[j].iType = ASM_OPRAND_TYPE_STRING_INDEX;
    				break;
    			case ASM_OPRAND_TYPE_INSTR_INDEX:
    				fread(&pOplist[j].iInstrIndex, sizeof(int), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_ABS_STACK_INDEX:
    				fread(&pOplist[j].iStackIndex, sizeof(int), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_REL_STACK_INDEX:
    				fread(&pOplist[j].iStackIndex, sizeof(int), 1, pScriptFile);
    				fread(&pOplist[j].iOffsetIndex, sizeof(int), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_FUNC_INDEX:
    				fread(&pOplist[j].iFuncIndex, sizeof(int), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_HOST_API_CALL_INDEX:
    				fread(&pOplist[j].iHostAPICallIndex, sizeof(int), 1, pScriptFile);
    				break;
    			case ASM_OPRAND_TYPE_REG:
    				fread(&pOplist[j].iReg, sizeof(int), 1, pScriptFile);
    				break;
    			default:
    				break;
    			}
    		}
    
    		vm_script.instrStream.pInstr[i].pOplist = pOplist;
    	}
    
    	int stringTableSize;
    	fread(&stringTableSize, 4, 1, pScriptFile);
    	if (stringTableSize)
    	{
    		char** ppStringTable = (char**) malloc(stringTableSize * sizeof(char*));
    		for (int i = 0; i < stringTableSize; i++)
    		{
    			int stringLen = 0;
    			fread(&stringLen, sizeof(int), 1, pScriptFile);
    
    			char* pString = (char*) malloc((stringLen + 1) * sizeof(char));
    			fread(pString, stringLen, 1, pScriptFile);
    			pString[stringLen] = '\0';
    
    			ppStringTable[i] = pString;
    		}
    
    		for (int i = 0; i < vm_script.instrStream.iSize; i++)
    		{
    			int opCount = vm_script.instrStream.pInstr[i].iOpCount; 
    			Value* opList = vm_script.instrStream.pInstr[i].pOplist;
    			for (int j = 0; j < opCount; j++)
    			{
    				if (opList[j].iType == ASM_OPRAND_TYPE_STRING_INDEX)
    				{
    					int stringIndex = opList[j].iIntLiteral;
    					char* temp = (char*) malloc(strlen(ppStringTable[stringIndex]) + 1);
    					strcpy(temp, ppStringTable[stringIndex]);
    
    					opList[j].strStringLiteral = temp;
    				}
    			}
    		}
    
    		for (int i = 0; i < stringTableSize; i++)
    			free(ppStringTable[i]);
    		free(ppStringTable);
    	}
    	
    	int functionTableSize;
    	fread(&functionTableSize, 4, 1, pScriptFile);
    	vm_script.pFuncTable = (Function*) malloc(functionTableSize * sizeof(Function));
    	for (int i = 0; i < functionTableSize; i++)
    	{
    		int entryPoint;
    		fread(&entryPoint, 4, 1, pScriptFile);
    
    		int paramCount = 0;
    		fread(&paramCount, 1, 1, pScriptFile);
    
    		int localDataSize;
    		fread(&localDataSize, 4, 1, pScriptFile);
    
    		int stackFrameSize = paramCount + 1 + localDataSize;
    
    		vm_script.pFuncTable[i].iEntryPoint     = entryPoint;
    		vm_script.pFuncTable[i].iParamCount     = paramCount;
    		vm_script.pFuncTable[i].iLocalDataSize  = localDataSize;
    		vm_script.pFuncTable[i].iStackFrameSize = stackFrameSize;
    	}
    
    	fread(&vm_script.hostAPICallTable.iSize, 4, 1, pScriptFile);
    	vm_script.hostAPICallTable.ppStrCalls = (char**) malloc(vm_script.hostAPICallTable.iSize * sizeof(char*));
    	for (int i = 0; i < vm_script.hostAPICallTable.iSize; i++)
    	{
    		int callLength = 0;
    		fread(&callLength, 1, 1, pScriptFile);
    
    		char* pAPICall = (char*) malloc(callLength + 1);
    		fread(pAPICall, callLength, 1, pScriptFile);
    		pAPICall[callLength] = '\0';
    
    		vm_script.hostAPICallTable.ppStrCalls[i] = pAPICall;
    	}
    
    
    	fclose(pScriptFile);
    
    	return VM_LOAD_OK;
    }
    
    void ResetScript()
    {
    	int mainFuncIndex = vm_script.iMainFuncIndex;
    
    	if (vm_script.pFuncTable)
    	{
    		if (vm_script.iIsMainFuncExist)
    			vm_script.instrStream.iCurrentInstr = vm_script.pFuncTable[mainFuncIndex].iEntryPoint;
    	}
    
    	vm_script.stack.iTopIndex = 0;
    	vm_script.stack.iFrameIndex = 0;
    
    	for (int i = 0; i < vm_script.stack.iSize; i++)
    		vm_script.stack.pElement[i].iType = ASM_OPRAND_TYPE_NULL;
    
    	vm_script.iIsPaused = FALSE;
    
    	PushFrame(vm_script.iGlobalDataSize);
		// Note: We plus one extra stack element to compensate for
		// the function index that usually sits on top of stack
    	PushFrame(vm_script.pFuncTable[mainFuncIndex].iStackFrameSize + 1);
    }
    
    void RunScript()
    {
    	int isExitExeLoop = FALSE;
    	while (TRUE)
    	{
    		if (vm_script.iIsPaused)
    		{
    			/*if (GetCurrentTime() >= svm.iPauseEndTime)
    			svm.iIsPaused = FALSE;
    			else
    			continue;*/
    		}
    
    		int currentInstr = vm_script.instrStream.iCurrentInstr;
    		int opCode = vm_script.instrStream.pInstr[currentInstr].iOpcode;
    
    		/*	printf("\t");
    		if (opCode < 10)
    		printf(" %d", opCode);
    		else
    		printf("%d", opCode);
    		printf(" %s", "dd");*/
    
    		switch (opCode)
    		{
    		case ASM_INSTR_MOV:
    
    		case ASM_INSTR_ADD:
    		case ASM_INSTR_SUB:
    		case ASM_INSTR_MUL:
    		case ASM_INSTR_DIV:
    		case ASM_INSTR_MOD:
    		case ASM_INSTR_EXP:
    
    		case ASM_INSTR_AND:
    		case ASM_INSTR_OR:
    		case ASM_INSTR_XOR:
    		case ASM_INSTR_SHL:
    		case ASM_INSTR_SHR:
    			{
    				Value dest = GetOpValue(0);
    				Value source = GetOpValue(1);
    
    				switch (opCode)
    				{
    				case ASM_INSTR_MOV:
    					if (GetOpValuePointer(0) == GetOpValuePointer(1))
    						break;
    
    					CopyValue(&dest, source);
    					break;
    				case ASM_INSTR_ADD:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral += GetOpValueAsInt(1);
    					else
    						dest.fFloatLiteral += GetOpValueAsFloat(1);
    					break;
    				case ASM_INSTR_SUB:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral -= GetOpValueAsInt(1);
    					else
    						dest.fFloatLiteral -= GetOpValueAsFloat(1);
    					break;
    				case ASM_INSTR_MUL:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral *= GetOpValueAsInt(1);
    					else
    						dest.fFloatLiteral *= GetOpValueAsFloat(1);
    					break;
    				case ASM_INSTR_DIV:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral /= GetOpValueAsInt(1);
    					else
    						dest.fFloatLiteral /= GetOpValueAsFloat(1);
    					break;
    				case ASM_INSTR_MOD:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral %= GetOpValueAsInt(1);
    					break;
    				case ASM_INSTR_EXP:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral = (int) pow((float)dest.iIntLiteral, GetOpValueAsInt(1));
    					else
    						dest.fFloatLiteral = (float) pow((float)dest.iIntLiteral, GetOpValueAsFloat(1));
    					break;
    
    				case ASM_INSTR_AND:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral &= GetOpValueAsInt(1);
    					break;
    				case ASM_INSTR_OR:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral |= GetOpValueAsInt(1);
    					break;
    				case ASM_INSTR_XOR:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral ^= GetOpValueAsInt(1);
    					break;
    				case ASM_INSTR_SHL:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral <<= GetOpValueAsInt(1);
    					break;
    				case ASM_INSTR_SHR:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral >>= GetOpValueAsInt(1);
    					break;
    				default:
    					break;
    				}
    
    				*GetOpValuePointer(0) = dest;
    
    				break;
    			}
    
    		case ASM_INSTR_NEG:
    		case ASM_INSTR_NOT:
    		case ASM_INSTR_INC:
    		case ASM_INSTR_DEC:
    			{
    				Value dest = GetOpValue(0);
    
    				switch (opCode)
    				{
    				case ASM_INSTR_NEG:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral = -dest.iIntLiteral;
    					else
    						dest.fFloatLiteral = -dest.fFloatLiteral;
    					break;
    				case ASM_INSTR_NOT:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral = ~dest.iIntLiteral;
    				case ASM_INSTR_INC:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral++;
    					else
    						dest.fFloatLiteral++;
    					break;
    				case ASM_INSTR_DEC:
    					if (dest.iType == ASM_OPRAND_TYPE_INT)
    						dest.iIntLiteral--;
    					else
    						dest.fFloatLiteral--;
    					break;
    				default:
    					break;
    				}
    
    				*GetOpValuePointer(0) = dest;
    
    				break;
    			}
    
    		case ASM_INSTR_CONCAT:
    			{
    				Value dest = GetOpValue(0);
    
    				if (dest.iType != ASM_OPRAND_TYPE_STRING_INDEX)
    					break;
    
    				char* appendString = GetOpValueAsString(1);
    				int newStringLength = strlen(dest.strStringLiteral) + strlen(appendString);
    				char* newString = (char*) malloc(newStringLength + 1);
    
    				strcpy(newString, dest.strStringLiteral);
    				strcat(newString, appendString);
    
    				//newString[newStringLength] = '\0';
    				free(dest.strStringLiteral);
    				dest.strStringLiteral = newString;
    
    				*GetOpValuePointer(0) = dest;
    
    				break;
    			}
    
    		case ASM_INSTR_GETCHAR:
    			{
    				Value dest = GetOpValue(0);
    
    				char* targetString = GetOpValueAsString(1);
    
    				char* newString;
    				if (dest.iType == ASM_OPRAND_TYPE_STRING_INDEX)
    				{
    					if (strlen(dest.strStringLiteral) >= 1)
    					{
    						newString = dest.strStringLiteral;
    					}
    					else
    					{
    						free(dest.strStringLiteral);
    						newString = (char*) malloc(2);
    					}
    				}
    				else
    				{
    					newString = (char*) malloc(2);
    					dest.iType = ASM_OPRAND_TYPE_STRING_INDEX;
    				}
    
    				int charIndex = GetOpValueAsInt(2);
    				newString[0] = targetString[charIndex];
    				newString[1] = '\0';
    
    				dest.strStringLiteral = newString;
    				*GetOpValuePointer(0) = dest;

					break;
    			}
    
    		case ASM_INSTR_SETCHAR:
    			{
    				int charIndex = GetOpValueAsInt(1);
    				char* targetString = GetOpValueAsString(2);
    
    				if (GetOpValue(0).iType != ASM_OPRAND_TYPE_STRING_INDEX)
    					break;
    				GetOpValuePointer(0)->strStringLiteral[charIndex] = targetString[0];
    
    				break;
    			}
    
    		case ASM_INSTR_JMP:
    			{
    				int nextInstr = GetOpValueAsInstrIndex(0);
    				vm_script.instrStream.iCurrentInstr = nextInstr;
    
    				break;
    			}
    		case ASM_INSTR_JE:
    		case ASM_INSTR_JNE:
    		case ASM_INSTR_JG:
    		case ASM_INSTR_JGE:
    		case ASM_INSTR_JL:
    		case ASM_INSTR_JLE:
    			{
    				Value op0 = GetOpValue(0);
    				Value op1 = GetOpValue(1);
    				int nextInstr = GetOpValueAsInstrIndex(2);
    
    				int isJump = FALSE;
    				switch (opCode)
    				{
    				case ASM_INSTR_JE:
    					{
    						switch (op0.iType)
    						{
    						case ASM_OPRAND_TYPE_INT:
    							if (op0.iIntLiteral == op1.iIntLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_FLOAT:
    							if (op0.fFloatLiteral == op1.fFloatLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_STRING_INDEX:
								if (op0.strStringLiteral == NULL || op1.strStringLiteral == NULL)
								{
									isJump = TRUE;
								} 
								else
								{
									if (strcmp(op0.strStringLiteral, op1.strStringLiteral) != 0)
										isJump = TRUE;
								}
								break;
    						default:
    							break;
    						}

							break;
    					}
    				case ASM_INSTR_JNE:
    					{
    						switch (op0.iType)
    						{
    						case ASM_OPRAND_TYPE_INT:
    							if (op0.iIntLiteral != op1.iIntLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_FLOAT:
    							if (op0.fFloatLiteral != op1.fFloatLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_STRING_INDEX:
								if (op0.strStringLiteral == NULL || op1.strStringLiteral == NULL)
								{
									isJump = TRUE;
								} 
								else
								{
									if (strcmp(op0.strStringLiteral, op1.strStringLiteral) != 0)
										isJump = TRUE;
								}
										
    							break;
    						default:
    							break;
    						}

							break;
    					}
    				case ASM_INSTR_JG:
    					{
    						switch (op0.iType)
    						{
    						case ASM_OPRAND_TYPE_INT:
    							if (op0.iIntLiteral > op1.iIntLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_FLOAT:
    							if (op0.fFloatLiteral > op1.fFloatLiteral)
    								isJump = TRUE;
    							break;
    						default:
    							break;
    						}

							break;
    					}
    				case ASM_INSTR_JGE:
    					{
    						switch (op0.iType)
    						{
    						case ASM_OPRAND_TYPE_INT:
    							if (op0.iIntLiteral >= op1.iIntLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_FLOAT:
    							if (op0.fFloatLiteral >= op1.fFloatLiteral)
    								isJump = TRUE;
    							break;
    						default:
    							break;
    						}

							break;
    					}
    				case ASM_INSTR_JL:
    					{
    						switch (op0.iType)
    						{
    						case ASM_OPRAND_TYPE_INT:
    							if (op0.iIntLiteral < op1.iIntLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_FLOAT:
    							if (op0.fFloatLiteral < op1.fFloatLiteral)
    								isJump = TRUE;
    							break;
    						default:
    							break;
    						}

							break;
    					}
    				case ASM_INSTR_JLE:
    					{
    						switch (op0.iType)
    						{
    						case ASM_OPRAND_TYPE_INT:
    							if (op0.iIntLiteral <= op1.iIntLiteral)
    								isJump = TRUE;
    							break;
    						case ASM_OPRAND_TYPE_FLOAT:
    							if (op0.fFloatLiteral <= op1.fFloatLiteral)
    								isJump = TRUE;
    							break;
    						default:
    							break;
    						}

							break;
    					}
    				default:
    					break;
    				}
    
    				if (isJump)
    					vm_script.instrStream.iCurrentInstr = nextInstr;
    
    				break;
    			}
    
    		case ASM_INSTR_PUSH:
    			{
    				Value value = GetOpValue(0);
    				Push(value);
    				break;
    			}
    		case ASM_INSTR_POP:
    			{
    				*GetOpValuePointer(0) = Pop();
    				break;
    			}
    
    		case ASM_INSTR_CALL:
    			{
    				int targetFuncIndex = GetOpValueAsFuncIndex(0);
    				Function targetFunc = vm_script.pFuncTable[targetFuncIndex];
    
    				Value returnAddress;
    				returnAddress.iInstrIndex = vm_script.instrStream.iCurrentInstr + 1;
    				Push(returnAddress);
    
    				PushFrame(targetFunc.iLocalDataSize + 1);
    
    				Value targetFuncIndexValue;
    				targetFuncIndexValue.iFuncIndex = targetFuncIndex;
    				SetStackValue(vm_script.stack.iTopIndex - 1, targetFuncIndexValue);
    				
    				vm_script.instrStream.iCurrentInstr = targetFunc.iEntryPoint;
    				break;
    			}
    		case ASM_INSTR_RET:
    			{
    				Value targetFuncIndexValue = Pop();
    				Function targetFunc = vm_script.pFuncTable[targetFuncIndexValue.iFuncIndex];
					
    				int frameIndex = targetFuncIndexValue.iOffsetIndex;
    
    				Value returnAddress = GetStackValue(vm_script.stack.iTopIndex - (targetFunc.iLocalDataSize + 1));
    				
    				PopFrame(targetFunc.iStackFrameSize);
    
    				//vm_script.stack.iFrameIndex = frameIndex;
    				vm_script.instrStream.iCurrentInstr = returnAddress.iInstrIndex;
    
    				break;
    			}
    		case ASM_INSTR_CALLHOST:
    			{
					Value hostAPICall = GetOpValue(0);
					int hostAPICallIndex = hostAPICall.iHostAPICallIndex;

					char* hostAPIFuncName = vm_script.hostAPICallTable.ppStrCalls[hostAPICallIndex];

					BuiltInFunc* pBuiltInFunc = GetBuiltInFunc(&vm_script.builtInFuncTable, hostAPIFuncName);

					int iTopIndex = vm_script.stack.iTopIndex;
					int paramCount = pBuiltInFunc->iParamCount;

					Value* params = (Value*) malloc(sizeof(Value) * paramCount);
					for (int i = 0; i < paramCount; i++)
					{
						params[paramCount - i - 1] = vm_script.stack.pElement[iTopIndex - (i + 1)];
					}

					pBuiltInFunc->pf(params, paramCount);

					PopFrame(paramCount);



					//Value returnValue = GetReturnValue();
					//Push(returnValue);
    				break;
    			}
    
    		case ASM_INSTR_PRINT:
    			{
    				Value value = GetOpValue(0);
    				switch(value.iType)
    				{
    				case ASM_OPRAND_TYPE_INT:
    					printf("%d\n", value.iIntLiteral);
    					break;
    				case ASM_OPRAND_TYPE_FLOAT:
    					printf("%f\n", value.fFloatLiteral);
    					break;
    				case ASM_OPRAND_TYPE_STRING_INDEX:
    					printf("%s\n", value.strStringLiteral);
    					break;
    				default:
    					break;
    				}
    
    				break;
    			}
    
    		case ASM_INSTR_PAUSE:
    			{
    				break;
    			}
    		case ASM_INSTR_EXIT:
    			{
    				Value exitCodeValue = GetOpValue(0);
    				int exitCode = exitCodeValue.iIntLiteral;
    
    				isExitExeLoop = TRUE;
    
    				break;
    			}
    
    		default:
    			break;
    		}
    
    		//printf("\n");
    
    		if (currentInstr == vm_script.instrStream.iCurrentInstr)
    			vm_script.instrStream.iCurrentInstr++;
    
    		if (isExitExeLoop)
    			break;
    	}
    	
    }
    
    void CopyValue(Value* dest, Value source)
    {
    	if (dest->iType == ASM_OPRAND_TYPE_STRING_INDEX)
    		free(dest->strStringLiteral);
    
    	*dest = source;
    
    	if (source.iType == ASM_OPRAND_TYPE_STRING_INDEX)
    	{
    		dest->strStringLiteral = (char*) malloc(strlen(source.strStringLiteral) + 1);
    		strcpy(dest->strStringLiteral, source.strStringLiteral);
    	}
    
    }
    
    int ValueToInt(Value value)
    {
    	switch (value.iType)
    	{
    	case ASM_OPRAND_TYPE_INT:
    		return value.iIntLiteral;
    	case ASM_OPRAND_TYPE_FLOAT:
    		return (int)value.fFloatLiteral;
    	case ASM_OPRAND_TYPE_STRING_INDEX:
    		return atoi(value.strStringLiteral);
    	default:
    		return 0;
    	}
    }
    
    float ValueToFloat(Value value)
    {
    	switch (value.iType)
    	{
    	case ASM_OPRAND_TYPE_INT:
    		return (float) value.iIntLiteral;
    	case ASM_OPRAND_TYPE_FLOAT:
    		return value.fFloatLiteral;
    	case ASM_OPRAND_TYPE_STRING_INDEX:
    		return (float) atof(value.strStringLiteral);
    	default:
    		return 0;
    	}
    }
    
    char* ValueToString(Value value)
    {
    	char* str = NULL;
    	if (value.iType != ASM_OPRAND_TYPE_STRING_INDEX)
    		str = (char*) malloc(MAX_VALUE_STRING_SIZE + 1);
    
    	switch (value.iType)
    	{
    	case ASM_OPRAND_TYPE_INT:
    		_itoa(value.iIntLiteral, str, 10);
    		return str;
    	case ASM_OPRAND_TYPE_FLOAT:
    		sprintf(str, "%f", value.fFloatLiteral);
    		return str;
    	case ASM_OPRAND_TYPE_STRING_INDEX:
    		return value.strStringLiteral;
    	default:
    		return NULL;
    	}
    }
    
    Value GetStackValue(int index)
    {
    	return vm_script.stack.pElement[GET_STACK_INDEX(index)];
    }
    
    void SetStackValue(int index, Value value)
    {
    	vm_script.stack.pElement[GET_STACK_INDEX(index)] = value;
    }
    
    
    Value GetOpValue(int opIndex)
    {
    	int currentInstr = vm_script.instrStream.iCurrentInstr;
    	Value opValue = vm_script.instrStream.pInstr[currentInstr].pOplist[opIndex];
    
    	switch (opValue.iType)
    	{
    	case ASM_OPRAND_TYPE_ABS_STACK_INDEX:
    	case ASM_OPRAND_TYPE_REL_STACK_INDEX:
    		{
    			int index = GetOpValueAsStackIndex(opIndex);
    			return GetStackValue(index);
    		}
    		
    
    	case ASM_OPRAND_TYPE_REG:
    		return vm_script._RetVal;
    
    	default:
    		return opValue;
    	}
    }
    
    Value* GetOpValuePointer(int opIndex)
    {
    
    	int opType = GetOpTypeFromInstr(opIndex);
    	switch (opType)
    	{
    	case ASM_OPRAND_TYPE_ABS_STACK_INDEX:
    	case ASM_OPRAND_TYPE_REL_STACK_INDEX:
    		{
    			int index = GetOpValueAsStackIndex(opIndex);
    			return &vm_script.stack.pElement[GET_STACK_INDEX(index)];
    		}
    		
    
    	case ASM_OPRAND_TYPE_REG:
    		return &vm_script._RetVal;
    
    	default:
    		return NULL;
    	}
    }
    
    int GetOpTypeFromStack(int opIndex)
    {
    	return GetOpValue(opIndex).iType;
    }
    
    int GetOpTypeFromInstr(int opIndex)
    {
    	int currentInstr = vm_script.instrStream.iCurrentInstr;
    	Value opValue = vm_script.instrStream.pInstr[currentInstr].pOplist[opIndex];
    
    	return opValue.iType;
    }
    
    int GetOpValueAsInt(int opIndex)
    {
    	Value value = GetOpValue(opIndex);
    	int valueInt = ValueToInt(value);
    
    	return valueInt;
    }
    float GetOpValueAsFloat(int opIndex)
    {
    	Value value = GetOpValue(opIndex);
    	float valueFloat = ValueToFloat(value);
    
    	return valueFloat;
    }
    char* GetOpValueAsString(int opIndex)
    {
    	Value value = GetOpValue(opIndex);
    	char* valueStr = ValueToString(value);
    
    	return valueStr;
    }
    int GetOpValueAsStackIndex(int opIndex)
    {
    	int currentInstr = vm_script.instrStream.iCurrentInstr;
    	Value opValue = vm_script.instrStream.pInstr[currentInstr].pOplist[opIndex];
    
    	switch (opValue.iType)
    	{
    	case ASM_OPRAND_TYPE_ABS_STACK_INDEX:
    		return opValue.iStackIndex;
    	case ASM_OPRAND_TYPE_REL_STACK_INDEX:
    		int baseIndex = opValue.iStackIndex;
    		int offsetIndex = opValue.iOffsetIndex;
    
    		Value stackValue = GetStackValue(offsetIndex);
    
    		return baseIndex + stackValue.iIntLiteral;
    	}
    	return 0;
    }
    int GetOpValueAsInstrIndex(int opIndex)
    {
    	Value value = GetOpValue(opIndex);
    
    	return value.iInstrIndex;
    }
    int GetOpValueAsFuncIndex(int opIndex)
    {
    	Value value = GetOpValue(opIndex);
    
    	return value.iFuncIndex;
    }
    char* GetOpValueASHostAPI(int opIndex)
    {
    	Value value = GetOpValue(opIndex);
    
    	int hostAPIIndex = value.iHostAPICallIndex;
    	
    	return vm_script.hostAPICallTable.ppStrCalls[hostAPIIndex];
    }
    
    void Push(Value value)
    {
    	int topIndex = vm_script.stack.iTopIndex;
    	vm_script.stack.pElement[topIndex] = value;
    	vm_script.stack.iTopIndex++;
    }
    Value Pop()
    {
    	vm_script.stack.iTopIndex--;
    	int topIndex = vm_script.stack.iTopIndex;
    
    	Value value;
    	CopyValue(&value, vm_script.stack.pElement[topIndex]);
    
    	return value;
    }
    void PushFrame(int size)
    {
    	vm_script.stack.iTopIndex += size;
    	vm_script.stack.iFrameIndex = vm_script.stack.iTopIndex;
    }
    void PopFrame(int size)
    {
    	vm_script.stack.iTopIndex -= size;
    }

	void ReturnValue_String(char* str)
	{
		Value returnValue;
		returnValue.iType =  ASM_OPRAND_TYPE_STRING_INDEX;
		returnValue.strStringLiteral = str;

		CopyValue(&vm_script._RetVal, returnValue);
	}
	void ReturnValue_Int(int integer)
	{
		vm_script._RetVal.iType = ASM_OPRAND_TYPE_INT;
		vm_script._RetVal.iIntLiteral = integer;
	}
	void ReturnValue_Floatt(float fnum)
	{
		vm_script._RetVal.iType = ASM_OPRAND_TYPE_FLOAT;
		vm_script._RetVal.fFloatLiteral = fnum;
	}
}



