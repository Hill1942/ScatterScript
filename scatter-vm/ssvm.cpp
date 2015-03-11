#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
				case INSTR_DIV:
				case INSTR_MOD:
				case INSTR_EXP:

				case INSTR_AND:
				case INSTR_OR:
				case INSTR_XOR:
				case INSTR_SHL:
				case INSTR_SHR:
				default:
					break;
				}
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

void CopyValue(Value* dest, Value value)
{
	*dest = value;
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

int GetOpType(int opIndex);
Value GetOpValue(int opIndex)
{
	int currentInstr = g_Script.instrStream.iCurrentInstr;
	Value opValue = g_Script.instrStream.pInstr[currentInstr].pOplist[opIndex];

	switch (opValue.iType)
	{
		
	
	}
	return opValue;
}
Value* GetOpValuePointer(int opIndex)
{
	return NULL;
}
int GetOpValueAsInt(int opIndex)
{
	return 0;
}
float GetOpValueAsFloat(int opIndex)
{
	return 0;
}
char* GetOpValueAsString(int opIndex)
{
	return 0;
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
		break;


	}
	return 0;
}
int GetOpValueAsInstrIndex(int opIndex)
{
	return 0;
}
int GetOpValueAsFuncIndex(int opIndex)
{
	return 0;
}
char* GetOpValueASHostAPI(int opIndex)
{
	return NULL;
}