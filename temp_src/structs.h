#ifndef STRUCTS_H
#define STRUCTS_H


#include <stdio.h>
#include <stdlib.h>
/*
 * Global constants to define lexical unit data types
 */
enum lex_units {ERROR, OPERATOR, IDENTIFICATOR,
				KEYWORD, INTEGER, DECIMAL, STRING,
				OPERATOR_ERR, ID_ERR, INT_ERR, DEC_ERR, STR_ERR};
/*
 * Structure to hold a single lexical unit
 * 'unit_type' holds which lexical unit it is, as is defined in 'lex_units'
 * 'data' points to data extracted from source code
 * 'data_size' holds size of allocated space for 'data'
 * 'next' points to next lexical unit in a linked list structure
 */
struct lex_unit{
	int unit_type;
	void* data;
	size_t data_size;
	struct lex_unit* next;
};
typedef struct lex_unit lex_unit_t;

lex_unit_t* LexUnitCreate();
void LexUnitCtor(lex_unit_t* unit);
void LexUnitClear(lex_unit_t* unit);
void LexUnitDelete(lex_unit_t* unit);

#endif // STRUCTS_H