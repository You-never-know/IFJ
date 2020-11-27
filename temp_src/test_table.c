#include "symtable.h"
#include "structs.h"
#include "create_tables.h"
#include <stdio.h>
#include <assert.h>

void print_table(sym_tab * st) {

	for (size_t i = 0; i < st->arr_size; i++) {

		printf("%lu Riadok\n", i);
		printf("-----------------------------------------------\n");
		
		for (ht_item *tmp = st->ptr[i]; tmp != NULL; tmp=tmp->next) {

			if (tmp->func != NULL) {
				printf("FUNCTION: ");
				printf("%s ", (char *)tmp->func->func_name->data);
				printf("Parameters: ");
				
				for (Par *tmp1 = tmp->func->parameters; tmp1 != NULL; tmp1=tmp1->next) {
					printf("%s ", (char*)tmp1->name->data);
					switch (tmp1->type) {
						case INTEGER: printf("Int "); break;
						case DECIMAL: printf("Dec "); break;
						case STRING : printf("Str "); break;
					}
					printf("|| ");
				}
				
				printf("Return types: ");
				
				for (Ret *tmp2 = tmp->func->return_val; tmp2 != NULL; tmp2=tmp2->next) {
					switch (tmp2->type) {
						case INTEGER: printf("Int "); break;
						case DECIMAL: printf("Dec "); break;
						case STRING : printf("Str "); break;
					}
					printf("|| ");
				}

			}
			else {
				
				printf("ID: ");
				printf("%s ", (char*)tmp->id->id_name->data);

				switch (tmp->id->type) {
					case INTEGER: printf("Int"); break;
					case DECIMAL: printf("Dec"); break;
					case STRING : printf("Str"); break;
				}
			} 
			printf("\n"); 
		}
		printf("-----------------------------------------------\n");
	}
}


int main(int argc, char* argv[]) {

	if (argc > 1) {
		FILE* file = fopen(argv[1], "r"); // open file for reading 

		if (file == NULL) {
			return 1; 
		}
		int ret = 0;
		sym_tab* function_table = NULL;
		sym_list* sl = create_tables(file, &ret, &function_table);
		printf("-----------------------------------------------\n");
		printf("RET INT %d /n", ret);
		printf("FUNCTION_TABlE PRINT\n");
		print_table(function_table);
		printf("-----------------------------------------------\n");
		printf("SYM_LIST PRINT\n");

		

		if (sl != NULL) {
			sl_elem_ptr tmp = sl->first;

			while (tmp != NULL) {
				print_table(tmp->st_data);
				tmp = tmp->r;
			}
		}
		else {
			printf("NULL\n");
		}
		printf("-----------------------------------------------\n");

		clean_function_table(function_table);
		free_table(function_table);
		fclose(file);  // close the file
		if (sl != NULL) {
			sl_dissolve(sl);
		} 
	}


	return 0;

}

