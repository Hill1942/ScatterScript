#ifndef SSSYSTEM_H
#define SSSYSTEM_H

namespace _asm_
{
	void Init();
    void InitInstrTable();
    
    void Exit();
    void ShutDown();
    
    void ExitOnError(const char* errorMessage);
    void ExitOnCodeError(const char* errorMessage);
    void ExitOnCharExpectedError(char c);
}

namespace _vm
{
	void Init();
	void ShutDown();
}

namespace _cl
{
	void ExitOnInvalidInputError(char c);
    void CL_InitLexer();

	void Init();
}



#endif