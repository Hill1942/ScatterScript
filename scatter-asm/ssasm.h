#ifndef  SSASM_H
#define  SSAMS_H



int IsCharNumeric(char cChar);
int IsCharWhiteSpace(char cChar);
int IsCharIdent(char cChar);
int IsCharDelimiter(char cChar);

int IsStringInt(const char* str);
int IsStringFloat(const char* str);
int IsStringWhitespace(const char* str);
int IsStringIdent(const char* str);

void ExitOnError(const char* errorMessage);
void Exit();
void ShutDown();




#endif