
/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Author:
* Fabo Matúš			xfabom01
**/

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"
#include "symtable.h"

#define CHUNK_SIZE 128 // Defines how much space is allocated when previous buffer overflows

typedef struct lex_u_list //linked list for test 
{
	lex_unit_t * unit;
	struct lex_u_list *next;
}lex_list;

void Error(const char *msg);
bool isWhiteSpace(const char c);
bool isNumber(const char c);
bool isLetter(const char c);
bool isOperator(const char c);
bool isMultiOperator(const char c);
bool isKeyword(const lex_unit_t* lex);
lex_list* Loading_lex_units(FILE * go_file);


/*
 * Analyzes given file and returns first found lexical unit
 * or returns 'NULL' pointer if memory allocation failed
 */
lex_unit_t* Analyze(FILE* file_descriptor, lex_unit_t* unit);

#endif // LEXICAL_ANALYZER_H
