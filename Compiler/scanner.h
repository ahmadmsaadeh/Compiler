#pragma once
/* scanner .h*/
#include "fd.h"

#ifndef __SCANNER__H
#define __SCANNER__H


//Token types enumeration
typedef enum
{
	/* Literals */
	lx_identifier, lx_integer, lx_string, lx_float,
	/* Keywords */
	kw_program,
	kw_var, kw_constant, kw_integer, kw_boolean, kw_string, kw_float,
	kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,
	kw_while, kw_do, kw_od,
	kw_and, kw_or,
	kw_read, kw_write,
	kw_for, kw_from, kw_to, kw_by,

	//kw_function, kw_procedure,kw_return, kw_not, kw_begin, kw_end,

	/* Operators */
	lx_Iparen, lx_rparen, Ix_colon, x_colon_eq, lx_plus, lx_minus,
	lx_star, lx_slash, lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge,
	lx_dot, lx_semicolon, lx_lbracket, lx_rbracket, lx_comma, lx_eof,
	lx_lcbracket, lx_rcbracket,
}
LEXEME_TYPE;
char c_eof = ' ';
// Definition of TOKEN, you can use a class if you  want 
struct TOKEN
{
	LEXEME_TYPE type;
	int value;  // can be used instead of the str_ptr for IDs and strings
	float float_value;

	char* str_ptr; // points to strings or Identifiers, can use value
	//instead but with type casting
};

class SCANNER {
private:
	FileDescriptor* Fd;
	// define your functions ..
	//I would define the following functions..
	void skip_comments();
	TOKEN* check_keyword(const char* str);
	TOKEN* get_id(const char* first_char);
	TOKEN* get_op(const char* op);
	TOKEN* get_string(const char* str);
	TOKEN* get_int(const char* first_char); // gets integers and floats

public:
	SCANNER();
	SCANNER(FileDescriptor* fd);
	TOKEN* Scan(FileDescriptor* fd);
	// You may define it as TOKEN *Scan(FileDescriptor *fd);

};
int keys = 24; /* number of keywords */
const char* keyword[] = {
	"program", "var", "constant", "integer", "bool", "string",
	"float", "true", "false", "if", "fi", "then", "else", "while",
	"do", "od", "and", "or", "read", "write", "for", "from", "to",
	"by", nullptr
};


LEXEME_TYPE key_type[] = {
	kw_program, kw_var, kw_constant, kw_integer, kw_boolean, kw_string,
	kw_float,kw_true, kw_false, kw_if, kw_fi, kw_then, kw_else,kw_while,
	kw_do, kw_od,kw_and, kw_or,kw_read, kw_write,kw_for, kw_from, kw_to,
	kw_by,
};


const char* operator_list[] = {
	"(", ")", ":", ":=", "+", "-",
	"*", "/", "=", "!=", "<", "<=", ">", ">=",
	".", ";", "[", "]", ",", "", 
	"{", "}",
};
LEXEME_TYPE operator_type[] = {
	lx_Iparen, lx_rparen, Ix_colon, x_colon_eq, lx_plus, lx_minus,
	lx_star, lx_slash, lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge,
	lx_dot, lx_semicolon, lx_lbracket, lx_rbracket, lx_comma, lx_eof,
	lx_lcbracket, lx_rcbracket,
};
int operators = 22;


#endif




