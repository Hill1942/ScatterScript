
#include "ssbase_type.h"
#include "sstype.h"
#include "sslexeme.h"

extern LinkList g_FunctionTable;
extern LinkList g_StringTable;
extern LinkList g_LabelTable;
extern LinkList g_SymbolTable;

extern Lexer  g_Lexer;
extern ScriptHeader g_ScriptHeader;
extern Instr* g_InstrStream;

extern int g_InstrStreamSize;
extern int g_IsSetStackSizeFound;

extern InstrLookup g_InstrTable[MAX_INSTR_LOOKUP_COUNT];

void AssembleSourceFile()
{
    g_ScriptHeader.iStackSize = 0;
    g_ScriptHeader.iIsMainFuncPresent = FALSE;

    g_InstrStreamSize = 0;
    g_IsSetStackSizeFound = FALSE;
    g_ScriptHeader.iGlobalDataSize = 0;

    ResetLexer();

    while (TRUE)
    {
        if (GetNextToken() == END_OF_TOKEN_STREAM)
            break;

        switch (g_Lexer.currentToken)
        {
            case TOKEN_TYPE_SETSTACKSIZE:

        }
    }
}
















