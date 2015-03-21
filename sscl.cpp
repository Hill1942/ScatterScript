#include <stdio.h>
#include <stdlib.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sscl.h"


namespace _cl
{
	void LoadScriptSource(char* filename, Lexer lexer)
	{
		FILE* scriptFile = fopen(filename, "rb");

		if (scriptFile == NULL)
		{
			printf("Can not load script file!\n");
			exit(0);
		}

		fseek(scriptFile, 0, SEEK_END);
		int scriptSize = ftell(scriptFile);
		fseek(scriptFile, 0, SEEK_SET);

		lexer.stringSource = (char*) malloc(scriptSize + 1);

		char currentChar;
		for (int i = 0; i < scriptSize; i++)
		{
			currentChar = fgetc(scriptFile);
			if (currentChar == 13)	// '13' means return
			{
				fgetc(scriptFile);	// jump the next char
				scriptSize--;
				lexer.stringSource[i] = '\n';
			}
			else
			{
				lexer.stringSource[i] = currentChar;
			}
		}

		lexer.stringSource[scriptSize] = '\0';

		fclose(scriptFile);
	}

}