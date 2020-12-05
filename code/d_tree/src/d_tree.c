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

d_node* next_left(d_node* node){
	if(node==NULL){
		fprintf(stderr,"root is NULL\n");
		return node;
	}
	return node->left;

}
void prints_side(unsigned space,unsigned offset,unsigned old_space){
	for(unsigned i = offset; i < space; i++){
   		if(old_space>i)
   			printf(" ");
   		if(old_space==i)
   			printf("|");
   		if(old_space<i)
   			printf("-");
   }
}
void Print_tree2(d_node* TempTree, char* sufix, char fromdir)
{
    if (TempTree != NULL)
     {
	char* suf2 = (char*) malloc(strlen(sufix) + 4);
	strcpy(suf2, sufix);
        if (fromdir == 'L')
	{
	   suf2 = strcat(suf2, "  |");
	   printf("%s\n", suf2);
	}
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->right, suf2, 'R');
        printf("%s  +-[,%d]\n", sufix, TempTree->type);
	strcpy(suf2, sufix);
        if (fromdir == 'R')
	   suf2 = strcat(suf2, "  |");
	else
	   suf2 = strcat(suf2, "   ");
	Print_tree2(TempTree->left, suf2, 'L');
	if (fromdir == 'R') printf("%s\n", suf2);
	free(suf2);
    }
}

void Print_tree(d_node* TempTree)
{
  printf("Struktura binarniho stromu:\n");
  printf("\n");
  if (TempTree != NULL)
     Print_tree2(TempTree, "", 'X');
  else
     printf("strom je prazdny\n");
  printf("\n");
  printf("=================================================\n");
}

void delete_tree(d_node * root){

	if(root==NULL)return;
	delete_tree(root->left);
	delete_tree(root->right);
	free(root);
	root=NULL;
	
}
