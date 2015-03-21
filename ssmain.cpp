#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssutil.h"
#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sssystem.h"
#include "sslexeme.h"
#include "ssasm.h"
#include "ssvm.h"

_asm_::ASM sasm;
_vm::Script vm_script;
_vm::Script vm_scripts[MAX_THREAD_NUMBER];

_cl::Lexer cl_lexer;


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
			{
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
	strcpy(sasm.exeInfoFilename, sasm.exeFileName);
	strcat(sasm.exeInfoFilename, ".info");

	_asm_::Init();

	_asm_::LoadSourceFile();

	_asm_::AssembleSourceFile();

	_asm_::BuildSSE();

	_asm_::BuildSSE_Info();

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
	}

	_vm::ResetScript();

	_vm::RunScript();

	_vm::ShutDown();
}
void compile()
{

}
