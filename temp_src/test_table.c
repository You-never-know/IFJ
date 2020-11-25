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

			
			fprintf(stdout, "%s: ", (char*)tmp->name->data);
			
			if (! (tmp -> is_function)) {
				printf("ID: ");

				switch (tmp->data->unit_type) {
					case INTEGER: printf("%d || ", *(int *)(tmp->data->data)); break;
					case DECIMAL: printf("%f || ", *(double *)(tmp->data->data)); break;
					case STRING : printf("%s || ", (char *)tmp->data->data); break;
				}
			}

			else {
				printf("FUNCTION: ");
				printf("Parameters: ");
				
				for (Par *tmp1 = tmp->parameters; tmp1 != NULL; tmp1=tmp1->next) {
					printf("%s ", (char*)tmp1->name->data);
					switch (tmp1->type) {
						case INTEGER: printf("INT "); break;
						case DECIMAL: printf("DECIMAL "); break;
						case STRING : printf("STRING "); break;
					}
					printf("|| ");
				}
				
				printf("Return types: ");
				
				for (Ret *tmp2 = tmp->return_val; tmp2 != NULL; tmp2=tmp2->next) {
					switch (tmp2->type) {
						case INTEGER: printf("INT "); break;
						case DECIMAL: printf("DECIMAL "); break;
						case STRING : printf("STRING "); break;
					}
					printf("|| ");
				}
				printf("\n"); 
			}
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
		sym_tab* function_table;
		sym_list* sl = create_tables(file, &ret, &function_table);
		printf("-----------------------------------------------\n");
		printf("RET INT %d /n", ret);
		printf("FUNCTION_TABlE PRINT\n");
		//print_table(function_table);
		printf("-----------------------------------------------\n");
		printf("SYM_LIST PRINT\n");

		sl_elem_ptr tmp = sl->first;

		while (tmp != NULL) {
			print_table(tmp->st_data);
			tmp = tmp->r;
		}

		printf("-----------------------------------------------\n");

		fclose(file);  // close the file 
	}


	return 0;

}

