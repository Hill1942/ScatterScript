#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scatter-asm/ssasm_pre.h"
#include "scatter-vm/ssvm_pre.h"
#include "scatter-vm/sslang_vm.h"
#include "scatter-vm/ssvm.h"

Script g_Script;

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		printf("Wrong usage !\n");
		return 0;
	}

	char* exeFilename = (char*) malloc(strlen(argv[1]) + 1);
	strcpy(exeFilename, argv[1]);

	Init();

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

		return 0;
	}
}