#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"

extern _asm_::ASM sasm;
extern _cl::Compiler compiler;

namespace _asm_
{
    int AddString(LinkList* pStringTable, char* str)
    {
    	//Look through the linklist to find whether the string has been already in the list.
    	LinkListNode* pNode = pStringTable->pHead;
    	for (int i = 0; i < pStringTable->iNodeCount; i++)
    	{
    		if (strcmp((char*)pNode->pData, str) == 0)
    			return i;
    		pNode = pNode->pNext;
    	}
    	//Not in the list, so allocate space and add string into the list.
    	char* newStr =(char*) malloc(strlen(str) + 1);
    	strcpy(newStr, str);
    
    	return AddNode(pStringTable, newStr);
    }

	char* GetString(LinkList* pStringTable, int index)
	{
		LinkListNode* pNode = pStringTable->pHead;
		for (int i = 0; i < pStringTable->iNodeCount; i++)
		{
			if (i == index)
				return (char*) pNode->pData;
			pNode = pNode->pNext;
		}

		return NULL;
	}

	FuncNode* GetFunctionByName(LinkList* pFunctionTable, char* name)
	{
		if (pFunctionTable->iNodeCount == 0)
			return NULL;

		//Look through the function table to find the specific function
		LinkListNode* pCurrentNode = pFunctionTable->pHead;
		for (int i = 0; i < pFunctionTable->iNodeCount; i++)
		{
			FuncNode* pCurrentFunc = (FuncNode*)pCurrentNode->pData;
			if (strcmp(pCurrentFunc->strName, name) == 0)
				return pCurrentFunc;
			//Move to next node in table
			pCurrentNode = pCurrentNode->pNext;
		}
		//Can not find the specific function
		return NULL;
	}

	int AddFunction(LinkList* pFunctionTable, char* name, int entryPoint)
	{
		//if the function already exists in table, return -1
		if (GetFunctionByName(&sasm.functionTable, name))
			return -1;
		//create a new function according to the name and entryPoint;
		FuncNode* newFunc = (FuncNode*) malloc(sizeof(FuncNode));
		strcpy(newFunc->strName, name);
		newFunc->iEntryPoint = entryPoint;

		//Get the index of the function in the global function table;
		int index = AddNode(pFunctionTable, newFunc);
		newFunc->iIndex = index;

		return index;
	}

	void SetFunctionInfo(char* name, int paramNum, int localDataSize)
	{
		//Get the function according to the name;
		FuncNode* funcNode       = GetFunctionByName(&sasm.functionTable, name);
		funcNode->iParamCount    = paramNum;
		funcNode->iLocalDataSize = localDataSize;
	}

	SymbolNode *GetSymbol(LinkList* pSymbolTable, char *identifier, int funcIndex)
	{
		if (pSymbolTable->iNodeCount == 0)
			return NULL;
		LinkListNode* throughNode = pSymbolTable->pHead;
		for (int i = 0; i < pSymbolTable->iNodeCount; i++)
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

	int AddSymbol(LinkList* pSymbolTable, char *identifier, int size, int stackIndex, int funcIndex) {
		if (GetSymbol(&sasm.symbolTable, identifier, funcIndex))
			return -1;
		SymbolNode* newSymbol  = (SymbolNode*) malloc(sizeof(SymbolNode));
		strcpy(newSymbol->strIdentifier, identifier);
		newSymbol->iSize       = size;
		newSymbol->iStackIndex = stackIndex;
		newSymbol->iFuncIndex  = funcIndex;
		newSymbol->iIndex      = AddNode(pSymbolTable, newSymbol);

		return newSymbol->iIndex;
	}

	int GetStackIndexByIdent(char *identifier, int funcIndex) {
		return GetSymbol(&sasm.symbolTable, identifier, funcIndex)->iStackIndex;
	}

	int GetSizeByIdent(char *identifier, int funcIndex) {
		return GetSymbol(&sasm.symbolTable, identifier, funcIndex)->iSize;
	}

	LabelNode *GetLabel(LinkList* pLabelTable, char *identifier, int funcIndex)
	{
		if (pLabelTable->iNodeCount == 0)
			return NULL;

		LinkListNode* throughNode = pLabelTable->pHead;
		for (int i = 0; i < pLabelTable->iNodeCount; i++)
		{
			LabelNode* currentLabel = (LabelNode*) throughNode->pData;
			if (strcmp(currentLabel->strIndentifier, identifier) == 0 &&
				currentLabel->iFuncIndex == funcIndex)
				return currentLabel;
			throughNode = throughNode->pNext;
		}
		return NULL;
	}

	int AddLabel(LinkList* pLabelTable, char *identifier, int targetIndex, int funcIndex) {
		if (GetLabel(&sasm.labelTable, identifier, funcIndex))
			return -1;

		LabelNode* newLabel    = (LabelNode*) malloc(sizeof(LabelNode));
		strcpy(newLabel->strIndentifier, identifier);
		newLabel->iFuncIndex   = funcIndex;
		newLabel->iTargetIndex = targetIndex;
		newLabel->iIndex       = AddNode(pLabelTable, newLabel);

		return newLabel->iIndex;
	}

	int GetInstruction(InstrLookup instrLookupTable[], char* name, InstrLookup* instrLookup)
	{
		for (int i = 0; i < MAX_INSTR_LOOKUP_COUNT; i++)
		{
			if (strcmp(instrLookupTable[i].strMnemonic, name) == 0)
			{
				*instrLookup = instrLookupTable[i];
				return TRUE;
			}
		}
		return FALSE;
	}

	int AddInstrLookup(InstrLookup instrLookupTable[], char* mnemonic, int opCode, int opCount)
	{
		static int instrIndex = 0;
		if (instrIndex > MAX_INSTR_LOOKUP_COUNT)
			return -1;

		strcpy(instrLookupTable[instrIndex].strMnemonic, mnemonic);
		instrLookupTable[instrIndex].iOpcode  = opCode;
		instrLookupTable[instrIndex].iOpcount = opCount;

		instrLookupTable[instrIndex].pOplist  = (OpType*) malloc(opCount * sizeof(OpType));

		int returnIndex = instrIndex;

		instrIndex++;

		return returnIndex;
	}

	void setOpType(InstrLookup instrLookupTable[], int instrIndex, int opIndex, OpType opType)
	{
		instrLookupTable[instrIndex].pOplist[opIndex] = opType;
	}

}

namespace _cl
{
	SymbolNode* GetSymbol(LinkList* pSymbolTable, int index)
	{
		if (pSymbolTable->iNodeCount == 0)
		{
			return NULL;
		}

		LinkListNode* pCurrentNode = pSymbolTable->pHead;
		for (int i = 0; i < pSymbolTable->iNodeCount; i++)
		{
			SymbolNode* pSymbol = (SymbolNode*) pCurrentNode->pData;
			if (pSymbol->iIndex == index)
				return pSymbol;

			pCurrentNode =pCurrentNode->pNext;
		}

		return NULL;
	}
	SymbolNode* GetSymbol(LinkList* pSymbolTable, char* identifier, int scope)
	{
		SymbolNode* pSymbol;

		for (int i = 0; i < pSymbolTable->iNodeCount; i++)
		{
			SymbolNode* pSymbol = GetSymbol(pSymbolTable, i);
			if (pSymbol && 
				strcmp(pSymbol->strIdentifier, identifier) == 0 &&
				(pSymbol->iScope == scope || pSymbol->iScope == 0))
				return pSymbol;
		}

		return NULL;
	}
	int GetSymbolSize(LinkList* pSymbolTable, char* identifer, int scope)
	{
		SymbolNode* pSymbol = GetSymbol(pSymbolTable, identifer, scope);
		if (pSymbol != NULL)
			return pSymbol->iSize;

		return -1;
	}
	int AddSymbol(LinkList* pSymbolTable, int size, int scope, int type, char* identifier)
	{
		if (GetSymbol(pSymbolTable, identifier, scope))
			return -1;

		SymbolNode* pNewSymbol = (SymbolNode*) malloc(sizeof(SymbolNode));

		strcpy(pNewSymbol->strIdentifier, identifier);
		pNewSymbol->iSize = size;
		pNewSymbol->iScope = scope;
		pNewSymbol->iType = type;

		int index = AddNode(pSymbolTable, pNewSymbol);
		pNewSymbol->iIndex = index;

		return index;
	}

	FuncNode* GetFunction(LinkList* pFunctionTable, int index)
	{
		if (pFunctionTable->iNodeCount == 0)
		{
			return NULL;
		}

		LinkListNode* pCurrentNode = pFunctionTable->pHead;
		for (int i = 0; i < pFunctionTable->iNodeCount; i++)
		{
			FuncNode* pFunction = (FuncNode*) pCurrentNode->pData;
			if (pFunction->iIndex == index)
				return pFunction;

			pCurrentNode = pCurrentNode->pNext;
		}

		return NULL;
	}
	FuncNode* GetFunction(LinkList* pFunctionTable, char* name)
	{
		FuncNode* pFunction;

		for (int i = 0; i < pFunctionTable->iNodeCount; i++)
		{
			FuncNode* pFunction = GetFunction(pFunctionTable, i);
			if (pFunction && 
				strcmp(pFunction->strName, name) == 0)
				return pFunction;
		}

		return NULL;
	}
	void SetFuncParamCount(LinkList* pFunctionTable, int index, int paramCount)
	{
		FuncNode* pFunction = GetFunction(pFunctionTable, index);
		pFunction->iParamCount = paramCount;
	}
	int AddFunction(LinkList* pFunctionTable, int isHostAPI, char* name)
	{
		if (GetFunction(pFunctionTable, name))
			return -1;

		FuncNode* pNewFunction = (FuncNode*) malloc(sizeof(FuncNode));

		strcpy(pNewFunction->strName, name);
		pNewFunction->iIndex = AddNode(pFunctionTable, pNewFunction) + 1;
		pNewFunction->isHostAPI = isHostAPI;
		pNewFunction->iParamCount = 0;
		pNewFunction->codeStream.iNodeCount = 0;

		if (strcmp(name, CL_KW_MAIN_FUNC_NAME) == 0)
		{
			compiler.scriptHeader.iIsMainFuncExist = TRUE;
			compiler.scriptHeader.iMainFuncIndex = pNewFunction->iIndex;
		}
		return pNewFunction->iIndex;
	}

	char* GetString(LinkList* pStringTable, int index)
	{
		LinkListNode* pNode = pStringTable->pHead;
		for (int i = 0; i < pStringTable->iNodeCount; i++)
		{
			if (i == index)
				return (char*) pNode->pData;
			pNode = pNode->pNext;
		}

		return NULL;
	}
	int AddString(LinkList* pStringTable, char* str)
	{
		LinkListNode* pNode = pStringTable->pHead;
		for (uint i = 0; i < pStringTable->iNodeCount; i++)
		{
			if (strcmp((char*)pNode->pData, str) == 0)
				return i;

			pNode = pNode->pNext;
		}

		char* pStringNode = (char*) malloc(strlen(str) + 1);
		strcpy(pStringNode, str);

		return AddNode(pStringTable, pStringNode);
	}
}

namespace _IL
{
	ILCodeNode* GetILCodeNode(LinkList* pFuncTable, int funcIndex, int instrIndex)
	{
		_cl::FuncNode* pFunc = _cl::GetFunction(pFuncTable, funcIndex);

		if (!pFunc->codeStream.iNodeCount)
			return NULL;

		LinkListNode* pCurrentNode = pFunc->codeStream.pHead;
		for (int i = 0; i < pFunc->codeStream.iNodeCount; i++)
		{
			if (instrIndex == i)
				return (ILCodeNode*) pCurrentNode->pData;

			pCurrentNode = pCurrentNode->pNext;
		}

		return NULL;
	}
	void AddILCodeSourceLine(LinkList* pFuncTable, int funcIndex, char* sourceLine)
	{
		_cl::FuncNode* pFunc = _cl::GetFunction(pFuncTable, funcIndex);
		ILCodeNode* pSourceLineNode = (ILCodeNode*) malloc(sizeof(ILCodeNode));

		pSourceLineNode->iType        = IL_CODE_NODE_SOURCE_LINE;
		pSourceLineNode->strSoureLine = sourceLine;

		AddNode(&pFunc->codeStream, pSourceLineNode);
	}
	int AddILCodeInstr(LinkList* pFuncTable, int funcIndex, int opCode)
	{
		_cl::FuncNode* pFunc = _cl::GetFunction(pFuncTable, funcIndex);
		ILCodeNode* pInstrNode = (ILCodeNode*) malloc(sizeof(ILCodeNode));

		pInstrNode->iType         = IL_CODE_NODE_INSTR;
		pInstrNode->instr.iOpCode = opCode;
		pInstrNode->instr.OpList.iNodeCount = 0;

		int index = AddNode(&pFunc->codeStream, pInstrNode);

		return index;
	}
	Oprand* GetILCodeOprand(ILCodeNode* pInstr, int index)
	{
		if (pInstr->instr.OpList.iNodeCount == 0)
			return NULL;

		LinkListNode* pCurrentNode = pInstr->instr.OpList.pHead;

		for (int i = 0; i < pInstr->instr.OpList.iNodeCount; i++)
		{
			if (index == i)
				return (Oprand*) pCurrentNode->pData;

			pCurrentNode = pCurrentNode->pNext;
		}

		return NULL;
	}
	void AddILCodeOprand(LinkList* pFuncTable, int funcIndex, 
		                 int instrIndex, Oprand value)
	{
		ILCodeNode* pILCodeNode = GetILCodeNode(pFuncTable, funcIndex, instrIndex);

		Oprand* pValue = (Oprand*) malloc(sizeof(Oprand));
		memcpy(pValue, &value, sizeof(Oprand));

		AddNode(&pILCodeNode->instr.OpList, pValue);
	}

	void AddILCodeOprand_Int(LinkList* pFuncTable, int funcIndex, 
		                     int instrIndex, int iValue)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_INT;
		value.iIntLiteral = iValue;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_Float(LinkList* pFuncTable, int funcIndex, 
		                       int instrIndex, float fValue)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_FLOAT;
		value.fFloatLiteral = fValue;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_String(LinkList* pFuncTable, int funcIndex, 
		                        int instrIndex, int stringIndex)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_STRING_INDEX;
		value.iStringIndex = stringIndex;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_Variable(LinkList* pFuncTable, int funcIndex, 
		                     int instrIndex, int symbolIndex)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_VARIABLE;
		value.iSymbolIndex = symbolIndex;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_AbsArrayIndex(LinkList* pFuncTable, int funcIndex, 
		                               int instrIndex, int arrayIndex, int offset)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_ABS_ARRAY_INDEX;
		value.iSymbolIndex = arrayIndex;
		value.iOffset = offset;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_RelArrayIndex(LinkList* pFuncTable, int funcIndex, 
		                               int instrIndex, int arrayIndex, int offsetSymbolIndex)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_REL_ARRAY_INDEX;
		value.iSymbolIndex = offsetSymbolIndex;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_Func(LinkList* pFuncTable, int funcIndex, 
		                      int instrIndex, int opFuncIndex)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_FUNC_INDEX;
		value.iFuncIndex = opFuncIndex;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_Reg(LinkList* pFuncTable, int funcIndex, 
		                     int instrIndex, int regCode)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_REG;
		value.iSymbolIndex = regCode;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}
	void AddILCodeOprand_JumpTarget(LinkList* pFuncTable, int funcIndex, 
		                            int instrIndex, int targetIndex)
	{
		Oprand value;
		value.iType = IL_OPRAND_TYPE_JUMP_TARGET_INDEX;
		value.iJumpTargetIndex = targetIndex;

		AddILCodeOprand(pFuncTable, funcIndex, instrIndex, value);
	}

	int GetNextJumpTargetIndex()
	{
		return compiler.jumpTargetIndex++;
	}
	void AddILCodeJumpTarget(LinkList* pFuncTable, int funcIndex, int targetIndex)
	{
		_cl::FuncNode* pFunc = _cl::GetFunction(pFuncTable, funcIndex);
		ILCodeNode* pSourceLineNode = (ILCodeNode*) malloc(sizeof(ILCodeNode));

		pSourceLineNode->iType            = IL_CODE_NODE_JUMP_TARGET;
		pSourceLineNode->iJumpTargetIndex = targetIndex;

		AddNode(&pFunc->codeStream, pSourceLineNode);
	}

}
























