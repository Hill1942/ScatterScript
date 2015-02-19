#ifndef SSUTIL_H
#define SSUTIL_H

char* strtoupper(char* str);

void ExitOnError(const char* errorMessage);
void Exit();
void ShutDown();

#endif