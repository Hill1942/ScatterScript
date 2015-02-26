#ifndef  SSASM_H
#define  SSAMS_H

#define INSTR_MOVE       0
#define INSTR_ADD        1
#define INSTR_SUB        2
#define INSTR_MUL        3
#define INSTR_DIV        4
#define INSTR_MOD        5
#define INSTR_EXP        6
#define INSTR_NEG        7
#define INSTR_INC        8
#define INSTR_DEC        9

#define INSTR_AND        10
#define INSTR_OR         11
#define INSTR_XOR        12
#define INSTR_NOT        13
#define INSTR_SHL        14
#define INSTR_SHR        15

#define INSTR_CONCAT     16
#define INSTR_GETCHAR    17
#define INSTR_SETCHAR    18

#define INSTR_JMP        19
#define INSTR_JE         20
#define INSTR_JNE        21
#define INSTR_JG         22
#define INSTR_JL         23
#define INSTR_JGE        24
#define INSTR_JLE        25

#define INSTR_PUSH       26
#define INSTR_POP        27

#define INSTR_CALL       28
#define INSTR_RET        29
#define INSTR_CALLHOST   30

#define INSTR_PAUSE      31
#define INSTR_EXIT       32

#define OP_TYPE_INT                 0
#define OP_TYPE_FLOAT               1
#define OP_TYPE_STRING_INDEX        2
#define OP_TYPE_ABS_STACK_INDEX     3
#define OP_TYPE_REL_STACK_INDEX     4
#define OP_TYPE_INSTR_INDEX         5
#define OP_TYPE_FUNC_INDEX          6
#define OP_TYPE_HOST_API_CALL_INDEX 7
#define OP_TYPE_REG                 8

#define ASM_SOURCE_EXTENSION        ".ssam"
#define	SOURCE_EXTENSION            ".ss"
#define EXE_EXTENSION               ".sse"

#define SSE_ID_STRING               "sse0"
#define VERSION_MAJOR               0
#define VERSION_MINOR               1

#define MAIN_FUNC_NAME   "_Main"



#define ERROR_MSG_INVALID_INPUT            \
	"Invalid input"
#define ERROR_MSG_LOCAL_SETSTACKSIZE       \
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




void AssembleSourceFile();









#endif