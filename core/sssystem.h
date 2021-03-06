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
	void InitBuiltInFuncs();
	void ShutDown();
}

namespace _cl
{
	void ExitOnError(char* strErrorMsg);
	void ExitOnCodeError(char* strErrorMsg);
	void ExitOnInvalidInputError(char c);

	void Init();
	void ShutDown();
	void Exit();
}



#endif