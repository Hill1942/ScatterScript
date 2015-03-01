#include <stdio.h>
#include <string.h>

#include "scatter-asm/ssasm_pre.h"
#include "scatter-asm/ssutil.h"
#include "scatter-asm/ssbase_type.h"
#include "scatter-asm/sslang.h"
#include "scatter-asm/sssystem.h"
#include "scatter-asm/sslexeme.h"
#include "scatter-asm/ssasm.h"

FILE*  g_SourceFile;
char   g_SourceFileName[MAX_FILENAME_SIZE];
int    g_SourceCodeLines;
char** g_SourceCode;
char   g_ExeFileName[MAX_FILENAME_SIZE];

LinkList g_FunctionTable;
LinkList g_StringTable;
LinkList g_LabelTable;
LinkList g_SymbolTable;
LinkList g_HostAPICallTable;

Lexer  g_Lexer;
ScriptHeader g_ScriptHeader;
Instr* g_InstrStream;

int g_InstrStreamSize;
int g_IsSetStackSizeFound;
int g_currentInstrIndex;

InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Wrong usage!\n");
		return 0;
	}

	strcpy(g_SourceFileName, argv[1]);

	if (argv[2])
	{
		strcpy(g_ExeFileName, argv[2]);
		if (! strstr(g_ExeFileName, EXE_EXTENSION))
			strcat(g_ExeFileName, EXE_EXTENSION);
	}
	else
	{
		int ExtOffset = strrchr(g_SourceFileName, '.') - g_SourceFileName;
		strncpy(g_ExeFileName, g_SourceFileName, ExtOffset);
		g_ExeFileName[ExtOffset] = '\0';
		strcat(g_ExeFileName, EXE_EXTENSION);
	}

	Init();

	LoadSourceFile();

	AssembleSourceFile();

	BuildSSE();

	ShutDown();

	return 0;
}