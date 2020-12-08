/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Fabo Matus	xfabom01
**/

#ifndef CODE_GEN_H
#define CODE_GEN_H

#include "d_tree.h"
#include "sym_list.h"

/*
 * Generates 'ifjcode20' code
 */
void code_gen(d_node* root, FILE* file_descriptor, sym_list* sl);


#endif // CODE_GEN_H
