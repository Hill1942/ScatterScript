#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "ssasm_pre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sssystem.h"

extern FILE*  g_SourceFile;
extern char*  g_SourceFileName;
extern int    g_SourceCodeLines;
extern char** g_SourceCode;

extern LinkList g_FunctionTable;
extern LinkList g_StringTable;
extern LinkList g_LabelTable;
extern LinkList g_SymbolTable;
extern LinkList g_HostAPICallTable;

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

}

void ExitOnCharExpectedError(char c)
{
}

