/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#include "prec_table.h"
#include <stdio.h>
#include <string.h>


enum tokens merge_event(lex_unit_t* lex) {

	switch (lex->unit_type) {

	case OPERATOR:

		if (lex->data_size = 1) {

			if (strcmp(lex->data, "$") = 0)
				return DOLLAR;

			else if (strcmp(lex->data, "(") = 0)
				return L_BRACKET;

			else if (strcmp(lex->data, ")") = 0)
				return R_BRACKET;

			else if (strcmp(lex->data, "+") = 0) || (strcmp(lex->data, "-") = 0)
				return PLUS_MINUS;

			else if (strcmp(lex->data, "*") = 0) || (strcmp(lex->data, "/") = 0)
				return MUL_DIV;

			else if (strcmp(lex->data, "<") = 0) || (strcmp(lex->data, ">") = 0)
				return COMPARISON;

			else if (strcmp(lex->data, ",") = 0)
				return COMMA;
		}
		else if (lex->data_size = 2) {
			return COMPARISON;
		}

	case IDENTIFICATOR: //fix later
		return I;

	case OPERATOR or INTEGER or DECIMAL or STRING:
		return I;

	default:
		return NULL;
	}
}

int prec_event(int top, int input) {

	switch (top) {

	case DOLLAR:
		switch (input) {
		case DOLLAR:
			return 42;
		case L_BRACKET:
			return -1;
		case R_BRACKET:
			return 42;
		case I:
			return -1;
		case PLUS_MINUS:
			return -1;
		case MUL_DIV:
			return -1;
		case COMPARISON:
			return -1;
		case F:
			return -1;
		case COMMA:
			return 42;
		default:
			return 42;
		}
	case L_BRACKET:
		switch (input) {
		case DOLLAR:
			return 42;
		case L_BRACKET:
			return -1;
		case R_BRACKET:
			return 0;
		case I:
			return -1;
		case PLUS_MINUS:
			return -1;
		case MUL_DIV:
			return -1;
		case COMPARISON:
			return -1;
		case F:
			return -1;
		case COMMA:
			return 0;
		default:
			return 42;
		}
	case R_BRACKET:
		switch (input) {
		case DOLLAR:
			return 1;
		case L_BRACKET:
			return 42;
		case R_BRACKET:
			return 1;
		case I:
			return 42;
		case PLUS_MINUS:
			return 1;
		case MUL_DIV:
			return 1;
		case COMPARISON:
			return 1;
		case F:
			return 42;
		case COMMA:
			return 42;
		default:
			return 42;
		}
	case I:
		switch (input) {
		case DOLLAR:
			return 1;
		case L_BRACKET:
			return 42;
		case R_BRACKET:
			return 1;
		case I:
			return 42;
		case PLUS_MINUS:
			return 1;
		case MUL_DIV:
			return 1;
		case COMPARISON:
			return 1;
		case F:
			return 42;
		case COMMA:
			return 1;
		default:
			return 42;
		}
	case PLUS_MINUS:
		switch (input) {
		case DOLLAR:
			return 1;
		case L_BRACKET:
			return -1;
		case R_BRACKET:
			return 1;
		case I:
			return -1;
		case PLUS_MINUS:
			return 1;
		case MUL_DIV:
			return -1;
		case COMPARISON:
			return 1;
		case F:
			return -1;
		case COMMA:
			return 1;
		default:
			return 42;
		}
	case MUL_DIV:
		switch (input) {
		case DOLLAR:
			return 1;
		case L_BRACKET:
			return -1;
		case R_BRACKET:
			return 1;
		case I:
			return -1;
		case PLUS_MINUS:
			return 1;
		case MUL_DIV:
			return 1;
		case COMPARISON:
			return 1;
		case F:
			return -1;
		case COMMA:
			return 1;
		default:
			return 42;
		}
	case COMPARISON:
		switch (input) {
		case DOLLAR:
			return 1;
		case L_BRACKET:
			return -1;
		case R_BRACKET:
			return 1;
		case I:
			return -1;
		case PLUS_MINUS:
			return -1;
		case MUL_DIV:
			return -1;
		case COMPARISON:
			return 1;
		case F:
			return -1;
		case COMMA:
			return 1;
		default:
			return 42;
		}
	case F:
		switch (input) {
		case DOLLAR:
			return 42;
		case L_BRACKET:
			return 0;
		case R_BRACKET:
			return 42;
		case I:
			return 42;
		case PLUS_MINUS:
			return 42;
		case MUL_DIV:
			return 42;
		case COMPARISON:
			return 42;
		case F:
			return -1;
		case COMMA:
			return 42;
		default:
			return 42;
		}
	case COMMA:
		switch (input) {
		case DOLLAR:
			return 42;
		case L_BRACKET:
			return -1;
		case R_BRACKET:
			return 0;
		case I:
			return -1;
		case PLUS_MINUS:
			return -1;
		case MUL_DIV:
			return -1;
		case COMPARISON:
			return -1;
		case F:
			return -1;
		case COMMA:
			return 0;
		default:
			return 42;
		}
	default:
		return 42;
	}
}