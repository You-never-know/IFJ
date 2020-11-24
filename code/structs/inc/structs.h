
/**
* Project:
* Implementace p?eklada? imperativn?o jazyka IFJ20
*
* Author:
* Fabo Mat??			xfabom01
**/
#ifndef STRUCTS_H
#define STRUCTS_H


#include <stdio.h>
#include <stdlib.h>
#include "symtable.h"
	
/*
 * Global constants to define lexical unit data types
 */
enum lex_units {ERROR, OPERATOR, IDENTIFICATOR, KEYWORD,
				INTEGER, DECIMAL, STRING, NEWLINE,
				OPERATOR_ERR, ID_ERR, INT_ERR, DEC_ERR, STR_ERR};
/*
 * Structure to hold a single lexical unit
 * 'unit_type' holds which lexical unit it is, as is defined in 'lex_units'
 * 'data' points to data extracted from source code
 * 'data_size' holds size of allocated space for 'data'
 * 'next' pointer to the next lex_unit, linked list
 * 'table' points to the location in the symtable where it is stored
 */
struct lex_unit{
	int unit_type;
	void* data;
	size_t data_size;
	struct htable_item* table;
};
typedef struct lex_unit lex_unit_t;

lex_unit_t* LexUnitCreate();
void LexUnitCtor(lex_unit_t* unit);
void LexUnitClear(lex_unit_t* unit);
void LexUnitDelete(lex_unit_t* unit);

#endif // STRUCTS_H