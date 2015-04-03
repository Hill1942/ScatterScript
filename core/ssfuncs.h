#ifndef SSFUNCS_H
#define SSFUNCS_H


namespace _lib
{
	LinkList built_in_funcs;

	int AddBuiltInFunc(LinkList* builtInFuncTable, char* name);

	int GetBuiltInFunc(LinkList* builtInFuncTable, char* name);

}


#endif