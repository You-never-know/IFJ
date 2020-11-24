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
d_node * d_tree_init(d_node * root); // inits d_tree
d_node * d_node_create(d_node * node,lex_unit_t * r_data,int r_type); //creates node
d_node * d_node_insert_left(d_node * node); //insert left son 
d_node * d_node_insert_right(d_node * node); //insert right son



#endif