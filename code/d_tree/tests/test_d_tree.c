#include "d_tree.h"

int main(){

	d_node * root = NULL; 
	d_tree_init(root);
 	root = d_root_insert(root,NULL,0);
 	d_node_insert_left(root,NULL,1);
 	d_node_insert_right(root,NULL,2);
 	printf("%d root \n",root->type);
 	printf("%d root left \n",root->left->type);
 	printf("%d root right \n",root->right->type);
 	

}