#include "d_tree.h"

int main(){

	d_node * root = NULL,*lf=NULL,*rh=NULL; 
	d_tree_init(root);
 	root = d_node_create(root,NULL,0);
 	lf=d_node_create(lf,NULL,1);
 	rh=d_node_create(rh,NULL,2);


 	d_node_insert_left(root,lf);
 	d_node_insert_right(root,rh);
 	printf("%d root \n",root->type);
 	printf("%d root left \n",root->left->type);
 	printf("%d root right \n",root->right->type);
 	delete_tree(root);
 	

}