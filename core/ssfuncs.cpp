#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssutil.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "ssvm.h"
#include "ssfuncs.h"

namespace _vm
{
	void ssprint(Value* params, int paramCount)
	{
		if (paramCount != 1)
		{
			printf("vm error, ssprint must take 1 parameter!\n");
			return;
		}

		Value param = params[0];
		switch(param.iType)
		{
		case ASM_OPRAND_TYPE_INT:
			printf("%d", param.iIntLiteral);
			break;
		case ASM_OPRAND_TYPE_FLOAT:
			printf("%f", param.fFloatLiteral);
			break;
		case ASM_OPRAND_TYPE_STRING_INDEX:
			printf("%s", param.strStringLiteral);
			break;
		default:
			break;
		}
	}

	void ssfopen(Value* params, int paramCount)
	{
		if (paramCount != 2)
		{
			printf("vm error, ssfopen must take 2 parameter!\n");
			return;
		}

		Value filename = params[0];
		Value mode     = params[1];

		FILE* pFile = fopen(filename.strStringLiteral, mode.strStringLiteral);

		ReturnValue_Pointer(pFile);
	}

	void ssgetc(Value* params, int paramCount)
	{
		if (paramCount != 1)
		{
			printf("vm error, ssgetc must take 1 parameter!\n");
			return;
		}

		Value fileHandler = params[0];
		FILE* pFile = (FILE*)fileHandler.iIntLiteral;

		char str[2];
		str[0] = getc(pFile);
		if (str[0] != -1) 
		{
			str[1] = '\0';
			ReturnValue_String(str);
		}
		else
		{
		   ReturnValue_Int(0);
		}
		
	}

	
}
