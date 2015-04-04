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
			printf("%d\n", param.iIntLiteral);
			break;
		case ASM_OPRAND_TYPE_FLOAT:
			printf("%f\n", param.fFloatLiteral);
			break;
		case ASM_OPRAND_TYPE_STRING_INDEX:
			printf("%s\n", param.strStringLiteral);
			break;
		default:
			break;
		}
	}

	void sstest(Value* params, int paramCount)
	{
		char* chr = "hello";

		ReturnValue_String(chr);
		
	}
}
