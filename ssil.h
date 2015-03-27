#ifndef SSIL_H
#define SSIL_H

namespace _IL
{
	void OutHeader();
	void OutDirectives();
	void OutScopeSymbols(int scope, int type);
	void OutFunction(_cl::FuncNode* pFunction);
	void OutCode();
}

#endif