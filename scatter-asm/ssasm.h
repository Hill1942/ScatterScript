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

void AssembleSourceFile();









#endif