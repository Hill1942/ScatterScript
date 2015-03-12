#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssasm_pre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sssystem.h"

extern FILE*  g_SourceFile;
extern char   g_SourceFileName[MAX_FILENAME_SIZE];
extern int    g_SourceCodeLines;
extern char** g_SourceCode;

extern LinkList g_FunctionTable;
extern LinkList g_StringTable;
extern LinkList g_LabelTable;
extern LinkList g_SymbolTable;
extern LinkList g_HostAPICallTable;

extern Lexer g_Lexer;

void Init()
{
	InitInstrTable();

	InitLinkList(&g_SymbolTable);
	InitLinkList(&g_LabelTable);
	InitLinkList(&g_FunctionTable);
	InitLinkList(&g_StringTable);
	InitLinkList(&g_HostAPICallTable);
}
void InitInstrTable()
{
	int instrIndex;

	instrIndex = AddInstrLookup("mov", INSTR_MOV, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
							 OP_FLAG_TYPE_STRING |
							 OP_FLAG_TYPE_MEM_REF |
							 OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("add", INSTR_AND, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("sub", INSTR_SUB, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("mul", INSTR_MUL, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("div", INSTR_DIV, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("mod", INSTR_MOD, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("exp", INSTR_EXP, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("neg", INSTR_NEG, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("inc", INSTR_INC, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("dec", INSTR_DEC, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("and", INSTR_AND, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("or", INSTR_OR, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("xor", INSTR_XOR, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("not", INSTR_NOT, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);


	instrIndex = AddInstrLookup("shl", INSTR_SHL, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("shr", INSTR_SHR, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("concat", INSTR_CONCAT, 2);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG |
							 OP_FLAG_TYPE_STRING);

	instrIndex = AddInstrLookup("getchar", INSTR_GETCHAR, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG |
		                     OP_FLAG_TYPE_STRING);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG |
		                     OP_FLAG_TYPE_INT);

	instrIndex = AddInstrLookup("setchar", INSTR_SETCHAR, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG |
							 OP_FLAG_TYPE_INT);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG |
		                     OP_FLAG_TYPE_STRING);

	instrIndex = AddInstrLookup("jmp", INSTR_JMP, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("je", INSTR_JE, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("jne", INSTR_JNE, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("jg", INSTR_JG, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("jl", INSTR_JL, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("jge", INSTR_JGE, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("jle", INSTR_JLE, 3);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 1, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
	setOpType(instrIndex, 2, OP_FLAG_TYPE_LINE_LABEL);

	instrIndex = AddInstrLookup("push", INSTR_PUSH, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("pop", INSTR_POP, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("call", INSTR_CALL, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_FUNC_NAME);

	instrIndex = AddInstrLookup("ret", INSTR_RET, 0);

	instrIndex = AddInstrLookup("callhost", INSTR_CALLHOST, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_HOST_API_CALL);


	instrIndex = AddInstrLookup("pause", INSTR_PAUSE, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("exit", INSTR_EXIT, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);

	instrIndex = AddInstrLookup("_print", INSTR_PRINT, 1);
	setOpType(instrIndex, 0, OP_FLAG_TYPE_INT |
		                     OP_FLAG_TYPE_FLOAT |
		                     OP_FLAG_TYPE_STRING |
		                     OP_FLAG_TYPE_MEM_REF |
		                     OP_FLAG_TYPE_REG);
}

void Exit()
{
	ShutDown();
	exit(0);
}

void ShutDown()
{
	for (int i = 0; i < g_SourceCodeLines; i++)
	{
		free(g_SourceCode[i]);
	}
	free(g_SourceCode);
}

void ExitOnError(const char* errorMessage)
{
    printf("Fatal Error: %s.\n", errorMessage);
    Exit();
}

void ExitOnCodeError(const char* errorMessage)
{
	printf("Error: %s.\n\n", errorMessage);
	printf("Line %d\n", g_Lexer.iCurrentSourceLine);

	char sourceLine[MAX_SOURCE_LINE_SIZE];
	strcpy(sourceLine, g_SourceCode[g_Lexer.iCurrentSourceLine]);

	for (unsigned int i = 0; i < strlen(sourceLine); i++)
		if (sourceLine[i] == '\t')
			sourceLine[i] = ' ';

	printf("%s", sourceLine);

	for (unsigned int i = 0; i < g_Lexer.iIndex0; i++)
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

