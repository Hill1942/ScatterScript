#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sspre.h"
#include "ssbase_type.h"
#include "sslang.h"
#include "sscl.h"
#include "ssil.h"
#include "sssystem.h"

extern _cl::Compiler compiler;

 namespace _IL
 {
	 char strMnemontics[][12] =
	 {
		 "mov",
		 "add", "sub", "mul", "div", "mod", "neg", "inc", "dec",
		 "and", "or", "xor", "not", "shl", "shr",
		 "concat", "getchar", "setchar",
		 "jmp", "je", "jne", "jg", "jl", "jge", "jle",
		 "push", "pop",
		 "call", "ret", "callhost",
		 "pause", "exit"
	 };

	 void OutHeader()
	 {
		 fprintf(compiler.outAssembleFile, "; Asseble file: %s\n", compiler.outAssembleFilename);
		 fprintf(compiler.outAssembleFile, "; Source File: %s\n", compiler.scriptSourceFile);
		 fprintf(compiler.outAssembleFile, "; Version: %d.%d\n", VERSION_MAJOR, VERSION_MINOR);
		 fprintf(compiler.outAssembleFile, "; Generate Time: %s\n", "ddddd");
	 }
	 void OutDirectives()
	 {
		 int isAddNewLine = FALSE;

		 if (compiler.scriptHeader.iStackSize)
		 {
			 fprintf(compiler.outAssembleFile, "\tSetStackSize %d\n", compiler.scriptHeader.iStackSize);
			 isAddNewLine = TRUE;
		 }

		 if (isAddNewLine)
			 fprintf(compiler.outAssembleFile, "\n");
	 }
	 void OutScopeSymbols(int scope, int type)
	 {
		  int isAddNewLine = FALSE;

		  _cl::SymbolNode* pCurrentSymbol;
		  for (uint i = 0; i < compiler.symbolTable.iNodeCount; i++)
		  {
			  pCurrentSymbol = _cl::GetSymbol(&compiler.symbolTable, i);

			  if (pCurrentSymbol->iScope == scope && pCurrentSymbol->iType == type) 
			  {
				  fprintf(compiler.outAssembleFile, "\t");
				  if (scope != SCOPE_GLOBAL)
					  fprintf(compiler.outAssembleFile, "\t");

				  if (pCurrentSymbol->iType == SYMBOL_TYPE_PARAM)
					  fprintf(compiler.outAssembleFile, "param %s", pCurrentSymbol->strIdentifier);

				  if (pCurrentSymbol->iType == SYMBOL_TYPE_VAR) 
				  {
					  fprintf(compiler.outAssembleFile, "var %s", pCurrentSymbol->strIdentifier);
					  if (pCurrentSymbol->iSize > 1)
						fprintf(compiler.outAssembleFile, " [%d]", pCurrentSymbol->iSize);
				  }

				  fprintf(compiler.outAssembleFile, "\n");
				      isAddNewLine = TRUE; 
			  }
		  }

		  if (isAddNewLine)
			  fprintf(compiler.outAssembleFile, "\n");
	 }
	 void OutFunction(_cl::FuncNode* pFunction)
	 {
		 fprintf(compiler.outAssembleFile, "\tfunc %s\n", pFunction->strName);
		 fprintf(compiler.outAssembleFile, "\t{\n");

		 OutScopeSymbols(pFunction->iIndex, SYMBOL_TYPE_PARAM);
		 OutScopeSymbols(pFunction->iIndex, SYMBOL_TYPE_VAR);

		 if (pFunction->codeStream.iNodeCount > 0)
		 {
			 int isFirstSourceLine = TRUE;
			 for (int i = 0; i < pFunction->codeStream.iNodeCount; i++)
			 {
				 ILCodeNode* pCurrentNode = GetILCodeNode(&compiler.functionTable, pFunction->iIndex, i);
				 switch (pCurrentNode->iType)
				 {
				 case IL_CODE_NODE_SOURCE_LINE:
					 {
						 char* strSourceLine = pCurrentNode->strSoureLine;

						 int lastIndex = strlen(strSourceLine) - 1;
						 if (strSourceLine[lastIndex] == '\n')
							 strSourceLine[lastIndex] = '\0';

						 if (!isFirstSourceLine)
							 fprintf(compiler.outAssembleFile, "\n");

						 fprintf(compiler.outAssembleFile, "\t\t;%s\n\n", strSourceLine);

						 break;
					 }

				 case IL_CODE_NODE_INSTR:
					 {
						 fprintf(compiler.outAssembleFile, "\t\t%s", strMnemontics[pCurrentNode->instr.iOpCode]);
						 
						 int opCount = pCurrentNode->instr.OpList.iNodeCount;
						 if (opCount) 
						 {
							 fprintf(compiler.outAssembleFile, "\t");
							 if (strlen(strMnemontics[pCurrentNode->instr.iOpCode]) < TAB_WIDTH)
								 fprintf(compiler.outAssembleFile, "\t");
						 }

						 for (int i = 0; i < opCount; i++)
						 {
							 Oprand* oprand = GetILCodeOprand(pCurrentNode, i);
							 switch (oprand->iType)
							 {
							 case IL_OPRAND_TYPE_INT:
								 fprintf(compiler.outAssembleFile, "%d", oprand->iIntLiteral);
								 break;

							 case IL_OPRAND_TYPE_FLOAT:
							 	 fprintf(compiler.outAssembleFile, "%df", oprand->fFloatLiteral);
							 	 break;
							 
							 case IL_OPRAND_TYPE_STRING_INDEX:
							 	 fprintf(compiler.outAssembleFile, "\"%s\"",
									 _cl::GetString(&compiler.stringTable, oprand->iStringIndex));
							 	 break;
							 
							 case IL_OPRAND_TYPE_VARIABLE:
							 	 fprintf(compiler.outAssembleFile, "%s",
									 _cl::GetSymbol(&compiler.symbolTable, oprand->iSymbolIndex)->strIdentifier);
							 	 break;
							 
							 case IL_OPRAND_TYPE_ABS_ARRAY_INDEX:
							 	 fprintf(compiler.outAssembleFile, "%s[%d]", 
									 _cl::GetSymbol(&compiler.symbolTable, oprand->iSymbolIndex)->strIdentifier,
									 oprand->iOffset);
							 	 break;
							 
							 case IL_OPRAND_TYPE_REL_ARRAY_INDEX:
							 	 fprintf(compiler.outAssembleFile, "%s[%s]", oprand->iIntLiteral,
									  _cl::GetSymbol(&compiler.symbolTable, oprand->iSymbolIndex)->strIdentifier,
									  _cl::GetSymbol(&compiler.symbolTable, oprand->iOffsetSymbolIndex)->strIdentifier);
							 	 break;
							 
							 case IL_OPRAND_TYPE_FUNC_INDEX:
							 	 fprintf(compiler.outAssembleFile, "%s", 
									  _cl::GetFunction(&compiler.functionTable, oprand->iFuncIndex)->strName);
							 	 break;

							 case IL_OPRAND_TYPE_BUILTIN_FUNC_INDEX:
								 fprintf(compiler.outAssembleFile, "%s", 
									 _cl::GetBuiltInFunc(&compiler.builInFuncTable, oprand->iFuncIndex)->strName);
								 break;

							 case IL_OPRAND_TYPE_REG:
							 	 fprintf(compiler.outAssembleFile, "_retval");
							 	 break;

							 case IL_OPRAND_TYPE_JUMP_TARGET_INDEX:
								 fprintf(compiler.outAssembleFile, "_L%d", oprand->iJumpTargetIndex);
								 break;

							 default:
								 break;
							 }

							 if (i != opCount - 1)
								 fprintf(compiler.outAssembleFile, ", ");
						 }

						 fprintf(compiler.outAssembleFile, "\n");

						 break;
					 }

				 case IL_CODE_NODE_JUMP_TARGET:
					 {
						 fprintf(compiler.outAssembleFile, "\t_L%d:\n", pCurrentNode->iJumpTargetIndex);
						 break;
					 }
				 }

				 if (isFirstSourceLine)
					 isFirstSourceLine = FALSE;

			 }
		 }
		 else 
		 {
			 fprintf(compiler.outAssembleFile, "\t\t; (no code)\n");
         }

		 fprintf(compiler.outAssembleFile, "\t}");
	 }
	 void OutCode()
	 {
		 compiler.outAssembleFile = fopen(compiler.outAssembleFilename, "wb");
		 if (compiler.outAssembleFile == NULL)
			 _cl::ExitOnCodeError("Could not open out asm file");

		 OutHeader();
		 fprintf(compiler.outAssembleFile, "\n");

		 fprintf(compiler.outAssembleFile, ";---- Directives ----------------------\n");
		 OutDirectives();
		 fprintf(compiler.outAssembleFile, "\n");

		 fprintf(compiler.outAssembleFile, ";---- Global Varibles -------------------------\n");
		 OutScopeSymbols(SCOPE_GLOBAL, SYMBOL_TYPE_VAR);
		 fprintf(compiler.outAssembleFile, "\n");

		 fprintf(compiler.outAssembleFile, ";---- Functions -----------------------------\n");
		 LinkListNode* pNode = compiler.functionTable.pHead;
		 _cl::FuncNode* pFunction;
		 _cl::FuncNode* pMainFunction = NULL;
		 if (compiler.functionTable.iNodeCount > 0) 
		 {
			 while (TRUE)
			 {
				 pFunction = (_cl::FuncNode*)pNode->pData;
				 if (!pFunction->isHostAPI) 
				 {
					 if (strcmp(pFunction->strName, ASM_KW_MAIN_FUNC_NAME) == 0)
					 {
						 pMainFunction = pFunction;
					 }
					 else
					 {
						 OutFunction(pFunction);
						 fprintf(compiler.outAssembleFile, "\n\n");
					 }
				 }

				 pNode = pNode->pNext;
				 if (pNode == NULL)
					 break;
			 }
		 }
		 fprintf(compiler.outAssembleFile, "\n");

		 fprintf(compiler.outAssembleFile, ";---- Main ----------------------------\n");
		 if (pMainFunction)
		 {
			 OutFunction(pMainFunction);
		 }

		 fclose(compiler.outAssembleFile);
		 
		 compiler.isOutCode = TRUE;
	 }


 }
