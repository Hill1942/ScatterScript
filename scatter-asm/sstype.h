#ifndef SSTYPE_H
#define SSTYPE_H

#define MAX_INDENT_SIZE 4096

typedef struct _Op
{
	int iType;
	union
	{
		int iIntLiteral;
		float fFloatLiteral;
		int   iStringTableIndex;
		int   iStackIndex;
		int   iInstrIndex;
		int   iFuncIndex;
		int   iHostAPICallIndex;
		int   iReg;
	};
	int iOffsetIndex;
}Op;

typedef struct _Instr
{
	int iOpcode;
	int iOpCount;
	Op* pOplist;
}Instr;

typedef struct _ScriptHeader
{
	int iStackSize;
	int iGlobalDataSize;
	int iIsMainFuncPresent;
	int iMainFuncIndex;
}ScriptHeader;

typedef struct _FuncNode
{
	int  iIndex;
	char strName[MAX_INDENT_SIZE];
	int  iEntryPoint;
	int  iParamCount;
	int  iLocalDataSize;
}FuncNode;

typedef struct _SymbolNode
{
	int  iIndex;
	char strIdentifier[MAX_INDENT_SIZE];
	int  iSize;
	int  iStackIndex;
	int  iFuncIndex;
}SymbolNode;

typedef struct _LabelNode
{
	int  iIndex;
	char strIndentifier[MAX_INDENT_SIZE];
	int  iTargetIndex;
	int  iFuncIndex;
}LabelNode;

int  AddString(LinkList* pList, char* str);

FuncNode* GetFunctionByName(char* name);
int  AddFunction(char* name, int entryPoint);
void SetFunctionInfo(char* name, int paramNum, int localDataSize);

SymbolNode* GetSymbolByIdent(char* identifier, int funcIndex);
int AddSymbol(char* identifier, int size, int stackIndex, int funcIndex);
int GetStackIndexByIdent(char* identifier, int funcIndex);
int GetSizeByIdent(char* identifier, int funcIndex);

LabelNode* GetLabelByIdent(char* identifier, int funcIndex);
int AddLabel(char* identifier, int targetIndex, int funcIndex);


#endif