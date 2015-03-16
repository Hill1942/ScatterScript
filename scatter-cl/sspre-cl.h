#ifndef SSPRE_CL_H
#define <{1:}>SSPRE_CL_H

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

#define token_type_end_of_stream            0
#define token_type_int                      1
#define token_type_float                    2
#define token_type_ident                    3
#define token_type_keyword_var              4
#define token_type_keyword_true             5
#define token_type_keyword_false            6
#define token_type_keyword_if               7
#define token_type_keyword_else             8
#define token_type_keyword_break            9
#define token_type_keyword_continue         10
#define token_type_keyword_for              11
#define token_type_keyword_while            12
#define token_type_func                     13
#define token_type_return                   14

#define token_type_op                       15

#define token_type_delim_comma              16
#define token_type_delim_open_paren         17
#define token_type_delim_close_paren        18
#define token_type_delim_open_brace         19
#define token_type_delim_close_brace        20

#define token_type_delim_comma              16
#define token_type_delim_open_paren         17
#define token_type_delim_close_paren        18
#define token_type_delim_open_brace         19
#define token_type_delim_close_brace        20
#define token_type_delim_open_curly_brace   21
#define TOKEN_TYPE_DELIM_CLOSE_CURLY_BRACE  22
#define token_type_delim_semicolon          23
#define token_type_string                   24

#define op_type_add                         0
#define op_type_sub                         1
#define op_type_mul                         2
#define op_type_div                         3
#define op_type_mod                         4
#define op_type_exp                         5

#define op_type_inc                         6
#define op_type_dec                         7

#define op_type_assign_add                  8
#define op_type_assign_sub                  9
#define op_type_assign_mul                  10
#define op_type_assign_div                  11
#define op_type_assign_mod                  12
#define op_type_assign_exp                  13

#define op_type_and                         14
#define op_type_or                          15
#define op_type_xor                         16
#define op_type_not                         17
#define op_type_shift_left                  18
#define op_type_shift_right                 19

#define op_type_assign_and                  20
#define op_type_assign_or                   21
#define op_type_assign_xor                  22
#define op_type_assign_not                  23
#define op_type_assign_shift_left           24
#define op_type_assign_shift_right          25

#define op_type_logical_and                 26
#define op_type_logical_or                  27
#define op_type_logical_not                 28

#define op_type_equal                       29
#define op_type_not_equal                   30
#define op_type_less                        31
#define op_type_less_equal                  32
#define op_type_greater                     33
#define op_type_greater_equal               34




#endif
