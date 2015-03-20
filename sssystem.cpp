#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sssystem.h"

extern _asm_::ASM ssam;
extern _vm::Script vm_script;
extern _cl::Lexer cl_lexer;

namespace _asm_
{
	void Init()
    {
    	InitInstrTable();
    
    	InitLinkList(&ssam.symbolTable);
    	InitLinkList(&ssam.labelTable);
    	InitLinkList(&ssam.functionTable);
    	InitLinkList(&ssam.stringTable);
    	InitLinkList(&ssam.hostAPICallTable);
    }
    void InitInstrTable()
    {
    	int instrIndex;
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "mov", ASM_INSTR_MOV, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    							 ASM_OPRAND_FLAG_TYPE_STRING |
    							 ASM_OPRAND_FLAG_TYPE_MEM_REF |
    							 ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "add", ASM_INSTR_AND, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "sub", ASM_INSTR_SUB, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "mul", ASM_INSTR_MUL, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "div", ASM_INSTR_DIV, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "mod", ASM_INSTR_MOD, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "exp", ASM_INSTR_EXP, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "neg", ASM_INSTR_NEG, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "inc", ASM_INSTR_INC, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "dec", ASM_INSTR_DEC, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "and", ASM_INSTR_AND, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "or", ASM_INSTR_OR, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "xor", ASM_INSTR_XOR, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "not", ASM_INSTR_NOT, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "shl", ASM_INSTR_SHL, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "shr", ASM_INSTR_SHR, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "concat", ASM_INSTR_CONCAT, 2);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    							 ASM_OPRAND_FLAG_TYPE_STRING);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "getchar", ASM_INSTR_GETCHAR, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    		                     ASM_OPRAND_FLAG_TYPE_STRING);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    		                     ASM_OPRAND_FLAG_TYPE_INT);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "setchar", ASM_INSTR_SETCHAR, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    							 ASM_OPRAND_FLAG_TYPE_INT);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG |
    		                     ASM_OPRAND_FLAG_TYPE_STRING);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "jmp", ASM_INSTR_JMP, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "je", ASM_INSTR_JE, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "jne", ASM_INSTR_JNE, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "jg", ASM_INSTR_JG, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "jl", ASM_INSTR_JL, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "jge", ASM_INSTR_JGE, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "jle", ASM_INSTR_JLE, 3);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 1, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    	setOpType(ssam.instrLookup, instrIndex, 2, ASM_OPRAND_FLAG_TYPE_LINE_LABEL);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "push", ASM_INSTR_PUSH, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "pop", ASM_INSTR_POP, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "call", ASM_INSTR_CALL, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_FUNC_NAME);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "ret", ASM_INSTR_RET, 0);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "callhost", ASM_INSTR_CALLHOST, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_HOST_API_CALL);
    
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "pause", ASM_INSTR_PAUSE, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "exit", ASM_INSTR_EXIT, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
    		                     ASM_OPRAND_FLAG_TYPE_FLOAT |
    		                     ASM_OPRAND_FLAG_TYPE_STRING |
    		                     ASM_OPRAND_FLAG_TYPE_MEM_REF |
    		                     ASM_OPRAND_FLAG_TYPE_REG);
    
    	instrIndex = AddInstrLookup(ssam.instrLookup, "_print", ASM_INSTR_PRINT, 1);
    	setOpType(ssam.instrLookup, instrIndex, 0, ASM_OPRAND_FLAG_TYPE_INT |
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
    	for (int i = 0; i < ssam.sourceCodeLines; i++)
    	{
    		free(ssam.sourceCode[i]);
    	}
    	free(ssam.sourceCode);
    }
    
    void ExitOnError(const char* errorMessage)
    {
        printf("Fatal Error: %s.\n", errorMessage);
        Exit();
    }
    
    void ExitOnCodeError(const char* errorMessage)
    {
    	printf("Error: %s.\n\n", errorMessage);
    	printf("Line %d\n", ssam.lexer.iCurrentSourceLine);
    
    	char sourceLine[MAX_SOURCE_LINE_SIZE];
    	strcpy(sourceLine, ssam.sourceCode[ssam.lexer.iCurrentSourceLine]);
    
    	for (unsigned int i = 0; i < strlen(sourceLine); i++)
    		if (sourceLine[i] == '\t')
    			sourceLine[i] = ' ';
    
    	printf("%s", sourceLine);
    
    	for (unsigned int i = 0; i < ssam.lexer.iIndex0; i++)
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
	void ExitOnInvalidInputError(char c)
    {
    	printf("Error: expected %c\n", c);
    	exit(0);
    }
    
    void CL_InitLexer()
    {
		cl_lexer.currentLexemeStart = 0;
		cl_lexer.currentLexemeEnd   = 0;
    }
}


