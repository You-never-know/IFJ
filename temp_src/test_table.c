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


int main() {

	sym_tab * test1 = htab_create(11);

	assert(test1 != NULL);
	assert(test1->size == 0);
	assert(test1->arr_size == 11);

	char * str1 = "tru";
	char * str2 = "fal";
	char * str3 = "tru";

	int i = 54;
	int j = 85.6;

	lex_unit_t a,b,c,d,e,f;
	a.unit_type = IDENTIFICATOR;
	a.data = str1;
	d.unit_type = IDENTIFICATOR;
	d.data = str2;
	b.unit_type = INTEGER;
	b.data = &i;
	c.unit_type = DECIMAL;
	c.data = &j;
	e.unit_type = IDENTIFICATOR;
	e.data = str3;
	f.unit_type = DECIMAL;
	f.data = &i;

	ht_item * t1 = add_item(test1, &a, false);
	add_data(t1, &b);

	ht_item * t2 = add_item(test1, &d, false);
	add_data(t2, &c);

	t1 = add_item(test1, &a, false);
	if (t1 != NULL) add_data(t1, &b);

	ht_item * t5 = add_item(test1, &e, false);
	add_data(t5, &f);

	ht_item * t3 = find_item(test1, &a);
	assert(t3 != NULL);

	ht_item * t4 = find_item(test1, &b);
	assert(t4 == NULL);


	// test funtions
	lex_unit_t a1,d1;
	a1.unit_type = IDENTIFICATOR;
	a1.data = str1;
	d1.unit_type = IDENTIFICATOR;
	d1.data = str2;

	t1 = add_item(test1, &a1, true);
	Par *p1,*p2,*p3,*p4;
	Ret *r1,*r2,*r3;

	p1 = malloc_param(t1);
	p2 = malloc_param(t1);
	p3 = malloc_param(t1);
	p4 = malloc_param(t1);
	
	add_param_type(p1, STRING);
	add_param_type(p2, DECIMAL);
	add_param_type(p3, INTEGER);
	add_param_type(p4, STRING);

	r1 = malloc_ret_val(t1);
	r2 = malloc_ret_val(t1);
	r3 = malloc_ret_val(t1);

	add_ret_type(r1, DECIMAL);
	add_ret_type(r2, INTEGER);
	add_ret_type(r3, STRING);

	t2 = add_item(test1, &d1, true);
	p1 = malloc_param(t2);
	p2 = malloc_param(t2);
	p3 = malloc_param(t2);
	p4 = malloc_param(t2);
	
	add_param_type(p1, INTEGER);
	add_param_type(p2, DECIMAL);
	add_param_type(p3, INTEGER);
	add_param_type(p4, STRING);

	r1 = malloc_ret_val(t2);
	r2 = malloc_ret_val(t2);
	r3 = malloc_ret_val(t2);

	add_ret_type(r1, STRING);
	add_ret_type(r2, DECIMAL);
	add_ret_type(r3, INTEGER);

	print_table(test1);
	clean_table(test1);
	assert(test1->size == 0);
	free_table(test1); 


	return 0;

}

