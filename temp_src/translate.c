#include "create_tables.h"
#include "prec_parser.h"
#include "d_tree.h"
#include "structs.h"
#include "symtable.h"
#include "lexical_analyzer.h"
#include <stdio.h>
#include <stdio.h>
#include <string.h>

int main() {

	int ret = 0;
	FILE * f = stdin;
	sym_tab * table = NULL;

	sym_list * sl = create_tables(f, &ret, &table);

	if (ret != 0) {
		sl_dissolve(sl);
		clean_function_table(table);
		free_table(table);
		return ret;
	}

	if(fseek(stdin, 0L, SEEK_SET) != 0) {
		sl_dissolve(sl);
		clean_function_table(table);
		free_table(table);
		return ret;
	}

	lex_unit_t * tmp = LexUnitCreate();
	lex_unit_t * tmp2 = tmp;
	LexUnitCtor(tmp);
	tmp = Analyze(f, tmp);

	while (tmp != NULL) {

		/*if (tmp->unit_type == IDENTIFICATOR) {

			d_node * node = d_node_create(NULL, NULL, 0);
			bool try = Parse_expresion(&tmp, node, f, table);
			if (try == true) {
				delete_tree(node);
			}
			else {
				if (tmp->unit_type == OPERATOR && ((tmp->data_size == 2 && memcmp(tmp->data, ":=" , 2ul) == 0 )|| (tmp->data_size == 1 && memcmp(tmp->data, "=" , 1ul) == 0))) {

					tmp = LexUnitCreate();
					tmp2 = tmp;
					LexUnitCtor(tmp);
					tmp = Analyze(f, tmp);
					d_node * node = d_node_create(NULL, NULL, 0);
					if (Parse_expresion(&tmp, node, f, table) == false) {
						sl_dissolve(sl);
						clean_function_table(table);
						free_table(table);
						return 2;
					}
					delete_tree(node);
				}
			}
		}*/
		if ((tmp->unit_type >= INTEGER && tmp->unit_type <= STRING) || (tmp->unit_type == OPERATOR && (tmp->data_size == 1 && memcmp(tmp->data, "(" , 1ul) == 0 ))) {
			d_node * node = d_node_create(NULL, NULL, 0);
			if (Parse_expresion(&tmp, node, f, table) == false) {
				sl_dissolve(sl);
				clean_function_table(table);
				free_table(table);
				return 2;
			}
			delete_tree(node);
		}
		else {
			LexUnitDelete(tmp2);
		}

		tmp = LexUnitCreate();
		tmp2 = tmp;
		LexUnitCtor(tmp);
		tmp = Analyze(f, tmp);

	}
	LexUnitDelete(tmp2);

	sl_dissolve(sl);
	clean_function_table(table);
	free_table(table);
	return ret;
}

