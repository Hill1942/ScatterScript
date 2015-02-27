#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssasm_pre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"


/*
 * @author  Yang Kaidi
 * @version 1.0
 * @date    2015-02-16
 * @{
 */

extern LinkList g_FunctionTable;
extern LinkList g_StringTable;
extern LinkList g_LabelTable;
extern LinkList g_SymbolTable;

extern InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

/*
 * @defgroup
 * @{
 */

/*
 *  Add string into linklist
 *  @param [out] pList the linklist which will be inserted into
 *  @param [in] str a string to be inserted
 *  @return if str has bean already in the linklist, return the index of the string in the list.
 *  Otherwise, insert the string and return the new index;
 */
int AddString(LinkList* pList, char* str)
{
	//Look through the linklist to find whether the string has been already in the list.
	LinkListNode* pNode = pList->pHead;
	for (int i = 0; i < pList->iNodeCount; i++)
	{
		if (strcmp((char*)pNode->pData, str) == 0)
			return i;
		pNode = pNode->pNext;
	}
	//Not in the list, so allocate space and add string into the list.
	char* newStr =(char*) malloc(strlen(str) + 1);
	strcpy(newStr, str);

	return AddNode(pList, newStr);
}

/*
 *  Get a function's name according to its name
 *  @param [in] name function's name
 *  @return function node represented by the name
 */
FuncNode* GetFunctionByName(char* name)
{
	if (g_FunctionTable.iNodeCount == 0)
		return NULL;

	//Look through the function table to find the specific function
	LinkListNode* pCurrentNode = g_FunctionTable.pHead;
	for (int i = 0; i < g_FunctionTable.iNodeCount; i++)
	{
		FuncNode* pCurrentFunc = (FuncNode*)pCurrentNode->pData;
		if (strcmp(pCurrentFunc->strName, name) == 0)
			return pCurrentFunc;
		//Move to next node in table
		pCurrentNode = pCurrentNode->pNext;
	}
	//Can not find the specific function
	return NULL;
}/*
 *  Add Function into the global function table
 *  @param [in] name function's name
 *  @param [in] entryPoint function's entryPoint
 *  @return if the function has bean already in the table, return -1.
 *  Otherwise, insert a new function according to the name and entryPoint,
 *  and return the new index;
 */
int AddFunction(char* name, int entryPoint)
{
	//if the function already exists in table, return -1
	if (GetFunctionByName(name))
		return -1;
	//create a new function according to the name and entryPoint;
	FuncNode* newFunc = (FuncNode*) malloc(sizeof(FuncNode));
	strcpy(newFunc->strName, name);
	newFunc->iEntryPoint = entryPoint;

	//Get the index of the function in the global function table;
	int index = AddNode(&g_FunctionTable, newFunc);
	newFunc->iIndex = index;

	return index;
}

/*
 *  Set the other info of a function node
 *  @param [in] name function's name
 *  @param [in] paramNum number of the function's parameter
 *  @param [in] function's data size
 *  @return void
 */
void SetFunctionInfo(char* name, int paramNum, int localDataSize)
{
	//Get the function according to the name;
	FuncNode* funcNode       = GetFunctionByName(name);
	funcNode->iParamCount    = paramNum;
	funcNode->iLocalDataSize = localDataSize;
}

SymbolNode *GetSymbolByIdent(char *identifier, int funcIndex)
{
	if (g_SymbolTable.iNodeCount == 0)
		return NULL;
	LinkListNode* throughNode = g_SymbolTable.pHead;
	for (int i = 0; i < g_SymbolTable.iNodeCount; i++)
	{
		SymbolNode* currentSymbol = (SymbolNode*) throughNode->pData;
		if (strcmp(currentSymbol->strIdentifier, identifier) == 0)
			if (currentSymbol->iFuncIndex == funcIndex ||
					currentSymbol->iStackIndex >= 0)
				return currentSymbol;
		throughNode = throughNode->pNext;
	}
	return NULL;
}

int AddSymbol(char *identifier, int size, int stackIndex, int funcIndex) {
	if (GetSymbolByIdent(identifier, funcIndex))
		return -1;
	SymbolNode* newSymbol  = (SymbolNode*) malloc(sizeof(SymbolNode));
	strcpy(newSymbol->strIdentifier, identifier);
	newSymbol->iSize       = size;
	newSymbol->iStackIndex = stackIndex;
	newSymbol->iFuncIndex  = funcIndex;
	newSymbol->iIndex      = AddNode(&g_SymbolTable, newSymbol);

	return newSymbol->iIndex;
}

int GetStackIndexByIdent(char *identifier, int funcIndex) {
	return GetSymbolByIdent(identifier, funcIndex)->iStackIndex;
}

int GetSizeByIdent(char *identifier, int funcIndex) {
	return GetSymbolByIdent(identifier, funcIndex)->iSize;
}

LabelNode *GetLabelByIdent(char *identifier, int funcIndex)
{
	if (g_LabelTable.iNodeCount == 0)
		return NULL;

	LinkListNode* throughNode = g_LabelTable.pHead;
	for (int i = 0; i < g_LabelTable.iNodeCount; i++)
	{
		LabelNode* currentLabel = (LabelNode*) throughNode->pData;
		if (strcpy(currentLabel->strIndentifier, identifier) == 0 &&
				currentLabel->iFuncIndex == funcIndex)
			return currentLabel;
		throughNode = throughNode->pNext;
	}
	return NULL;
}

int AddLabel(char *identifier, int targetIndex, int funcIndex) {
	if (GetLabelByIdent(identifier, funcIndex))
		return -1;

	LabelNode* newLabel    = (LabelNode*) malloc(sizeof(LabelNode));
	strcpy(newLabel->strIndentifier, identifier);
	newLabel->iFuncIndex   = funcIndex;
	newLabel->iTargetIndex = targetIndex;
	newLabel->iIndex       = AddNode(&g_LabelTable, newLabel);

	return newLabel->iIndex;
}

int GetInstrByMnemonic(char* mnemonic, InstrLookup* instrLookup)
{
	for (int i = 0; i < MAX_INSTR_LOOKUP_COUNT; i++)
	{
		if (strcmp(g_InstrTable[i].strMnemonic, mnemonic) == 0)
		{
			*instrLookup = g_InstrTable[i];
			return TRUE;
		}
	}
	return FALSE;
}

int AddInstrLookup(char* mnemonic, int opCode, int opCount)
{
	static int instrIndex = 0;
	if (instrIndex > MAX_INSTR_LOOKUP_COUNT)
		return -1;

	strcpy(g_InstrTable[instrIndex].strMnemonic, mnemonic);
	//strtoupper(g_InstrTable[instrIndex].strMnemonic);
	g_InstrTable[instrIndex].iOpcode  = opCode;
	g_InstrTable[instrIndex].iOpcount = opCount;

	g_InstrTable[instrIndex].pOplist  = (OpType*) malloc(opCount * sizeof(OpType));

	int returnIndex = instrIndex;

	instrIndex++;

	return returnIndex;
}

void setOpType(int instrIndex, int opIndex, OpType opType)
{
	g_InstrTable[instrIndex].pOplist[opIndex] = opType;
}

/** @}*/ // Function Definition

/** @}*/ // File Type End



















