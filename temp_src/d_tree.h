/**
* Project:
* Implementace prekladace imperativniho jazyka IFJ20
*
* Author:
* Drengubiak Vladimír	xdreng01
**/

#include "structs.h" 
#ifndef D_TREE
#define D_TREE

typedef struct d_tree
{
	int type; 
	lex_unit_t * data; //data of derivation tree OP or ID

	struct d_tree * left;  //left node
	struct d_tree * right; //right node
}d_node;

//---------------------------------------------------------------------------- prototypes
d_node * d_tree_init(d_node * root); // inits d_tree
d_node * d_node_create(d_node * node,lex_unit_t * r_data,int r_type); //creates node
d_node * d_node_insert_left(d_node * node,d_node * left_node); //insert left son 
d_node * d_node_insert_right(d_node * node,d_node * right_node); //insert right son
d_node * next_left(d_node* node); // gives you next left ptr
void Print_tree(d_node* TempTree); // prints tree
void delete_tree(d_node * root); //deletes tree;



#endif