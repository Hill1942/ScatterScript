#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#endif // _WIN32


#include "ssutil.h"
#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sslexeme.h"
#include "ssasm.h"
#include "ssvm.h"
#include "ssil.h"
#include "sscl.h"
#include "sssystem.h"


_asm_::ASM sasm;
_vm::Script vm_script;
_vm::Script vm_scripts[MAX_THREAD_NUMBER];

_cl::Compiler compiler;


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
		case 's':
			{
				strcpy(compiler.scriptSourceFile, argv[2]);
				int ExtOffset = strrchr(compiler.scriptSourceFile, '.') - compiler.scriptSourceFile;
				if (compiler.scriptSourceFile[ExtOffset + 1] != 's' || 
					compiler.scriptSourceFile[ExtOffset + 2] != 'c' ||
					compiler.scriptSourceFile[ExtOffset + 3] != 's')
				{
					printf("Not valid .scs file!\n");
					exit(0);
				}

				if (argv[3])
				{
					strcpy(compiler.outAssembleFilename, argv[3]);
					if (! strstr(compiler.outAssembleFilename, ASM_SOURCE_EXTENSION))
						strcat(compiler.outAssembleFilename, ASM_SOURCE_EXTENSION);
				}
				else
				{
					strncpy(compiler.outAssembleFilename, compiler.scriptSourceFile, ExtOffset);
					compiler.outAssembleFilename[ExtOffset] = '\0';
					strcat(compiler.outAssembleFilename, ASM_SOURCE_EXTENSION);
				}

				compile();
				break;
			}
			
		case 'a':
			{
				strcpy(sasm.sourceFileName, argv[2]);

				if (argv[3])
				{
					strcpy(sasm.exeFileName, argv[3]);
					if (! strstr(sasm.exeFileName, EXE_EXTENSION))
						strcat(sasm.exeFileName, EXE_EXTENSION);
				}
				else
				{
					int ExtOffset = strrchr(sasm.sourceFileName, '.') - sasm.sourceFileName;
					strncpy(sasm.exeFileName, sasm.sourceFileName, ExtOffset);
					sasm.exeFileName[ExtOffset] = '\0';
					strcat(sasm.exeFileName, EXE_EXTENSION);
				}

				asm_run();
			}
			break;
		case 'c':
			{
				strcpy(compiler.scriptSourceFile, argv[2]);
				int ExtOffset = strrchr(compiler.scriptSourceFile, '.') - compiler.scriptSourceFile;
				if (compiler.scriptSourceFile[ExtOffset + 1] != 's' || 
					compiler.scriptSourceFile[ExtOffset + 2] != 'c' ||
					compiler.scriptSourceFile[ExtOffset + 3] != 's')
				{
					printf("Not valid .ss file!\n");
					exit(0);
				}

				if (argv[3])
				{
					strcpy(compiler.outAssembleFilename, argv[3]);
					if (! strstr(compiler.outAssembleFilename, ASM_SOURCE_EXTENSION))
						strcat(compiler.outAssembleFilename, ASM_SOURCE_EXTENSION);

					strcpy(sasm.exeFileName, argv[3]);
					if (! strstr(sasm.exeFileName, EXE_EXTENSION))
						strcat(sasm.exeFileName, EXE_EXTENSION);
				}
				else
				{
					strncpy(compiler.outAssembleFilename, compiler.scriptSourceFile, ExtOffset);
					compiler.outAssembleFilename[ExtOffset] = '\0';
					strcat(compiler.outAssembleFilename, ASM_SOURCE_EXTENSION);

					strncpy(sasm.exeFileName, compiler.outAssembleFilename, ExtOffset);
					sasm.exeFileName[ExtOffset] = '\0';
					strcat(sasm.exeFileName, EXE_EXTENSION);
				}

				compile();

				if (compiler.isOutCode)
				{
					strcpy(sasm.sourceFileName, compiler.outAssembleFilename);
					asm_run();
				}
				else
				{
					printf("can not compile, error!");
				}

#ifdef _WIN32
				char* argv[4];

				argv[0] = (char*) malloc(strlen("ss") + 1);
				strcpy(argv[0], "ss");

				argv[1] = (char*) malloc(strlen("-d") + 1);
				strcpy(argv[1], "-d");

				argv[2] = (char*) malloc(strlen(compiler.outAssembleFilename) + 1);
				strcpy(argv[2], compiler.outAssembleFilename);

				argv[3] = NULL;

				_spawnv(_P_OVERLAY, "ss.exe", argv);

				//the following code will not run, so there may be memory leak 
				free ( argv[0] );
				free ( argv[1] );
				free ( argv[2] );
#endif

				break;
			}
			break;

		case 'd':
			{
				remove(argv[2]);
				break;
			}
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
	strcpy(sasm.exeInfoFilename, sasm.exeFileName);
	strcat(sasm.exeInfoFilename, ".info");

	_asm_::Init();

	_asm_::LoadSourceFile();

	_asm_::AssembleSourceFile();

	_asm_::BuildSSE();

#ifdef DEBUG_ON
	_asm_::BuildSSE_Info();
#endif

	_asm_::ShutDown();
}

void vm_run(char* exeFilename)
{
	_vm::Init();

	int errorCode = _vm::LoadScript(exeFilename);

	if (errorCode != VM_LOAD_OK)
	{
		printf("Error: ");
		switch (errorCode)
		{
		case VM_LOAD_ERROR_FAIL_FILE_OPEN:
			printf("Can not open sse file");
			break;

		case VM_LOAD_ERROR_INVALID_SSE:
			printf("Invalid sse file");
			break;

		case VM_LOAD_ERROR_UNSOPPORTED_VERSION:
			printf("Unsupported sse version");
			break;

		default:
			break;
		}
		printf(".\n");
		exit(0);
	}

	_vm::ResetScript();

	_vm::RunScript();

	_vm::ShutDown();
}
void compile()
{
	_cl::Init();

	_cl::LoadScriptSource();
	_cl::PreProcessSourceCode();

	compiler.tempVar0SymbolIndex = _cl::AddSymbol(&compiler.symbolTable, 1,
		SCOPE_GLOBAL, SYMBOL_TYPE_VAR, TEMP_VAR_0);
	compiler.tempVar1SymbolIndex = _cl::AddSymbol(&compiler.symbolTable, 1,
		SCOPE_GLOBAL, SYMBOL_TYPE_VAR, TEMP_VAR_1);

	_cl::ParseSourceCode();

	_IL::OutCode();
}

