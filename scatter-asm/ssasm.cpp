#include <stdio.h>
#include <string.h>

#include "sspre.h"
#include "ssasm.h"




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

int main()
{
	printf("%d\n", IsCharNumeric('8'));
}













