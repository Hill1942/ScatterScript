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

	namespace io
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
			case ASM_OPRAND_TYPE_NAN:
				printf("NaN");
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

		void ssfclose(Value* params, int paramCount)
		{
			if (paramCount != 1)
			{
				printf("vm error, ssfclose must take 1 parameter!\n");
				return;
			}

			Value fileHandler = params[0];

			FILE* pFile = (FILE*)fileHandler.iIntLiteral;
			fclose(pFile);
		}

		void ssfgetc(Value* params, int paramCount)
		{
			if (paramCount != 1)
			{
				printf("vm error, ssfgetc must take 1 parameter!\n");
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

		void ssfgets(Value* params, int paramCount)
		{
			if (paramCount != 2)
			{
				printf("vm error, ssfgets must take 2 parameter!\n");
				exit(0);
			}

			Value fileHandler = params[0];
			Value charNum     = params[1];
			FILE* pFile = (FILE*)fileHandler.iIntLiteral;

			char* str = (char*) malloc(charNum.iIntLiteral + 1);

			if (!feof(pFile))
			{
				fgets(str, charNum.iIntLiteral, pFile);
				ReturnValue_String(str);
			}
			else
			{
				ReturnValue_Int(0);
			}
		}

		void ssfwrite(Value* params, int paramCount)
		{
			if (paramCount != 2)
			{
				printf("vm error, ssfgets must take 2 parameter!\n");
				exit(0);
			}

			Value fileHandler = params[0];
			Value out         = params[1];
			FILE* pFile = (FILE*)fileHandler.iIntLiteral;

			if (pFile == NULL)
			{
				printf("ssfwrite can not open file\n");
				exit(0);
			}

			switch(out.iType)
			{
			case ASM_OPRAND_TYPE_INT:
				fprintf(pFile, "%d", out.iIntLiteral);
				break;
			case ASM_OPRAND_TYPE_FLOAT:
				fprintf(pFile, "%f", out.fFloatLiteral);
				break;
			case ASM_OPRAND_TYPE_STRING_INDEX:
				fprintf(pFile, "%s", out.strStringLiteral);
				break;
			case ASM_OPRAND_TYPE_NAN:
				fprintf(pFile, "NaN");
			default:
				break;
			}
		}
	}
	

	
}
