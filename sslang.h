#ifndef SSLANG_H
#define SSLANG_H

typedef unsigned int uint;
typedef int Token;

namespace _asm_
{
	typedef int OpType;
	
	struct Oprand
	{
		int iType;
		union
		{
			int   iIntLiteral;
			float fFloatLiteral;
			int   iStringTableIndex;
			int   iStackIndex;
			int   iInstrIndex;
			int   iFuncIndex;
			int   iHostAPICallIndex;
			int   iReg;
		};
		int iOffsetIndex;
	};

	struct Instruction
	{
		int     iOpcode;
		int     iOpCount;
		Oprand* pOplist;
	};

	struct InstrLookup
	{
		char    strMnemonic[MAX_INDENT_SIZE];
		int     iOpcode;
		int     iOpcount;
		OpType* pOplist;
	};

	struct Header
	{
		int iStackSize;
		int iGlobalDataSize;
		int iIsMainFuncPresent;
		int iMainFuncIndex;
	};

	struct FuncNode
	{
		int  iIndex;
		char strName[MAX_INDENT_SIZE];
		int  iEntryPoint;
		int  iParamCount;
		int  iLocalDataSize;
	};

	struct SymbolNode
	{
		int  iIndex;
		char strIdentifier[MAX_INDENT_SIZE];
		int  iSize;
		int  iStackIndex;
		int  iFuncIndex;
	};

	struct LabelNode
	{
		int  iIndex;
		char strIndentifier[MAX_INDENT_SIZE];
		int  iTargetIndex;
		int  iFuncIndex;
	};

	struct Lexer
	{
		int   iCurrentSourceLine;
		uint  iIndex0;
		uint  iIndex1;
		Token currentToken;
		char  pCurrentLexeme[MAX_LEXEME_SIZE];
		int   iState;
	};

	struct ASM
	{
		char         exeFileName[MAX_FILENAME_SIZE];
		char         exeInfoFilename[MAX_FILENAME_SIZE];

		FILE*        sourceFile;
		char         sourceFileName[MAX_FILENAME_SIZE];
		int          sourceCodeLines;
		char**       sourceCode;

		LinkList     functionTable;
		LinkList     stringTable;
		LinkList     labelTable;
		LinkList     symbolTable;
		LinkList     hostAPICallTable;

		Lexer        lexer;
		Header       header;
		Instruction* instrStream;

		int          instrStreamSize;
		int          isSetStackSizeFound;
		int          currentInstrIndex;

		InstrLookup  instrLookup[MAX_INSTR_LOOKUP_COUNT];
	};

	int AddString(LinkList* pStringTable, char* str);

	FuncNode* GetFunctionByName(LinkList* pFunctionTable, char* name);
	int AddFunction(LinkList* pFunctionTable, char* name, int entryPoint);
	void SetFunctionInfo(char* name, int paramNum, int localDataSize);

	SymbolNode *GetSymbol(LinkList* pSymbolTable, char *identifier, int funcIndex);
	int AddSymbol(LinkList* pSymbolTable, char *identifier, int size, int stackIndex, int funcIndex);
	int GetStackIndexByIdent(char *identifier, int funcIndex);
	int GetSizeByIdent(char *identifier, int funcIndex);

	LabelNode *GetLabel(LinkList* pLabelTable, char *identifier, int funcIndex);
	int AddLabel(LinkList* pLabelTable, char *identifier, int targetIndex, int funcIndex);

	int AddInstrLookup(InstrLookup instrLookupTable[], char* mnemonic, int opCode, int opCount);
	void setOpType(InstrLookup instrLookup[], int instrIndex, int opIndex, OpType opType);
	int GetInstruction(InstrLookup instrLookupTable[], char* name, InstrLookup* instrLookup);
}

namespace _vm
{
	struct Value
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
	};

	struct Instruction
	{
		int    iOpcode;
		int    iOpCount;
		Value* pOplist;
	};

	struct InstrStream
	{
		Instruction* pInstr;
		int          iSize;
		int          iCurrentInstr;
	};

	struct RuntimeStack
	{
		Value* pElement;
		int    iSize;
		int    iTopIndex;
		int    iFrameIndex;
	};

	struct Function
	{
		int iEntryPoint;
		int iParamCount;
		int iLocalDataSize;
		int iStackFrameSize;
	};

	struct HostAPICallTable
	{
		char** ppStrCalls;
		int    iSize;
	};

	struct Script
	{
		int iGlobalDataSize;
		int iIsMainFuncExist;
		int iMainFuncIndex;

		int iIsPaused;
		int iPauseEndTime;

		Value _RetVal;

		InstrStream      instrStream;
		RuntimeStack     stack;
		Function*        pFuncTable;
		HostAPICallTable hostAPICallTable;
	};
}


namespace _cl
{
	struct OpState
    {
    	char cChar;
    	int  iSubStateIndex;
    	int  iSubStateCount;
    	int  iIndex;
    };

	struct Lexer
	{
		char* stringSource;
        char  currentLexeme[MAX_LEXEME_SIZE];
        int   currentLexemeStart;
        int   currentLexemeEnd;
        int   currentOp;
	};

	struct Header
	{
		int iStackSize;
		int iIsMainFuncExist;
		int iMainFuncIndex;
	};

	struct Compiler
	{
		LinkList sourceCode;
		LinkList functionTable;
		LinkList symbolTable;
		LinkList stringTable;

		Header   scriptHeader;

	};
}










#endif