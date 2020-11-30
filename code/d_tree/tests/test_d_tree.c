#include "d_tree.h"
#define E 1
#define OP 2


int main(){

	d_node * root = NULL;
	d_tree_init(root);
 	root = d_node_create(root,NULL,0);
 	d_node * x[10];
 	lex_unit_t *s[10];

 	for(unsigned i=0;i<5;i++){
 		x[i]=NULL;
 		s[i]=malloc(sizeof(lex_unit_t));
 		if(i%2){
 		x[i]=d_node_create(x[i],s[i],E);
 		}
 		else{
 		x[i]=d_node_create(x[i],s[i],OP);
 		}
 	}
 	d_node_insert_right(root,x[0]);
 	d_node_insert_right(x[0],x[1]);
 	d_node_insert_left(x[0],x[2]);
 	d_node_insert_right(x[1],x[3]);
 	d_node_insert_left(x[1],x[4]);
 	Print_tree(root);
 	delete_tree(root);
 	

}