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

int  AddString(LinkList* pList, char* str);
int  AddFunction(char* name, int entryPoint);

#endif