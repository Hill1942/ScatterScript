#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssvm_pre.h"
#include "sslang_vm.h"
#include "ssvm.h"

extern Script g_Script;

int LoadScript(char* filename)
{
	FILE* pScriptFile = fopen(filename, "rb");
	if (pScriptFile == NULL)
		return LOAD_ERROR_FAIL_FILE_OPEN;

	char* versionStr = (char*) malloc(5);
	fread(versionStr, 4, 1, pScriptFile);
	versionStr[strlen(SSE_ID_STRING)] = '\0';

	if (strcmp(versionStr, SSE_ID_STRING) != 0)
		return LOAD_ERROR_INVALID_SSE;

	free(versionStr);

	int majorVersion = 0;
	int minorVersion = 0;

	fread(&majorVersion, 1, 1, pScriptFile);
	fread(&minorVersion, 1, 1, pScriptFile);

	if (majorVersion != VERSION_MAJOR || minorVersion != VERSION_MINOR)
		return LOAD_ERROR_UNSOPPORTED_VERSION;

	if (g_Script.stack.iSize == 0)
		g_Script.stack.iSize = DEFAULT_STACK_SIZE;

	g_Script.stack.pElement = (Value*) malloc(g_Script.stack.iSize * sizeof(Value));

	fread(&g_Script.iGlobalDataSize, 4, 1, pScriptFile);
	fread(&g_Script.iIsMainFuncExist, 1, 1, pScriptFile);

}