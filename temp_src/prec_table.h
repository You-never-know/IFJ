﻿/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#ifndef PREC_TABLE
#define PREC_TABLE

#include "structs.h"
#include "symtable.h"

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
	ERR,
	ASSIGNMENT,
	E,
	HANDLE
};

enum tokens merge_event(lex_unit_t*, sym_tab*);
int prec_event(int, int);
//  < = >  ERR
// -1 0 1  42
#endif
