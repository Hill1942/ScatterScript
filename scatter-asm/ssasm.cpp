#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sstype.h"
#include "ssasm.h"

#define MAX_SOURCE_LINE_SIZE 4096

FILE*  g_SourceFile;
char*  g_SourceFileName;
int    g_SourceCodeLines;
char** g_SourceCode;

int IsCharNumeric(char cChar)
{
	if (cChar >= '0' && cChar <= '9') 
		return TRUE;
	else
		return FALSE;
}
int IsCharWhiteSpace(char cChar)
{
	if (cChar == ' ' || cChar == '\t')
		return TRUE;
	else 
		return FALSE;
}
int IsCharIdent(char cChar)
{
	if ((cChar >= '0' && cChar <= '9') ||
		(cChar >= 'A' && cChar <= 'Z') ||
		(cChar >= 'a' && cChar <= 'z') ||
		cChar >= '_')
		return TRUE;
	else 
		return FALSE;
}
int IsCharDelimiter(char cChar)
{
	if (cChar == ':' || cChar == ',' || cChar == '"' ||
		cChar == '[' || cChar == ']' || 
		cChar == '{' || cChar == '}' ||
		IsCharWhiteSpace(cChar))
		return TRUE;
	else
		return FALSE;
}

int IsStringInt(char* str)
{
	if (!str)
		return FALSE;

	if (strlen(str) == 0)
		return TRUE;

	for (int i = 0; i < strlen(str); i++)
	{
		if (!IsCharNumeric(str[i]) && str[i] != '-')
			return FALSE;
	}

	for (int i = 1; i < strlen(str); i++) 
	{
		if (str[i] == '-')
			return FALSE;
	}
}
int IsStringFloat(char* str)
{
	if (!str) 
		return FALSE;

	if (strlen(str) == 0) 
		return FALSE;

	for (int i = 0; i < strlen(str); i++) 
	{
		if (!IsCharNumeric(str[i]) && str[i] != '-' && str[i] != '.')
			return FALSE;
	}

	int isRadixPointFound = 0;
	for (int i = 0; i < strlen(str); i++) 
	{
		if (str[i] == '.')
		{
			if (isRadixPointFound) 
				return FALSE;
			else
				isRadixPointFound = 1;
		}	
	}

	for (int i = 1; i < strlen(str); i++) 
	{
		if (str[i] == '-')
			return FALSE;
	}

	if (isRadixPointFound) 
		return TRUE;
	else 
		return FALSE;
}
int IsStringWhitespace(char* str)
{
	if (!str)
		return FALSE;

	if (strlen(str) == 0)
		return TRUE;

	for (int i = 0; i < strlen(str); i++) 
	{
		if (!IsCharWhiteSpace(str[i]) && str[i] != '\n')
			return FALSE;
	}
	return TRUE;
}
int IsStringIdent(char* str)
{
	if (!str) 
		return FALSE;

	if (strlen(str) == 0) 
		return FALSE;

	if (str[0] >= '0' && str[9] <= '9')
		return FALSE;

	for (int i = 0; i < strlen(str); i++)
	{
		if (!IsCharIdent(str[i]))
			return FALSE;
	}

	return TRUE;
}

int StripComments(char* str)
{
	int isInString = 0;
	for (int i = 0; i < strlen(str); i++) 
	{
		if (str[i] == '"') 
		{
			if (isInString)
				isInString = 0;
			else
				isInString = 1;
		}
		if (str[i] == ';') 
		{
			if (!isInString)
			{
				str[i]     = '\n';
				str[i + 1] = '\0';
				break;
			}
		}
	}
}
int TrimWhiteSpace(char* str)
{
	int strLength    = strlen(str);
	int currentIndex = 0;
	int padIndex     = 0;

	if (strLength > 1) 
	{
		for (int currentIndex = 0; currentIndex < strLength; currentIndex++) 
		{
			if (!IsCharWhiteSpace(str[currentIndex]))
				break;
		}

		padIndex = currentIndex;
		if (padIndex)
		{
			for (currentIndex = padIndex; currentIndex < strLength; currentIndex++) 
			{
				str[currentIndex - padIndex] = str[currentIndex];
			}
			for (currentIndex = strLength - padIndex; currentIndex < strLength; currentIndex++)
			{
				str[currentIndex] = ' ';
			}
		}
		
		for (currentIndex = strLength - 1; currentIndex > 0; currentIndex--) 
		{
			if (!IsCharWhiteSpace(str[currentIndex]))
			{
				str[currentIndex + 1] = '\n';
				break;
			}
		}
	}
}


void LoadSourceFile()
{
	if (! (g_SourceFile = fopen(g_SourceFileName, "rb")))
		ExitOnError("Could not open source file");

	while (!feof(g_SourceFile))
	{
		if (fgetc(g_SourceFile) == '\n')
			g_SourceCodeLines++;
	}
	g_SourceCodeLines++;

	rewind(g_SourceFile);

	if (! (g_SourceCode = (char**)malloc(g_SourceCodeLines * sizeof(char*))))
		ExitOnError("Could not allocate space for source code");

	for (int i = 0; i < g_SourceCodeLines; i++) 
	{
		if (! (g_SourceCode[i] = (char*)malloc(MAX_SOURCE_LINE_SIZE + 1)))
			ExitOnError("Could not allocate space for source code");

		fgets(g_SourceCode[i], MAX_SOURCE_LINE_SIZE, g_SourceFile);

		StripComments(g_SourceCode[i]);
		TrimWhiteSpace(g_SourceCode[i]);

		// Make sure to add a new newline if it was removed by the stripping of the
		// comments and whitespace. We do this by checking the character right before
		// the null terminator to see if it's \n. If not, we move the terminator over
		// by one and add it. We use strlen () to find the position of the newline
		// easily.

		int newLineIdex = strlen(g_SourceCode[i]) - 1;
		if (g_SourceCode[i][newLineIdex] != '\n' )
		{
			g_SourceCode[i][newLineIdex + 1] = '\n';
			g_SourceCode[i][newLineIdex + 2] = '\0';
		}
	}
}

void ExitOnError(char* errorMessage)
{
	printf("Fatal Error: %s.\n", errorMessage);
	Exit();
}

void Exit()
{
	ShutDown();
	exit(0);
}

void ShutDown()
{
	for (int i = 0; i < g_SourceCodeLines; i++)
	{
		free(g_SourceCode[i]);
	}
	free(g_SourceCode);
}

int main()
{
	printf("%d\n", IsCharNumeric('8'));
}













