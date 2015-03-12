#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <windows.h>

#include "ssvm_pre.h"
#include "sslang_vm.h"
#include "ssvm.h"

extern Script g_Script;

int LoadScript(char* filename)
{
	FILE* pScriptFile = fopen(filename, "rb");
	if (pScriptFile == NULL)
		return LOAD_ERROR_FAIL_FILE_OPEN;

	char* magicNumStr = (char*) malloc(5);
	fread(magicNumStr, 4, 1, pScriptFile);
	magicNumStr[strlen(SSE_ID_STRING)] = '\0';

	if (strcmp(magicNumStr, SSE_ID_STRING) != 0)
		return LOAD_ERROR_INVALID_SSE;

	free(magicNumStr);

	int majorVersion = 0;
	int minorVersion = 0;

	fread(&majorVersion, 1, 1, pScriptFile);
	fread(&minorVersion, 1, 1, pScriptFile);

	if (majorVersion != VERSION_MAJOR || minorVersion != VERSION_MINOR)
		return LOAD_ERROR_UNSOPPORTED_VERSION;

	fread(&g_Script.stack.iSize, 4, 1, pScriptFile);
	if (g_Script.stack.iSize == 0)
		g_Script.stack.iSize = DEFAULT_STACK_SIZE;

	g_Script.stack.pElement = (Value*) malloc(g_Script.stack.iSize * sizeof(Value));

	fread(&g_Script.iGlobalDataSize, 4, 1, pScriptFile);
	fread(&g_Script.iIsMainFuncExist, 1, 1, pScriptFile);
	fread(&g_Script.iMainFuncIndex, 4, 1, pScriptFile);

	fread(&g_Script.instrStream.iSize, 4, 1, pScriptFile);
	g_Script.instrStream.pInstr = (Instr*) malloc(g_Script.instrStream.iSize * sizeof(Instr));

	for (int i = 0; i < g_Script.instrStream.iSize; i++)
	{
		g_Script.instrStream.pInstr[i].iOpcode  = 0;
		fread(&g_Script.instrStream.pInstr[i].iOpcode,  2, 1, pScriptFile);
		g_Script.instrStream.pInstr[i].iOpCount = 0;
		fread(&g_Script.instrStream.pInstr[i].iOpCount, 1, 1, pScriptFile);

		int opCount = g_Script.instrStream.pInstr[i].iOpCount;

		Value* pOplist = (Value*) malloc(opCount * sizeof(Value));

		for (int j = 0; j < opCount; j++)
		{
			pOplist[j].iType = 0;
			fread(&pOplist[j].iType, 1, 1, pScriptFile);

			switch (pOplist[j].iType)
			{
			case OP_TYPE_INT:
				fread(&pOplist[j].iIntLiteral, sizeof(int), 1, pScriptFile);
				break;
			case OP_TYPE_FLOAT:
				fread(&pOplist[j].fFloatLiteral, sizeof(float), 1, pScriptFile);
				break;
			case OP_TYPE_STRING_INDEX:
				fread(&pOplist[j].iIntLiteral, sizeof(int), 1, pScriptFile);
				pOplist[j].iType = OP_TYPE_STRING_INDEX;
				break;
			case OP_TYPE_INSTR_INDEX:
				fread(&pOplist[j].iInstrIndex, sizeof(int), 1, pScriptFile);
				break;
			case OP_TYPE_ABS_STACK_INDEX:
				fread(&pOplist[j].iStackIndex, sizeof(int), 1, pScriptFile);
				break;
			case OP_TYPE_REL_STACK_INDEX:
				fread(&pOplist[j].iStackIndex, sizeof(int), 1, pScriptFile);
				break;
			case OP_TYPE_FUNC_INDEX:
				fread(&pOplist[j].iOffsetIndex, sizeof(int), 1, pScriptFile);
				break;
			case OP_TYPE_HOST_API_CALL_INDEX:
				fread(&pOplist[j].iHostAPICallIndex, sizeof(int), 1, pScriptFile);
				break;
			case OP_TYPE_REG:
				fread(&pOplist[j].iReg, sizeof(int), 1, pScriptFile);
				break;
			default:
				break;
			}
		}

		g_Script.instrStream.pInstr[i].pOplist = pOplist;
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

		for (int i = 0; i < g_Script.instrStream.iSize; i++)
		{
			int opCount = g_Script.instrStream.pInstr[i].iOpCount; 
			Value* opList = g_Script.instrStream.pInstr[i].pOplist;
			for (int j = 0; j < opCount; j++)
			{
				if (opList[j].iType == OP_TYPE_STRING_INDEX)
				{
					int stringIndex = opList[j].iIntLiteral;
					char* temp = (char*) malloc(strlen(ppStringTable[stringIndex] + 1));
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
	g_Script.pFuncTable = (Func*) malloc(functionTableSize * sizeof(Func));
	for (int i = 0; i < functionTableSize; i++)
	{
		int entryPoint;
		fread(&entryPoint, 4, 1, pScriptFile);

		int paramCount = 0;
		fread(&paramCount, 1, 1, pScriptFile);

		int localDataSize;
		fread(&localDataSize, 4, 1, pScriptFile);

		int stackFrameSize = paramCount + 1 + localDataSize;

		g_Script.pFuncTable[i].iEntryPoint     = entryPoint;
		g_Script.pFuncTable[i].iParamCount     = paramCount;
		g_Script.pFuncTable[i].iLocalDataSize  = localDataSize;
		g_Script.pFuncTable[i].iStackFrameSize = stackFrameSize;
	}

	fread(&g_Script.hostAPICallTable.iSize, 4, 1, pScriptFile);
	g_Script.hostAPICallTable.ppStrCalls = (char**) malloc(g_Script.hostAPICallTable.iSize * sizeof(char*));
	for (int i = 0; i < g_Script.hostAPICallTable.iSize; i++)
	{
		int callLength = 0;
		fread(&callLength, 1, 1, pScriptFile);

		char* pAPICall = (char*) malloc(callLength + 1);
		fread(pAPICall, callLength, 1, pScriptFile);
		pAPICall[callLength] = '\0';

		g_Script.hostAPICallTable.ppStrCalls[i] = pAPICall;
	}


	fclose(pScriptFile);

	return LOAD_OK;
}

void ResetScript()
{
	int mainFuncIndex = g_Script.iMainFuncIndex;

	if (g_Script.pFuncTable)
	{
		if (g_Script.iIsMainFuncExist)
			g_Script.instrStream.iCurrentInstr = g_Script.pFuncTable[mainFuncIndex].iEntryPoint;
	}

	g_Script.stack.iTopIndex = 0;
	g_Script.stack.iFrameIndex = 0;

	for (int i = 0; i < g_Script.stack.iSize; i++)
		g_Script.stack.pElement[i].iType = OP_TYPE_NULL;

	g_Script.iIsPaused = FALSE;

	PushFrame(g_Script.iGlobalDataSize);
	PushFrame(g_Script.pFuncTable[mainFuncIndex].iStackFrameSize + 1);
}

void RunScript()
{
	int isExitExeLoop = FALSE;
	while (TRUE)
	{
		if (g_Script.iIsPaused)
		{
			/*if (GetCurrentTime() >= g_Script.iPauseEndTime)
			g_Script.iIsPaused = FALSE;
			else
			continue;*/
		}

		int currentInstr = g_Script.instrStream.iCurrentInstr;
		int opCode = g_Script.instrStream.pInstr[currentInstr].iOpcode;

		printf("\t");
		if (opCode < 10)
			printf(" %d", opCode);
		else
			printf("%d", opCode);
		printf(" %s", "dd");

		switch (opCode)
		{
		case INSTR_MOV:

		case INSTR_ADD:
		case INSTR_SUB:
		case INSTR_MUL:
		case INSTR_DIV:
		case INSTR_MOD:
		case INSTR_EXP:

		case INSTR_AND:
		case INSTR_OR:
		case INSTR_XOR:
		case INSTR_SHL:
		case INSTR_SHR:
			{
				Value dest = GetOpValue(0);
				Value source = GetOpValue(1);

				switch (opCode)
				{
				case INSTR_MOV:
					if (GetOpValuePointer(0) == GetOpValuePointer(1))
						break;

					CopyValue(&dest, source);
					break;
				case INSTR_ADD:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral += GetOpValueAsInt(1);
					else
						dest.fFloatLiteral += GetOpValueAsFloat(1);
					break;
				case INSTR_SUB:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral -= GetOpValueAsInt(1);
					else
						dest.fFloatLiteral -= GetOpValueAsFloat(1);
					break;
				case INSTR_MUL:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral *= GetOpValueAsInt(1);
					else
						dest.fFloatLiteral *= GetOpValueAsFloat(1);
					break;
				case INSTR_DIV:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral /= GetOpValueAsInt(1);
					else
						dest.fFloatLiteral /= GetOpValueAsFloat(1);
					break;
				case INSTR_MOD:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral %= GetOpValueAsInt(1);
					break;
				case INSTR_EXP:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral = (int) pow((float)dest.iIntLiteral, GetOpValueAsInt(1));
					else
						dest.fFloatLiteral = (float) pow((float)dest.iIntLiteral, GetOpValueAsFloat(1));
					break;

				case INSTR_AND:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral &= GetOpValueAsInt(1);
					break;
				case INSTR_OR:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral |= GetOpValueAsInt(1);
					break;
				case INSTR_XOR:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral ^= GetOpValueAsInt(1);
					break;
				case INSTR_SHL:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral <<= GetOpValueAsInt(1);
					break;
				case INSTR_SHR:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral >>= GetOpValueAsInt(1);
					break;
				default:
					break;
				}

				*GetOpValuePointer(0) = dest;
			}

		case INSTR_NEG:
		case INSTR_NOT:
		case INSTR_INC:
		case INSTR_DEC:
			{
				Value dest = GetOpValue(0);

				switch (opCode)
				{
				case INSTR_NEG:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral = -dest.iIntLiteral;
					else
						dest.fFloatLiteral = -dest.fFloatLiteral;
					break;
				case INSTR_NOT:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral = ~dest.iIntLiteral;
				case INSTR_INC:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral++;
					else
						dest.fFloatLiteral++;
					break;
				case INSTR_DEC:
					if (dest.iType == OP_TYPE_INT)
						dest.iIntLiteral--;
					else
						dest.fFloatLiteral--;
					break;
				default:
					break;
				}

				*GetOpValuePointer(0) = dest;
			}

		case INSTR_CONCAT:
			{
				Value dest = GetOpValue(0);

				if (dest.iType != OP_TYPE_STRING_INDEX)
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

		case INSTR_GETCHAR:
			{
				Value dest = GetOpValue(0);

				char* targetString = GetOpValueAsString(1);

				char* newString;
				if (dest.iType == OP_TYPE_STRING_INDEX)
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
					dest.iType = OP_TYPE_STRING_INDEX;
				}

				int charIndex = GetOpValueAsInt(2);
				newString[0] = targetString[charIndex];
				newString[1] = '\0';

				dest.strStringLiteral = newString;
				*GetOpValuePointer(0) = dest;
			}

		case INSTR_SETCHAR:
			{
				int charIndex = GetOpValueAsInt(1);
				char* targetString = GetOpValueAsString(2);

				if (GetOpValue(0).iType != OP_TYPE_STRING_INDEX)
					break;
				GetOpValuePointer(0)->strStringLiteral[charIndex] = targetString[0];
			}

		case INSTR_JMP:
			{
				int nextInstr = GetOpValueAsInstrIndex(0);
				g_Script.instrStream.iCurrentInstr = nextInstr;

				break;
			}
		case INSTR_JE:
		case INSTR_JNE:
		case INSTR_JG:
		case INSTR_JGE:
		case INSTR_JL:
		case INSTR_JLE:
			{
				Value op0 = GetOpValue(0);
				Value op1 = GetOpValue(1);
				int nextInstr = GetOpValueAsInstrIndex(2);

				int isJump = FALSE;
				switch (opCode)
				{
				case INSTR_JE:
					{
						switch (op0.iType)
						{
						case OP_TYPE_INT:
							if (op0.iIntLiteral == op1.iIntLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_FLOAT:
							if (op0.fFloatLiteral == op1.fFloatLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_STRING_INDEX:
							if (strcmp(op0.strStringLiteral, op1.strStringLiteral) == 0)
								isJump = TRUE;
							break;
						default:
							break;
						}
					}
				case INSTR_JNE:
					{
						switch (op0.iType)
						{
						case OP_TYPE_INT:
							if (op0.iIntLiteral != op1.iIntLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_FLOAT:
							if (op0.fFloatLiteral != op1.fFloatLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_STRING_INDEX:
							if (strcmp(op0.strStringLiteral, op1.strStringLiteral) != 0)
								isJump = TRUE;
							break;
						default:
							break;
						}
					}
				case INSTR_JG:
					{
						switch (op0.iType)
						{
						case OP_TYPE_INT:
							if (op0.iIntLiteral > op1.iIntLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_FLOAT:
							if (op0.fFloatLiteral > op1.fFloatLiteral)
								isJump = TRUE;
							break;
						default:
							break;
						}
					}
				case INSTR_JGE:
					{
						switch (op0.iType)
						{
						case OP_TYPE_INT:
							if (op0.iIntLiteral >= op1.iIntLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_FLOAT:
							if (op0.fFloatLiteral >= op1.fFloatLiteral)
								isJump = TRUE;
							break;
						default:
							break;
						}
					}
				case INSTR_JL:
					{
						switch (op0.iType)
						{
						case OP_TYPE_INT:
							if (op0.iIntLiteral < op1.iIntLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_FLOAT:
							if (op0.fFloatLiteral < op1.fFloatLiteral)
								isJump = TRUE;
							break;
						default:
							break;
						}
					}
				case INSTR_JLE:
					{
						switch (op0.iType)
						{
						case OP_TYPE_INT:
							if (op0.iIntLiteral <= op1.iIntLiteral)
								isJump = TRUE;
							break;
						case OP_TYPE_FLOAT:
							if (op0.fFloatLiteral <= op1.fFloatLiteral)
								isJump = TRUE;
							break;
						default:
							break;
						}
					}
				default:
					break;
				}

				if (isJump)
					g_Script.instrStream.iCurrentInstr = nextInstr;
				break;
			}

		case INSTR_PUSH:
			{
				Value value = GetOpValue(0);
				Push(value);
				break;
			}
		case INSTR_POP:
			{
				*GetOpValuePointer(0) = Pop();
				break;
			}

		case INSTR_CALL:
			{
				int targetFuncIndex = GetOpValueAsFuncIndex(0);
				Func targetFunc = g_Script.pFuncTable[targetFuncIndex];

				Value returnAddress;
				returnAddress.iInstrIndex = g_Script.instrStream.iCurrentInstr + 1;
				Push(returnAddress);

				PushFrame(targetFunc.iLocalDataSize + 1);

				Value targetFuncIndexValue;
				targetFuncIndexValue.iFuncIndex = targetFuncIndex;
				SetStackValue(g_Script.stack.iTopIndex - 1, targetFuncIndexValue);
				
				g_Script.instrStream.iCurrentInstr = targetFunc.iEntryPoint;
				break;
			}
		case INSTR_RET:
			{
				Value targetFuncIndexValue = Pop();
				Func targetFunc = g_Script.pFuncTable[targetFuncIndexValue.iFuncIndex];
				int frameIndex = targetFuncIndexValue.iOffsetIndex;

				Value returnAddress = GetStackValue(g_Script.stack.iTopIndex - (targetFunc.iLocalDataSize + 1));
				
				PopFrame(targetFunc.iStackFrameSize);

				g_Script.stack.iFrameIndex = frameIndex;
				g_Script.instrStream.iCurrentInstr = returnAddress.iInstrIndex;

				break;
			}
		case INSTR_CALLHOST:
			{
				break;
			}

		case INSTR_PAUSE:
			{
				break;
			}
		case INSTR_EXIT:
			{
				Value exitCodeValue = GetOpValue(0);
				int exitCode = exitCodeValue.iIntLiteral;

				isExitExeLoop = TRUE;

				break;
			}

		default:
			break;
		}

		printf("\n");

		if (currentInstr == g_Script.instrStream.iCurrentInstr)
			g_Script.instrStream.iCurrentInstr++;

		if (isExitExeLoop)
			break;
	}
	
}

void CopyValue(Value* dest, Value source)
{
	if (dest->iType == OP_TYPE_STRING_INDEX)
		free(dest->strStringLiteral);

	*dest = source;

	if (source.iType == OP_TYPE_STRING_INDEX)
	{
		dest->strStringLiteral = (char*) malloc(strlen(source.strStringLiteral) + 1);
		strcpy(dest->strStringLiteral, source.strStringLiteral);
	}

}

int ValueToInt(Value value)
{
	switch (value.iType)
	{
	case OP_TYPE_INT:
		return value.iIntLiteral;
	case OP_TYPE_FLOAT:
		return (int)value.fFloatLiteral;
	case OP_TYPE_STRING_INDEX:
		return atoi(value.strStringLiteral);
	default:
		return 0;
	}
}

float ValueToFloat(Value value)
{
	switch (value.iType)
	{
	case OP_TYPE_INT:
		return (float) value.iIntLiteral;
	case OP_TYPE_FLOAT:
		return value.fFloatLiteral;
	case OP_TYPE_STRING_INDEX:
		return atof(value.strStringLiteral);
	default:
		return 0;
	}
}

char* ValueToString(Value value)
{
	char* str = NULL;
	if (value.iType != OP_TYPE_STRING_INDEX)
		str = (char*) malloc(MAX_VALUE_STRING_SIZE + 1);

	switch (value.iType)
	{
	case OP_TYPE_INT:
		_itoa(value.iIntLiteral, str, 10);
		return str;
	case OP_TYPE_FLOAT:
		sprintf(str, "%f", value.fFloatLiteral);
		return str;
	case OP_TYPE_STRING_INDEX:
		return value.strStringLiteral;
	default:
		return NULL;
	}
}

Value GetStackValue(int index)
{
	return g_Script.stack.pElement[GET_STACK_INDEX(index)];
}

void SetStackValue(int index, Value value)
{
	g_Script.stack.pElement[GET_STACK_INDEX(index)] = value;
}


Value GetOpValue(int opIndex)
{
	int currentInstr = g_Script.instrStream.iCurrentInstr;
	Value opValue = g_Script.instrStream.pInstr[currentInstr].pOplist[opIndex];

	switch (opValue.iType)
	{
	case OP_TYPE_ABS_STACK_INDEX:
	case OP_TYPE_REL_STACK_INDEX:
		int index = GetOpValueAsStackIndex(opIndex);
		return GetStackValue(index);

	case OP_TYPE_REG:
		return g_Script._RetVal;

	default:
		return opValue;
	}
}

Value* GetOpValuePointer(int opIndex)
{
	int opType = GetOpType(opIndex);
	switch (opType)
	{
	case OP_TYPE_ABS_STACK_INDEX:
	case OP_TYPE_REL_STACK_INDEX:
		int index = GetOpValueAsStackIndex(opIndex);
		return &g_Script.stack.pElement[GET_STACK_INDEX(index)];

	case OP_TYPE_REG:
		return &g_Script._RetVal;

	default:
		return NULL;
	}
}

int GetOpType(int opIndex)
{
	return GetOpValue(opIndex).iType;
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
	int currentInstr = g_Script.instrStream.iCurrentInstr;
	Value opValue = g_Script.instrStream.pInstr[currentInstr].pOplist[opIndex];

	switch (opValue.iType)
	{
	case OP_TYPE_ABS_STACK_INDEX:
		return opValue.iStackIndex;
	case OP_TYPE_REL_STACK_INDEX:
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
	
	return g_Script.hostAPICallTable.ppStrCalls[hostAPIIndex];
}

void Push(Value value)
{
	int topIndex = g_Script.stack.iTopIndex;
	g_Script.stack.pElement[topIndex] = value;
	g_Script.stack.iTopIndex++;
}
Value Pop()
{
	g_Script.stack.iTopIndex--;
	int topIndex = g_Script.stack.iTopIndex;

	Value value;
	CopyValue(&value, g_Script.stack.pElement[topIndex]);

	return value;
}
void PushFrame(int size)
{
	g_Script.stack.iTopIndex += size;
	g_Script.stack.iFrameIndex = g_Script.stack.iTopIndex;
}
void PopFrame(int size)
{
	g_Script.stack.iTopIndex -= size;
}

int GetTime()
{
	return GetTickCount();
}