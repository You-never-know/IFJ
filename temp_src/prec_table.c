/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#include "prec_table.h"
#include "symtable.h"
#include <stdio.h>
#include <string.h>

//DEBUG VERSION

/*
* Function groups cases where the return value is the same
* Returns enum case according to input
* Param lex_unit_t* lex: 
* Param sym_tab* st: Table in which we search for function identificator
*/
enum tokens merge_event(lex_unit_t* lex, sym_tab* st) {
	//printf("MERGE =================================================\n");
	switch (lex->unit_type) {

	case OPERATOR:
		//printf("OPERATOR:\n");
		//printf("P_TOKEN SIZE %ld\n",lex->data_size );
		if (lex->data_size == 1) {

			if (strcmp(lex->data, "$") == 0){
				//printf("DOLLAR, %s\n", (char*)lex->data);
				return DOLLAR;}

			else if (strcmp(lex->data, "(") == 0){
				//printf("L_BRACKET, %s\n", (char*)lex->data);
				return L_BRACKET;}

			else if (strcmp(lex->data, ")") == 0){
				//printf("R_BRACKET, %s\n", (char*)lex->data);
				return R_BRACKET;}

			else if ((strcmp(lex->data, "+") == 0) || (strcmp(lex->data, "-") == 0)){
				//printf("PLUS_MINUS, %s\n", (char*)lex->data);
				return PLUS_MINUS;}

			else if ((strcmp(lex->data, "*") == 0) || (strcmp(lex->data, "/") == 0)){
				//printf("MUL_DIV, %s\n", (char*)lex->data);
				return MUL_DIV;}

			else if ((strcmp(lex->data, "<") == 0) || (strcmp(lex->data, ">") == 0)){
				//printf("COMPARISON, %s\n", (char*)lex->data);
				return COMPARISON;}

			else if (strcmp(lex->data, ",") == 0){
				//printf("COMMA, %s\n", (char*)lex->data);
				return COMMA;}
		}
		else if (lex->data_size == 2) {
			//printf("COMPARISON, %s\n", (char*)lex->data);
			return COMPARISON;
		}

		break;

	case IDENTIFICATOR:
		//printf("IDENTIFICATOR\n");
		if(find_item(st, lex)==NULL)  
			return I; 
		else 
			return F;
		
	case INTEGER: case DECIMAL: case STRING:
	//printf("INTEGER/DECIMAL/STRING\n");
		return I;

	default:
	//printf("default\n");
		return ERR;
	}
	return ERR;
}

/*
* Returns control based on precedent table
* Explanations:
*  < = >  ERR
* -1 0 1  42
*/
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
