#ifndef SSSYSTEM_H
#define SSSYSTEM_H

void Init();
void InitInstrTable();

void Exit();
void ShutDown();

void ExitOnError(const char* errorMessage);
void ExitOnCodeError(const char* errorMessage);
void ExitOnCharExpectedError(char c);


#endif