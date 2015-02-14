#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <termio.h>

#define MAX_SOURCE_LINE_SIZE 4096
#define MAX_COMMAND_SIZE     64
#define MAX_PARAM_SIZE       1024

#define COMMAND_PRINTSTRING     "PrintString"
#define COMMAND_PRINTSTRINGLOOP "PrintStringLoop"
#define COMMAND_NEWLINE         "NewLine"
#define COMMAND_WAITFORKEYPRESS "WaitForKeyPress"

int    g_ScriptLines = 0;
int    g_CurrentLine = 0;
int    g_CurrentLineChar = 0;
char** g_ppScripts;

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}


char* strtoupper(char* str)
{
    char* p = str;
    
    *p = std::toupper(*p);
    while (*p) 
        p++;
    
    return str;
}

void GetCommand(char* pstrDestString) 
{
	int  commandSize = 0;
	char currentChar;

	while (g_CurrentLineChar < (int)strlen(g_ppScripts[g_CurrentLine])) 
	{
		currentChar = g_ppScripts[g_CurrentLine][g_CurrentLineChar];
		if (currentChar == ' ' || currentChar == '\n') 
		{
			break;
		}
		pstrDestString[commandSize] = currentChar;
		commandSize++;
		g_CurrentLineChar++;
	}
	g_CurrentLineChar++;                  //Jump the space at the end of command
	pstrDestString[commandSize] = '\0';
	strtoupper(pstrDestString);
}

int  GetInitParam()
{
	char pstrString[MAX_PARAM_SIZE];
	int  paramSize = 0;
	char currentChar;
	
	while (g_CurrentLineChar < (int)strlen(g_ppScripts[g_CurrentLine]))
	{
		currentChar = g_ppScripts[g_CurrentLine][g_CurrentLineChar];
		if (currentChar == ' ' || currentChar == '\n') 
		{
			break;
		}
		pstrString[paramSize] = currentChar;
		paramSize++;
		g_CurrentLineChar++;
	}
	g_CurrentLineChar++;
	pstrString[paramSize] = '\0';
	int intValue = atoi(pstrString);

	return intValue;
}

void GetStringParam(char* pstrDestString)
{
	int  paramSize = 0;
	char currentChar;
	g_CurrentLineChar++;
	while (g_CurrentLineChar < (int)strlen(g_ppScripts[g_CurrentLine])) 
	{
		currentChar = g_ppScripts[g_CurrentLine][g_CurrentLineChar];
		if (currentChar == '"' || currentChar == '\n') 
		{
			break;
		}
		pstrDestString[paramSize] = currentChar;
		paramSize++;
		g_CurrentLineChar++;
	}
	g_CurrentLineChar += 2;
	pstrDestString[paramSize] = '\0';
}

void LoadScript(char* pStrFileName)
{
	FILE* pScriptFile;
	if (!(pScriptFile = fopen(pStrFileName, "r")))
	{
		printf("Cannot load file %s\n", pStrFileName);
		exit(0);
	}

	while (!feof(pScriptFile))
	{
		if (fgetc(pScriptFile) == '\n') 
		{
			g_ScriptLines++;
		}
	}
	g_ScriptLines++;

	rewind(pScriptFile);
	g_ppScripts = (char**)malloc(g_ScriptLines * sizeof(char*));
	for (int currentLineIndex = 0; currentLineIndex < g_ScriptLines; ++currentLineIndex)
	{
		g_ppScripts[currentLineIndex] = (char*)malloc(MAX_SOURCE_LINE_SIZE + 1);
		fgets(g_ppScripts[currentLineIndex], MAX_SOURCE_LINE_SIZE, pScriptFile);
	}
	fclose(pScriptFile);
}

void UnloadScript()
{
	if (!g_ppScripts) 
	{
		return;
	}

	for (int i = 0; i < g_ScriptLines; i++)
	{
		free(g_ppScripts[i]);
	}
	free(g_ppScripts);
}

void RunScript()
{
	char pstrCommand[MAX_COMMAND_SIZE];
	char pstrStringParam[MAX_PARAM_SIZE];

	for (g_CurrentLine = 0; g_CurrentLine < g_ScriptLines; g_CurrentLine++)
	{
		g_CurrentLineChar = 0;
		GetCommand(pstrCommand);
		if (std::strcmp(pstrCommand, COMMAND_PRINTSTRING) == 0)
		{
			GetStringParam(pstrStringParam);
			printf("\t%s\n", pstrStringParam);
		}
		else if (std::strcmp(pstrCommand, COMMAND_PRINTSTRINGLOOP) == 0) 
		{
			GetStringParam(pstrStringParam);
			int loopCount = GetInitParam();
			for (int i = 0; i < loopCount; i++) 
			{
				printf("\t%d: %s\n", i, pstrStringParam);
			}
		}
		else if (std::strcmp(pstrCommand, COMMAND_NEWLINE) == 0) 
		{
			printf("\n");
		}
		else if (std::strcmp(pstrCommand, COMMAND_WAITFORKEYPRESS) == 0) 
		{
			while (_kbhit()) 
				_getch();
			while (!_kbhit());
		}
		else 
		{
			printf("\tError: Invalid Command!");
			break;
		}
	}
}

int main()
{
	LoadScript("test.txt");
	RunScript();
	UnloadScript();
	return 0;
}

