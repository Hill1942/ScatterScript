#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scatter-asm/ssasm_pre.h"d
#include "scatter-asm/ssutil.h"
#include "scatter-asm/ssbase_type.h"
#include "scatter-asm/sslang.h"
#include "scatter-asm/sssystem.h"
#include "scatter-asm/sslexeme.h"
#include "scatter-asm/ssasm.h"

#include "scatter-vm/ssvm_pre.h"
#include "scatter-vm/sslang_vm.h"
#include "scatter-vm/ssvm.h"

FILE*  g_SourceFile;
char   g_SourceFileName[MAX_FILENAME_SIZE];
int    g_SourceCodeLines;
char** g_SourceCode;
char   g_ExeFileName[MAX_FILENAME_SIZE];
char   g_ExeInfoFilename[MAX_FILENAME_SIZE];

LinkList g_FunctionTable;
LinkList g_StringTable;
LinkList g_LabelTable;
LinkList g_SymbolTable;
LinkList g_HostAPICallTable;

Lexer  g_Lexer;
ScriptHeader g_ScriptHeader;
ASM_Instr* g_InstrStream;

int g_InstrStreamSize;
int g_IsSetStackSizeFound;
int g_currentInstrIndex;

InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

Script g_Script;


void asm_run();
void vm_run(char* exeFilename);
void compile();

int main(int argc, char* argv[])
{

	char option[3];
	if (argv[1][0] == '-')
	{
		strcpy(option, argv[1]);
		option[2] = '\0';
		switch (option[1])
		{
		case 'c':
			break;
		case 'a':
			{
				strcpy(g_SourceFileName, argv[2]);

				if (argv[3])
				{
					strcpy(g_ExeFileName, argv[3]);
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

				asm_run();
			}
			break;
		case 'o':
			break;
		default:
			break;
		}
	}
	else
	{
		if (argc < 2)
		{
			printf("Wrong usage !\n");
			return 0;
		}

		char* exeFilename = (char*) malloc(strlen(argv[1]) + 1);
		strcpy(exeFilename, argv[1]);

		vm_run(exeFilename);
	}

	
	
}

void asm_run()
{
	strcpy(g_ExeInfoFilename, g_ExeFileName);
	strcat(g_ExeInfoFilename, ".info");

	ASM_Init();

	LoadSourceFile();

	AssembleSourceFile();

	BuildSSE();

	BuildSSE_Info();

	ASM_ShutDown();
}

void vm_run(char* exeFilename)
{
	VM_Init();

	int errorCode = LoadScript(exeFilename);

	if (errorCode != LOAD_OK)
	{
		printf("Error: ");
		switch (errorCode)
		{
		case LOAD_ERROR_FAIL_FILE_OPEN:
			printf("Can not open sse file");
			break;

		case LOAD_ERROR_INVALID_SSE:
			printf("Invalid sse file");
			break;

		case LOAD_ERROR_UNSOPPORTED_VERSION:
			printf("Unsupported sse version");
			break;

		default:
			break;
		}
		printf(".\n");
	}

	ResetScript();

	RunScript();

	VM_ShutDown();
}
void compile()
{

}