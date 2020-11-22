/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#ifndef PREC_TABLE
#define PREC_TABLE

enum tokens {
	DOLLAR,
	L_BRACKET,
	R_BRACKET,
	I,
	PLUS_MINUS,
	MUL_DIV,
	COMPARISON,
	F,
	COMMA,
};

enum lex_units {
	ERROR, OPERATOR, IDENTIFICATOR, KEYWORD,
	INTEGER, DECIMAL, STRING, NEWLINE,
	OPERATOR_ERR, ID_ERR, INT_ERR, DEC_ERR, STR_ERR
};

struct lex_unit {
	int unit_type;
	void* data;
	size_t data_size;
	struct lex_unit* next;
	struct htable_item* table;
};
typedef struct lex_unit lex_unit_t;

int prec_event(int, int);
#endif