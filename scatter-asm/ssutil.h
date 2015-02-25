#ifndef SSUTIL_H
#define SSUTIL_H

char* strtoupper(char* str);

void ExitOnError(const char* errorMessage);
void ExitOnCodeError(const char* errorMessage);
void ExitOnCharExpectedError(char c);
void Exit();
void ShutDown();

#endif