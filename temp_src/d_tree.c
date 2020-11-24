#include "d_tree.h"

d_node * d_tree_init(d_node * root){

	root=NULL;
	return root;
}

d_node * d_root_insert(d_node * root,lex_unit_t * r_data,int r_type){
	
	if(root!=NULL){ //checks if node is root
		fprintf(stderr,"node is not root\n" );
		return root;
	}

	root=malloc(sizeof(d_node));

	if(root==NULL){
		fprintf(stderr,"malloc failed\n" );
		return root;
	}

	root->left=NULL;
	root->right=NULL;
	
	root->type=r_type;
	root->data=r_data;

	return root;

}
d_node * d_node_insert_left(d_node * node,lex_unit_t * left_data,int left_type){

	if(node==NULL){
	 	fprintf(stderr,"node is not allocated\n");
		return node;
	 }

	 if(node->left != NULL){
	 	fprintf(stderr,"node left is allocated\n");
		return node;
	 }
	 else{
	 	node->left=malloc(sizeof(d_node));
	 	
	 	if(node->left==NULL){
			fprintf(stderr,"malloc failed\n");
			return node;
		}
	 		
	 	node->left->type=left_type;
	 	node->left->data=left_data;

	 	node->left->left=NULL;
	 	node->left->right=NULL;

	 	
	}

	return node;

}
d_node * d_node_insert_right(d_node * node,lex_unit_t * right_data,int right_type){

	if(node==NULL){
	 	fprintf(stderr,"node is not allocated\n");
		return node;
	 }

	 if(node->right != NULL){
	 	fprintf(stderr,"node right is allocated\n");
		return node;
	 }
	 else{
	 	node->right=malloc(sizeof(d_node));
	 	
	 	if(node->right==NULL){
			fprintf(stderr,"malloc failed\n");
			return node;
		}
	 		
	 	node->right->type=right_type;
	 	node->right->data=right_data;

	 	node->right->right=NULL;
	 	node->right->left=NULL;


	}
	return node;
}
