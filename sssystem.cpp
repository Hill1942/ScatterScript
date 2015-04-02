#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sslexeme.h"
#include "sssystem.h"

extern _asm_::ASM sasm;
extern _vm::Script vm_script;
extern _cl::Compiler compiler;

namespace _asm_
{
	void Init()
    {
    	InitInstrTable();
    
    	InitLinkList(&sasm.symbolTable);
    	InitLinkList(&sasm.labelTable);
    	InitLinkList(&sasm.functionTable);
    	InitLinkList(&sasm.stringTable);
    	InitLinkList(&sasm.hostAPICallTable);
    }
    void InitInstrTable()
    {
    	int instrIndex;
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "mov", ASM_INSTR_MOV, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    							 ASM_OPRAND_FLAG_TYPE_STRING |
    							 ASM_OPRAND_FLAG_TYPE_MEM_REF |
    							 ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "add", ASM_INSTR_AND, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "sub", ASM_INSTR_SUB, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "mul", ASM_INSTR_MUL, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "div", ASM_INSTR_DIV, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "mod", ASM_INSTR_MOD, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "exp", ASM_INSTR_EXP, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "neg", ASM_INSTR_NEG, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "inc", ASM_INSTR_INC, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "dec", ASM_INSTR_DEC, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "and", ASM_INSTR_AND, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "or", ASM_INSTR_OR, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "xor", ASM_INSTR_XOR, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "not", ASM_INSTR_NOT, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "shl", ASM_INSTR_SHL, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "shr", ASM_INSTR_SHR, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "concat", ASM_INSTR_CONCAT, 2);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    							 ASM_OPRAND_FLAG_TYPE_STRING);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "getchar", ASM_INSTR_GETCHAR, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    		                     ASM_OPRAND_FLAG_TYPE_STRING);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    		                     ASM_OPRAND_FLAG_TYPE_INT);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "setchar", ASM_INSTR_SETCHAR, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    							 ASM_OPRAND_FLAG_TYPE_INT);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    		                     ASM_OPRAND_FLAG_TYPE_STRING);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "jmp", ASM_INSTR_JMP, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "je", ASM_INSTR_JE, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "jne", ASM_INSTR_JNE, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "jg", ASM_INSTR_JG, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "jl", ASM_INSTR_JL, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "jge", ASM_INSTR_JGE, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "jle", ASM_INSTR_JLE, 3);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(sasm.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "push", ASM_INSTR_PUSH, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "pop", ASM_INSTR_POP, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "call", ASM_INSTR_CALL, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_FUNC_NAME);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "ret", ASM_INSTR_RET, 0);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "callhost", ASM_INSTR_CALLHOST, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_HOST_API_CALL);
    
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "pause", ASM_INSTR_PAUSE, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "exit", ASM_INSTR_EXIT, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(sasm.instrLookup, "_print", ASM_INSTR_PRINT, 1);
    	setOpType(sasm.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    }
    
    void Exit()
    {
    	ShutDown();
    	exit(0);
    }
    
    void ShutDown()
    {
    	for (int i = 0; i < sasm.sourceCodeLines; i++)
    	{
    		free(sasm.sourceCode[i]);
    	}
    	free(sasm.sourceCode);
    }
    
    void ExitOnError(const char* errorMessage)
    {
        printf("Fatal Error: %s.\n", errorMessage);
        Exit();
    }
    
    void ExitOnCodeError(const char* errorMessage)
    {
    	printf("Error: %s.\n\n", errorMessage);
    	printf("Line %d\n", sasm.lexer.iCurrentSourceLine);
    
    	char sourceLine[MAX_SOURCE_LINE_SIZE];
    	strcpy(sourceLine, sasm.sourceCode[sasm.lexer.iCurrentSourceLine]);
    
    	for (unsigned int i = 0; i < strlen(sourceLine); i++)
    		if (sourceLine[i] == '\t')
    			sourceLine[i] = ' ';
    
    	printf("%s", sourceLine);
    
    	for (unsigned int i = 0; i < sasm.lexer.iIndex0; i++)
    		printf(" ");
    	printf("^\n");
    
    	printf("Cound not assemble exe file\n");
    
    	Exit();
    }
    
    void ExitOnCharExpectedError(char c)
    {
    	char* errorMsg = (char*) malloc(strlen("' ' expected"));
    	sprintf(errorMsg, "'%c' expected", c);
    
    	ExitOnCodeError(errorMsg);
    }
}

namespace _vm
{
	void Init()
    {
    	vm_script.iIsMainFuncExist = FALSE;
    	vm_script.iIsPaused = FALSE;
    
    	vm_script.instrStream.pInstr = NULL;
    	vm_script.stack.pElement = NULL;
    	vm_script.pFuncTable = NULL;
    	vm_script.hostAPICallTable.ppStrCalls = NULL;
    }
    
    void ShutDown()
    {
    	for (int i = 0; i < vm_script.instrStream.iSize; i++)
    	{
    		int opCount = vm_script.instrStream.pInstr[i].iOpCount;
    		Value* pOplist = vm_script.instrStream.pInstr[i].pOplist;
    
    		for (int j = 0; j < opCount; j++)
    			if (pOplist[j].iType == ASM_OPRAND_TYPE_STRING_INDEX)
    				free(pOplist[j].strStringLiteral);
    	}
    
    	if (vm_script.instrStream.pInstr)
    		free(vm_script.instrStream.pInstr);
    
    	for (int i = 0; i < vm_script.stack.iSize; i++)
    		if (vm_script.stack.pElement[i].iType == ASM_OPRAND_TYPE_STRING_INDEX)
    			free(vm_script.stack.pElement[i].strStringLiteral);
    
    	if (vm_script.stack.pElement)
    		free(vm_script.stack.pElement);
    
    	if (vm_script.pFuncTable)
    		free(vm_script.pFuncTable);
    
    	for (int i = 0; i < vm_script.hostAPICallTable.iSize; i++)
    		if (vm_script.hostAPICallTable.ppStrCalls[i])
    			free(vm_script.hostAPICallTable.ppStrCalls[i]);
    
    	if (vm_script.hostAPICallTable.ppStrCalls)
    		free(vm_script.hostAPICallTable.ppStrCalls);
    }
}

namespace _cl
{
	void ExitOnError(char* strErrorMsg)
	{
		printf("Error: %s\n", strErrorMsg);
		Exit();
	}
	void ExitOnCodeError(char* strErrorMsg)
	{
		printf("Error: %s.\n\n", strErrorMsg);
		printf("Line %d\n", GetCurrentSourceLineIndex());

		char strCurrentSourceLine[MAX_SOURCE_LINE_SIZE];
		char* strTempSourceLine = GetCurrentSourceLine();
		if (strTempSourceLine)
			strcpy(strCurrentSourceLine, strTempSourceLine);
		else
			strCurrentSourceLine[0] = '\0';

		int lastIndex = strlen(strCurrentSourceLine) - 1;
		if (strCurrentSourceLine[lastIndex] == '\n')
			strCurrentSourceLine[lastIndex] = '\0';

		for (uint i = 0; i < lastIndex; i++)
			if (strCurrentSourceLine[i] == '\t')
				strCurrentSourceLine[i] = ' ';

		printf("%s\n", strCurrentSourceLine);

		for (uint i = 0; i < GetCurrentLexemeStartIndex(); i++)
			printf(" ");
		printf("^\n");

		printf("Could not compile %s\n", compiler.lexer.scriptSourceFile);

		Exit();
	}
	void ExitOnInvalidInputError(char c)
    {
    	printf("Error: expected %c\n", c);
    	exit(0);
    }
 
	void Init()
	{
		compiler.lexer.currLexerState.currentLexemeStart = 0;
		compiler.lexer.currLexerState.currentLexemeEnd   = 0;

		compiler.scriptHeader.iIsMainFuncExist = FALSE;
		compiler.scriptHeader.iStackSize       = 0;
		
		InitLinkList(&compiler.sourceCode);
		InitLinkList(&compiler.functionTable);
		InitLinkList(&compiler.symbolTable);
		InitLinkList(&compiler.stringTable);
	}

	void ShutDown()
	{
		FreeLinkList(&compiler.sourceCode);
		FreeLinkList(&compiler.functionTable);
		FreeLinkList(&compiler.symbolTable);
		FreeLinkList(&compiler.stringTable);
	}

	void Exit()
	{
		ShutDown();
		exit(0);
	}
}


