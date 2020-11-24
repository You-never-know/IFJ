#include "d_tree.h"

d_node * d_tree_init(d_node * root){

	root=NULL;
	return root;
}

d_node * d_node_create(d_node * node,lex_unit_t * r_data,int r_type){
	
	if(node!=NULL){ //checks if node is root
		fprintf(stderr,"node is already allocated\n" );
		return node;
	}

	node=malloc(sizeof(d_node));

	if(node==NULL){
		fprintf(stderr,"malloc failed\n" );
		return node;
	}

	node->left=NULL;
	node->right=NULL;
	
	node->type=r_type;
	node->data=r_data;

	return node;

}
d_node * d_node_insert_left(d_node * node,d_node * left_node){

	if(node==NULL){
	 	fprintf(stderr,"node is not allocated\n");
		return node;
	 }

	 if(node->left != NULL){
	 	fprintf(stderr,"node left is allocated\n");
		return node;
	 }
	 else{
	 	node->left=left_node;
	}

	return node;

}
d_node * d_node_insert_right(d_node * node,d_node * right_node){

	if(node==NULL){
	 	fprintf(stderr,"node is not allocated\n");
		return node;
	 }

	 if(node->right != NULL){
	 	fprintf(stderr,"node right is allocated\n");
		return node;
	 }
	 else{
	 	node->right=right_node;
	}
	return node;
}

void delete_tree(d_node * root){

	if(root==NULL)return;
	delete_tree(root->left);
	delete_tree(root->right);
	free(root);
	root=NULL;
	
}
