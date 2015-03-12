#ifndef SSASM_PRE_H
#define SSASM_PRE_H

#define VERSION_MAJOR        0
#define VERSION_MINOR        1

#define ASM_SOURCE_EXTENSION ".ssam"
#define	SOURCE_EXTENSION     ".ss"
#define EXE_EXTENSION        ".sse"
#define SSE_ID_STRING        "sse0"

#define INSTR_MOV            0
#define INSTR_ADD            1
#define INSTR_SUB            2
#define INSTR_MUL            3
#define INSTR_DIV            4
#define INSTR_MOD            5
#define INSTR_EXP            6
#define INSTR_NEG            7
#define INSTR_INC            8
#define INSTR_DEC            9

#define INSTR_AND            10
#define INSTR_OR             11
#define INSTR_XOR            12
#define INSTR_NOT            13
#define INSTR_SHL            14
#define INSTR_SHR            15

#define INSTR_CONCAT         16
#define INSTR_GETCHAR        17
#define INSTR_SETCHAR        18

#define INSTR_JMP            19
#define INSTR_JE             20
#define INSTR_JNE            21
#define INSTR_JG             22
#define INSTR_JL             23
#define INSTR_JGE            24
#define INSTR_JLE            25

#define INSTR_PUSH           26
#define INSTR_POP            27

#define INSTR_CALL           28
#define INSTR_RET            29
#define INSTR_CALLHOST       30

#define INSTR_PAUSE          31
#define INSTR_EXIT           32

#define INSTR_PRINT          33

#define OP_TYPE_INT                 0
#define OP_TYPE_FLOAT               1
#define OP_TYPE_STRING_INDEX        2
#define OP_TYPE_ABS_STACK_INDEX     3
#define OP_TYPE_REL_STACK_INDEX     4
#define OP_TYPE_INSTR_INDEX         5
#define OP_TYPE_FUNC_INDEX          6
#define OP_TYPE_HOST_API_CALL_INDEX 7
#define OP_TYPE_REG                 8
#define OP_TYPE_NULL                -1

#define OP_FLAG_TYPE_INT            1
#define OP_FLAG_TYPE_FLOAT          2
#define OP_FLAG_TYPE_STRING         4
#define OP_FLAG_TYPE_MEM_REF        8
#define OP_FLAG_TYPE_LINE_LABEL     16
#define OP_FLAG_TYPE_FUNC_NAME      32
#define OP_FLAG_TYPE_HOST_API_CALL  64
#define OP_FLAG_TYPE_REG            128

#define TOKEN_TYPE_INT              0
#define TOKEN_TYPE_FLOAT            1
#define TOKEN_TYPE_STRING           2
#define TOKEN_TYPE_QUATE            3
#define TOKEN_TYPE_IDENT            4
#define TOKEN_TYPE_COLON            5
#define TOKEN_TYPE_OPEN_BRACKET     6
#define TOKEN_TYPE_CLOSE_BRACKET    7
#define TOKEN_TYPE_COMMA            8
#define TOKEN_TYPE_OPEN_BRACE       9
#define TOKEN_TYPE_CLOSE_BRACE      10
#define TOKEN_TYPE_NEWLINE          11
#define TOKEN_TYPE_INSTR            12
#define TOKEN_TYPE_SETSTACKSIZE     13
#define TOKEN_TYPE_VAR              14
#define TOKEN_TYPE_FUNC             15
#define TOKEN_TYPE_PARAM            16
#define TOKEN_TYPE_REG_RETVAL       17
#define TOKEN_TYPE_INVALID          18
#define END_OF_TOKEN_STREAM         19

#define LEX_STATE_NO_STRING         0
#define LEX_STATE_IN_STRING         1
#define LEX_STATE_END_STRING        2

#define MAX_SOURCE_LINE_SIZE        4096
#define MAX_INFO_LINE_SIZE          256
#define MAX_INDENT_SIZE             256
#define MAX_INSTR_LOOKUP_COUNT      256
#define MAX_INSTR_MNEMONIC_SIZE     16
#define MAX_LEXEME_SIZE             256
#define MAX_FILENAME_SIZE           2048

#define KW_MAIN_FUNC_NAME           "_main"
#define KW_SET_STACK_SIZE           "setstacksize"
#define KW_VAR                      "var"
#define KW_FUNCTION                 "func"
#define KW_PARAM                    "param"
#define KW_RETVAL                   "_retval"
#define KW_PRINT                    "_print"

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
