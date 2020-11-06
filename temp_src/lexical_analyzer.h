
/**
* Project:
* Implementace p�eklada�e imperativn�ho jazyka IFJ20
*
* Author:
* Fabo Mat��			xfabom01
**/

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"

#define CHUNK_SIZE 128 // Defines how much space is allocated when previous buffer overflows

bool isWhiteSpace(const char c);
bool isNumber(const char c);
bool isLetter(const char c);
bool isOperator(const char c);
bool isMultiOperator(const char c);
bool isKeyword(const lex_unit_t* lex);
/*
 * Analyzes given file and returns first found lexical unit
 * or returns 'NULL' pointer if memory allocation failed
 */
lex_unit_t* Analyze(FILE* file_descriptor, lex_unit_t* unit);

#endif // LEXICAL_ANALYZER_H
