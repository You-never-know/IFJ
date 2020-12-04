/**
* Project:
* Implementace pøekladaèe imperativního jazyka IFJ20
*
* Authors:
* Daniel Marek 					xmarek72
* Kateřina Neprašová			xnepra01
**/

#include "structs.h"
#include "d_tree.h"
#include "prec_parser.h"
#include "prec_table.h"
#include "create_tables.h"
#include "lex_list.h"
#include "sym_list.h"
#include "symtable.h"
#include "lexical_analyzer.h"
#include <stdbool.h>

/*
* Auxiliary function for testing - prints "ERR" when expecting true but got false
*/
void expect_true(bool var) {
	if (var != true) {
		printf("ERR");
	}
}

/*
* Auxiliary function for testing - prints "ERR" when expecting false but got true
*/
void expect_false(bool var) {
	if (var == true) {
		printf("ERR");
	}
}

/*
* Functions for printing the tree, skeleton based on IAL tests
*/
//IAL INSPIRED !! ---------------------------------------------
void print_tree_in(d_node* root, char* suf, char direction)
{

	if (root != NULL)
	{

		char* suf2 = (char*)malloc(strlen(suf) + 4);
		strcpy(suf2, suf);
		if (direction == 'L')
		{
			suf2 = strcat(suf2, "  |");

		}
		else {
			suf2 = strcat(suf2, "   ");
		}
		print_tree_in(root->right, suf2, 'R');

		if (root->data != NULL) {

			if (root->data->unit_type == 4) {
				printf("%s  +-[%d; type %d]\n", suf, *((int*)root->data->data), root->data->unit_type);
			}
			else {
				printf("%s  +-[%s; type %d]\n", suf, (char*)root->data->data, root->data->unit_type);
			}

		}

		strcpy(suf2, suf);

		if (direction == 'R')
			suf2 = strcat(suf2, "  |");
		else
			suf2 = strcat(suf2, "   ");

		print_tree_in(root->left, suf2, 'L');

		if (direction == 'R') printf("%s\n", suf2);
		free(suf2);
	}
}

void print_tree(d_node* root)
{
	printf("PRINT TREE:\n");
	printf("TYPES: 0 ERROR; 1 OPERATOR; 2 IDENTIFICATOR; 3 KEYWORD; 4 INTEGER; 5 DECIMAL; 6 STRING\n");
	printf("\n");
	if (root != NULL)
		print_tree_in(root, "", 'X');
	else
		printf("EMPTY\n");
	printf("\n");
	printf("=================================================\n");
}

//------------------------------------------------------------

/*
* Testing
*/
int main(int argc, char* argv[]) {
	if (argc > 1) {
		FILE* file = fopen(argv[1], "r"); // open file for reading

		if (file == NULL) {
			return 1;
		}
		token_list* Active_token = NULL; //fix later
		sym_tab* function_table = htab_create(MEDIUM_TABLE);
		//sym_tab* function_table = NULL;
		//int ret=0;
		//sym_list* sl = create_tables(file, &ret, &function_table);
		lex_unit_t* lu = LexUnitCreate();
		LexUnitCtor(lu);
		d_node* root = NULL;
		root = d_node_create(NULL, NULL, DOLLAR);
		lex_unit_t* analyze = Analyze(file, lu);
		bool var = true;
		while ((analyze != NULL) || (var != false)) {
			//expect_true(Parse_expresion(&analyze, root , file, fun_tab));
			
			var = Parse_expresion(&analyze, root, &Active_token, function_table);
			if ((analyze != NULL) && (var != false)) {
				print_tree(root);
			}
			analyze = Analyze(file, lu);
			printf("EX===============================================\n");

		}
		LexUnitDelete(lu);
		fclose(file);  // close the file
	}


	return 0;

}
