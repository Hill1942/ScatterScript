#ifndef SSSYSTEM_H
#define SSSYSTEM_H

void ASM_Init();
void InitInstrTable();

void Exit();
void ASM_ShutDown();

void ExitOnError(const char* errorMessage);
void ExitOnCodeError(const char* errorMessage);
void ExitOnCharExpectedError(char c);


#endif