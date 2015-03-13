#ifndef SSLANG_VM_H
#define SSLANG_VM_H

typedef struct _Value
{
	int iType;
	union
	{
		int   iIntLiteral;
		float fFloatLiteral;
		char* strStringLiteral;
		int   iStackIndex;
		int   iInstrIndex;
		int   iFuncIndex;
		int   iHostAPICallIndex;
		int   iReg;
	};
	int iOffsetIndex;
}Value;

typedef struct _VM_Instr
{
	int iOpcode;
	int iOpCount;
	Value* pOplist;
}VM_Instr;

typedef struct _InstrStream
{
	VM_Instr* pInstr;
	int iSize;
	int iCurrentInstr;
}InstrStream;

typedef struct _RuntimeStack
{
	Value* pElement;
	int iSize;
	int iTopIndex;
	int iFrameIndex;
}RuntimeStack;

typedef struct _Func
{
	int iEntryPoint;
	int iParamCount;
	int iLocalDataSize;
	int iStackFrameSize;
}Func;

typedef struct _HostAPICallTable
{
	char** ppStrCalls;
	int    iSize;
}HostAPICallTable;

typedef struct _Script
{
	int iGlobalDataSize;
	int iIsMainFuncExist;
	int iMainFuncIndex;

	int iIsPaused;
	int iPauseEndTime;

	Value _RetVal;

	InstrStream instrStream;
	RuntimeStack stack;
	Func* pFuncTable;
	HostAPICallTable hostAPICallTable;
}Script;
#endif