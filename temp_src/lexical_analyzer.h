#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "structs.h"

#define CHUNK_SIZE 128 // Defines how much space is allocated when previous buffer overflows

/*
 * Analyzes given file and returns first found lexical unit
 * or returns 'NULL' pointer if memory allocation failed
 */
lex_unit_t* Analyze(FILE* file_descriptor);

#endif // LEXICAL_ANALYZER_H