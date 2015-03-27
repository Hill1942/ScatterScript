#ifndef SSPRE_H
#define SSPRE_H

#define VERSION_MAJOR                          0
#define VERSION_MINOR                          1

#define ASM_SOURCE_EXTENSION                   ".ssam"
#define	SOURCE_EXTENSION                       ".ss"
#define EXE_EXTENSION                          ".sse"
#define SSE_ID_STRING                          "sse0"

#define DEFAULT_STACK_SIZE                     1024
#define TAB_WIDTH                              4
#define SCOPE_GLOBAL                           0
#define SYMBOL_TYPE_PARAM                      1
#define SYMBOL_TYPE_VAR                        2

#define MAX_VALUE_STRING_SIZE                  4096
#define MAX_THREAD_NUMBER                      1024
#define MAX_SOURCE_LINE_SIZE                   4096
#define MAX_INFO_LINE_SIZE                     256
#define MAX_INDENT_SIZE                        256
#define MAX_INSTR_LOOKUP_COUNT                 256
#define MAX_INSTR_MNEMONIC_SIZE                16
#define MAX_FILENAME_SIZE                      2048
#define MAX_LEXEME_SIZE                        1024
#define MAX_OF_STATE_COUNT                     32
#define MAX_DELIMITER_COUNT                    24
#define MAX_OP_STATE_COUNT                     32

//ASM Instruction
#define ASM_INSTR_MOV                          0
#define ASM_INSTR_ADD                          1
#define ASM_INSTR_SUB                          2
#define ASM_INSTR_MUL                          3
#define ASM_INSTR_DIV                          4
#define ASM_INSTR_MOD                          5
#define ASM_INSTR_EXP                          6
#define ASM_INSTR_NEG                          7
#define ASM_INSTR_INC                          8
#define ASM_INSTR_DEC                          9

#define ASM_INSTR_AND                          10
#define ASM_INSTR_OR                           11
#define ASM_INSTR_XOR                          12
#define ASM_INSTR_NOT                          13
#define ASM_INSTR_SHL                          14
#define ASM_INSTR_SHR                          15
								               
#define ASM_INSTR_CONCAT                       16
#define ASM_INSTR_GETCHAR                      17
#define ASM_INSTR_SETCHAR                      18
								               
#define ASM_INSTR_JMP                          19
#define ASM_INSTR_JE                           20
#define ASM_INSTR_JNE                          21
#define ASM_INSTR_JG                           22
#define ASM_INSTR_JL                           23
#define ASM_INSTR_JGE                          24
#define ASM_INSTR_JLE                          25
								               
#define ASM_INSTR_PUSH                         26
#define ASM_INSTR_POP                          27
								               
#define ASM_INSTR_CALL                         28
#define ASM_INSTR_RET                          29
#define ASM_INSTR_CALLHOST                     30
								               
#define ASM_INSTR_PAUSE                        31
#define ASM_INSTR_EXIT                         32
								               
#define ASM_INSTR_PRINT                        33

//
#define ASM_OPRAND_TYPE_INT                    0
#define ASM_OPRAND_TYPE_FLOAT                  1
#define ASM_OPRAND_TYPE_STRING_INDEX           2
#define ASM_OPRAND_TYPE_ABS_STACK_INDEX        3
#define ASM_OPRAND_TYPE_REL_STACK_INDEX        4
#define ASM_OPRAND_TYPE_INSTR_INDEX            5
#define ASM_OPRAND_TYPE_FUNC_INDEX             6
#define ASM_OPRAND_TYPE_HOST_API_CALL_INDEX    7
#define ASM_OPRAND_TYPE_REG                    8
#define ASM_OPRAND_TYPE_NULL                   -1
											   
#define ASM_OPRAND_FLAG_TYPE_INT               1
#define ASM_OPRAND_FLAG_TYPE_FLOAT             2
#define ASM_OPRAND_FLAG_TYPE_STRING            4
#define ASM_OPRAND_FLAG_TYPE_MEM_REF           8
#define ASM_OPRAND_FLAG_TYPE_LINE_LABEL        16
#define ASM_OPRAND_FLAG_TYPE_FUNC_NAME         32
#define ASM_OPRAND_FLAG_TYPE_HOST_API_CALL     64
#define ASM_OPRAND_FLAG_TYPE_REG               128

#define ASM_TOKEN_TYPE_INT                     0
#define ASM_TOKEN_TYPE_FLOAT                   1
#define ASM_TOKEN_TYPE_STRING                  2
#define ASM_TOKEN_TYPE_QUATE                   3
#define ASM_TOKEN_TYPE_IDENT                   4
#define ASM_TOKEN_TYPE_COLON                   5
#define ASM_TOKEN_TYPE_OPEN_BRACKET            6
#define ASM_TOKEN_TYPE_CLOSE_BRACKET           7
#define ASM_TOKEN_TYPE_COMMA                   8
#define ASM_TOKEN_TYPE_OPEN_BRACE              9
#define ASM_TOKEN_TYPE_CLOSE_BRACE             10
#define ASM_TOKEN_TYPE_NEWLINE                 11
#define ASM_TOKEN_TYPE_INSTR                   12
#define ASM_TOKEN_TYPE_SETSTACKSIZE            13
#define ASM_TOKEN_TYPE_VAR                     14
#define ASM_TOKEN_TYPE_FUNC                    15
#define ASM_TOKEN_TYPE_PARAM                   16
#define ASM_TOKEN_TYPE_REG_RETVAL              17
#define ASM_TOKEN_TYPE_INVALID                 18
#define ASM_END_OF_TOKEN_STREAM                19
										       
#define ASM_LEX_STATE_NO_STRING                0
#define ASM_LEX_STATE_IN_STRING                1
#define ASM_LEX_STATE_END_STRING               2

#define ASM_KW_MAIN_FUNC_NAME                  "_main"
#define ASM_KW_SET_STACK_SIZE                  "setstacksize"
#define ASM_KW_VAR                             "var"
#define ASM_KW_FUNCTION                        "func"
#define ASM_KW_PARAM                           "param"
#define ASM_KW_RETVAL                          "_retval"
#define ASM_KW_PRINT                           "_print"


#define VM_LOAD_OK                             0
#define VM_LOAD_ERROR_FAIL_FILE_OPEN           1
#define VM_LOAD_ERROR_INVALID_SSE              2
#define VM_LOAD_ERROR_UNSOPPORTED_VERSION      3
#define VM_LOAD_ERROR_OUT_OF_MEMORY            4
#define VM_LOAD_ERROR_OUT_OF_THREAD            5


#define IL_INSTR_MOV                           0
#define IL_INSTR_ADD                           1
#define IL_INSTR_SUB                           2
#define IL_INSTR_MUL                           3
#define IL_INSTR_DIV                           4
#define IL_INSTR_MOD                           5
#define IL_INSTR_EXP                           6
#define IL_INSTR_NEG                           7
#define IL_INSTR_INC                           8
#define IL_INSTR_DEC                           9
											   
#define IL_INSTR_AND                           10
#define IL_INSTR_OR                            11
#define IL_INSTR_XOR                           12
#define IL_INSTR_NOT                           13
#define IL_INSTR_SHL                           14
#define IL_INSTR_SHR                           15
											   
#define IL_INSTR_CONCAT                        16
#define IL_INSTR_GETCHAR                       17
#define IL_INSTR_SETCHAR                       18
											   
#define IL_INSTR_JMP                           19
#define IL_INSTR_JE                            20
#define IL_INSTR_JNE                           21
#define IL_INSTR_JG                            22
#define IL_INSTR_JL                            23
#define IL_INSTR_JGE                           24
#define IL_INSTR_JLE                           25
											   
#define IL_INSTR_PUSH                          26
#define IL_INSTR_POP                           27
											   
#define IL_INSTR_CALL                          28
#define IL_INSTR_RET                           29
#define IL_INSTR_CALLHOST                      30
											   
#define IL_INSTR_PAUSE                         31
#define IL_INSTR_EXIT                          32

#define IL_OPRAND_TYPE_INT                     0
#define IL_OPRAND_TYPE_FLOAT                   1
#define IL_OPRAND_TYPE_STRING_INDEX            2
#define IL_OPRAND_TYPE_VARIABLE                3
#define IL_OPRAND_TYPE_ABS_ARRAY_INDEX         4
#define IL_OPRAND_TYPE_REL_ARRAY_INDEX         5
#define IL_OPRAND_TYPE_JUMP_TARGET_INDEX       6
#define IL_OPRAND_TYPE_FUNC_INDEX              7
#define IL_OPRAND_TYPE_REG                     8

#define IL_CODE_NODE_INSTR                     0
#define IL_CODE_NODE_SOURCE_LINE               1
#define IL_CODE_NODE_JUMP_TARGET               2


#define CL_LEX_STATE_START                     0
#define CL_LEX_STATE_INT                       1
#define CL_LEX_STATE_FLOAT                     2
#define CL_LEX_STATE_IDENT                     5
#define CL_LEX_STATE_OP                        6
#define CL_LEX_STATE_DELIMITER                 7
#define CL_LEX_STATE_STRING                    8
#define CL_LEX_STATE_STRING_ESCAPE             9
#define CL_LEX_STATE_STRING_CLOSE_QUOTE        10

#define CL_TOKEN_TYPE_END_OF_STREAM            0
#define CL_TOKEN_TYPE_INT                      1
#define CL_TOKEN_TYPE_FLOAT                    2
#define CL_TOKEN_TYPE_IDENT                    3
#define CL_TOKEN_TYPE_KEYWORD_VAR              4
#define CL_TOKEN_TYPE_KEYWORD_TRUE             5
#define CL_TOKEN_TYPE_KEYWORD_FALSE            6
#define CL_TOKEN_TYPE_KEYWORD_IF               7
#define CL_TOKEN_TYPE_KEYWORD_ELSE             8
#define CL_TOKEN_TYPE_KEYWORD_BREAK            9
#define CL_TOKEN_TYPE_KEYWORD_CONTINUE         10
#define CL_TOKEN_TYPE_KEYWORD_FOR              11
#define CL_TOKEN_TYPE_KEYWORD_WHILE            12
#define CL_TOKEN_TYPE_KEYWORD_FUNC             13
#define CL_TOKEN_TYPE_KEYWORD_RETURN           14
#define CL_TOKEN_TYPE_OPERATOR                 15
#define CL_TOKEN_TYPE_DELIM_COMMA              16
#define CL_TOKEN_TYPE_DELIM_OPEN_PAREN         17
#define CL_TOKEN_TYPE_DELIM_CLOSE_PAREN        18
#define CL_TOKEN_TYPE_DELIM_OPEN_BRACE         19
#define CL_TOKEN_TYPE_DELIM_CLOSE_BRACE        20
#define CL_TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE   21
#define CL_TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE  22
#define CL_TOKEN_TYPE_DELIM_SEMICOLON          23
#define CL_TOKEN_TYPE_STRING                   24

#define CL_OPERATOR_TYPE_ADD                   0		//  +
#define CL_OPERATOR_TYPE_SUB                   1		//	-
#define CL_OPERATOR_TYPE_MUL                   2		//	*
#define CL_OPERATOR_TYPE_DIV                   3		//	/
#define CL_OPERATOR_TYPE_MOD                   4		//	%
														
#define CL_OPERATOR_TYPE_INC                   40		//  ++
#define CL_OPERATOR_TYPE_DEC                   41		//  --
#define CL_OPERATOR_TYPE_ASSIGN                42		//  =
														
#define CL_OPERATOR_TYPE_ASSIGN_ADD            10		//  +=
#define CL_OPERATOR_TYPE_ASSIGN_SUB            11		//	-=
#define CL_OPERATOR_TYPE_ASSIGN_MUL            12		//	*=
#define CL_OPERATOR_TYPE_ASSIGN_DIV            13		//	/=
#define CL_OPERATOR_TYPE_ASSIGN_MOD            14		//	%=
														
#define CL_OPERATOR_TYPE_AND                   20		//  &
#define CL_OPERATOR_TYPE_OR                    21		//	|
#define CL_OPERATOR_TYPE_XOR                   22		//	^
#define CL_OPERATOR_TYPE_NOT                   23		//	~
#define CL_OPERATOR_TYPE_SHIFT_LEFT            24		//	<<
#define CL_OPERATOR_TYPE_SHIFT_RIGHT           25		//	>>
														
#define CL_OPERATOR_TYPE_ASSIGN_AND            30		//	&=
#define CL_OPERATOR_TYPE_ASSIGN_OR             31		//	|=
#define CL_OPERATOR_TYPE_ASSIGN_XOR            32		//	^=
#define CL_OPERATOR_TYPE_ASSIGN_SHIFT_LEFT     33		//	<<=
#define CL_OPERATOR_TYPE_ASSIGN_SHIFT_RIGHT    34		//	>>=
														
#define CL_OPERATOR_TYPE_LOGICAL_AND           50		//	&&
#define CL_OPERATOR_TYPE_LOGICAL_OR            51		//	||
#define CL_OPERATOR_TYPE_LOGICAL_NOT           52		//	!
														
#define CL_OPERATOR_TYPE_EQUAL                 61		//	==
#define CL_OPERATOR_TYPE_NOT_EQUAL             62		//	!=
#define CL_OPERATOR_TYPE_LESS                  63		//	<
#define CL_OPERATOR_TYPE_LESS_EQUAL            64		//	<=
#define CL_OPERATOR_TYPE_GREATER               65		//	>
#define CL_OPERATOR_TYPE_GREATER_EQUAL         66		//	>=

#define CL_KW_MAIN_FUNC_NAME                   "_main"
#define CL_KW_VAR                              "var"
#define CL_KW_TRUE                             "true"
#define CL_KW_FALSE                            "false"
#define CL_KW_IF                               "if"
#define CL_KW_ELSE                             "else"
#define CL_KW_FOR                              "for"
#define CL_KW_WHILE                            "while"
#define CL_KW_BREAK                            "break"
#define CL_KW_CONTINUE                         "continue"
#define CL_KW_FUNC                             "func"
#define CL_KW_RETURN                           "return"

#define ERROR_MSG_INVALID_INPUT             \
	"Invalid input"
#define ERROR_MSG_LOCAL_SETSTACKSIZE        \
	"SetStackSize can only appear in the global scope"
#define ERROR_MSG_INVALID_STACK_SIZE        \
	"Invalid stack size"
#define ERROR_MSG_MULTIPLE_SETSTACKSIZE     \
	"Multiple instances of SetStackSize illegal"
#define ERROR_MSG_IDENT_EXPECTED 			\
	"Indetifier expected"
#define ERROR_MSG_INVALID_ARRAY_SIZE		\
	"Invalid array size"
#define ERROR_MSG_IDENT_REDEFINITION		\
	"Identifier redefinition"
#define ERROR_MSG_UNDEFINED_IDENT			\
	"Undefined identifier"
#define ERROR_MSG_NESTED_FUNC				\
	"Nested functions illegal"
#define ERROR_MSG_FUNC_REDEFINITION			\
	"Function redefinition"
#define ERROR_MSG_UNDEFINED_FUNC			\
	"Undefined function"
#define ERROR_MSG_GLOBAL_PARAM				\
	"Parameters can only appear inside functions"
#define ERROR_MSG_MAIN_PARAM				\
	"_Main() functoin cannot accept param"
#define ERROR_MSG_GLOBAL_LINE_LABEL			\
	"Line labels can only appear inside functions"
#define ERROR_MSG_LINE_LABEL_REDEFINITION	\
	"Line label redefinition"
#define ERROR_MSG_UNDEFINED_LINE_LABEL		\
	"Undefined line label"
#define ERROR_MSG_GLOBAL_INSTR				\
	"Instructions can only appear inside functions"
#define ERROR_MSG_INVALID_INSTR				\
	"Invalid instruction"
#define ERROR_MSG_INVALID_OP				\
	"Invalid operand"
#define ERROR_MSG_INVALID_STRING			\
	"Invalid string"
#define ERROR_MSG_INVALID_ARRAY_NOT_INDEXED	\
	"Arrays must be indexed"
#define ERROR_MSG_INVALID_ARRAY				\
	"Invalid array"
#define ERROR_MSG_INVALID_ARRAY_INDEX		\
	"Invalid array index"

#endif
