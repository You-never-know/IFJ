#include "structs.h" 
#ifndef D_TREE
#define D_TREE

typedef struct d_tree
{
	int type; 
	lex_unit_t * data; //data of derivation tree

	struct d_tree * left;  //left node
	struct d_tree * right; //right node
}d_node;

//---------------------------------------------------------------------------- prototypes
void d_tree_init(d_node * root); // inits d_tree
void d_node_insert(d_node * node,lex_unit_t * left,lex_unit_t * right,int left_type,int right_type); //inserts left and right node


#endif