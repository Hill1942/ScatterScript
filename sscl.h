#ifndef SSCL_H
#define SSCL_H

namespace _cl
{
	void LoadScriptSource();
	void PreProcessSourceCode();
	void TestLexer();

	void ReadToken(Token token);
	
	void ParseSourceCode();
	void ParseStatement();
	void ParseBlock();
	void ParseVariable();
	void ParseHost();
	void ParseFunction();
	void ParseExpression();
	void ParseSubExpression();
	void ParseTerm();
	void ParseFactor();

	void ParseIf();
	void ParseWhile();
	void ParseBreak();
	void ParseFor();
	void ParseContinue();
	void ParseReturn();
	void ParseAssign();
	void ParseFunctionCall();

}

#endif

