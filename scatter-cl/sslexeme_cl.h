#ifndef SSLEXEME_CL_H
#define SSLEXEME_CL_H

#define MAX_LEXEME_SIZE                     1024
#define MAX_OF_STATE_COUNT                  32
#define MAX_DELIMITER_COUNT                 24

#define LEX_STATE_START                     0
#define LEX_STATE_INT                       1
#define LEX_STATE_FLOAT                     2
#define LEX_STATE_IDENT                     5
#define LEX_STATE_OP                        6
#define LEX_STATE_DELIMITER                 7
#define LEX_STATE_STRING                    8
#define LEX_STATE_STRING_ESCAPE             9
#define LEX_STATE_STRING_CLOSE_QUOTE        10

#define TOKEN_TYPE_END_OF_STREAM            0
#define TOKEN_TYPE_INT                      1
#define TOKEN_TYPE_FLOAT                    2
#define TOKEN_TYPE_IDENT                    3
#define TOKEN_TYPE_KEYWORD_VAR              4
#define TOKEN_TYPE_KEYWORD_TRUE             5
#define TOKEN_TYPE_KEYWORD_FALSE            6
#define TOKEN_TYPE_KEYWORD_IF               7
#define TOKEN_TYPE_KEYWORD_ELSE             8
#define TOKEN_TYPE_KEYWORD_BREAK            9
#define TOKEN_TYPE_KEYWORD_CONTINUE         10
#define TOKEN_TYPE_KEYWORD_FOR              11
#define TOKEN_TYPE_KEYWORD_WHILE            12
#define TOKEN_TYPE_FUNC                     13
#define TOKEN_TYPE_RETURN                   14

#define TOKEN_TYPE_OP                       15

#define TOKEN_TYPE_DELIM_COMMA              16
#define TOKEN_TYPE_DELIM_OPEN_PAREN         17
#define TOKEN_TYPE_DELIM_CLOSE_PAREN        18
#define TOKEN_TYPE_DELIM_OPEN_BRACE         19
#define TOKEN_TYPE_DELIM_CLOSE_BRACE        20
#define TOKEN_TYPE_DELIM_OPEN_CURLY_BRACE   21
#define TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE  22
#define TOKEN_TYPE_DELIM_SEMICOLON          23
#define TOKEN_TYPE_STRING                   24

#define OP_TYPE_ADD                         0
#define OP_TYPE_SUB                         1
#define OP_TYPE_MUL                         2
#define OP_TYPE_DIV                         3
#define OP_TYPE_MOD                         4

#define OP_TYPE_ASSIGN_ADD                  10
#define OP_TYPE_ASSIGN_SUB                  11
#define OP_TYPE_ASSIGN_MUL                  12
#define OP_TYPE_ASSIGN_DIV                  13
#define OP_TYPE_ASSIGN_MOD                  14

#define OP_TYPE_AND                         20
#define OP_TYPE_OR                          21
#define OP_TYPE_XOR                         22
#define OP_TYPE_NOT                         23
#define OP_TYPE_SHIFT_LEFT                  24
#define OP_TYPE_SHIFT_RIGHT                 25

#define OP_TYPE_ASSIGN_AND                  30
#define OP_TYPE_ASSIGN_OR                   31
#define OP_TYPE_ASSIGN_XOR                  32
#define OP_TYPE_ASSIGN_SHIFT_LEFT           33
#define OP_TYPE_ASSIGN_SHIFT_RIGHT          34

#define OP_TYPE_INC                         40
#define OP_TYPE_DEC                         41

#define OP_TYPE_LOGICAL_AND                 50  // &&
#define OP_TYPE_LOGICAL_OR                  51  // ||
#define OP_TYPE_LOGICAL_NOT                 52  // !

#define OP_TYPE_EQUAL                       61
#define OP_TYPE_NOT_EQUAL                   62
#define OP_TYPE_LESS                        63
#define OP_TYPE_LESS_EQUAL                  64
#define OP_TYPE_GREATER                     65
#define OP_TYPE_GREATER_EQUAL               66

#define MAX_OP_STATE_COUNT                  32


typedef int Token;

typedef struct _OpState
{
	char cChar;
	int  iSubStateIndex;
	int  iSubStateCount;
	int  iIndex;
}OpState;


#endif
