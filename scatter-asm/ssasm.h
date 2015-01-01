#ifndef  SSASM_H
#define SSAMS_H

int IsCharNumeric(char cChar);
int IsCharWhiteSpace(char cChar);
int IsCharIdent(char cChar);
int IsCharDelimiter(char cChar);

int IsStringInt(char* str);
int IsStringFloat(char* str);
int IsStringWhitespace(char* str);
int IsStringIdent(char* str);

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
	int iIndex;
	char strName[MAX_INDENT_SIZE];
	int  iEntryPoint;
	int iParamCount;
	int iLocalDataSize;
};




#endif