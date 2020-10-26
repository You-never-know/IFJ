#include "lexical_analyzer.h"

void Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(1); 
}

void Prints_lex(lex_unit_t* First){
	if(First == NULL) return;
	lex_unit_t* tmp = First;
	fprintf(stdout,"~~~~~~~~\n");
	fprintf(stdout,"Lexemes:\n");
	fprintf(stdout,"-+-------------------\n");
	while(tmp!=NULL){
		fprintf(stdout," | unit_type: ");
		switch(tmp->unit_type){
			case ERROR:			fprintf(stdout, "Error\n"); break;
			case OPERATOR:		fprintf(stdout, "Operator\n"); break;
			case IDENTIFICATOR:	fprintf(stdout, "Identificator\n"); break;
			case KEYWORD:		fprintf(stdout, "Keyword\n"); break;
			case INTEGER:		fprintf(stdout, "Integer\n"); break;
			case DECIMAL:		fprintf(stdout, "Decimal\n"); break;
			case STRING:		fprintf(stdout, "String\n"); break;
			case OPERATOR_ERR:	fprintf(stdout, "Operator Error\n"); break;
			case ID_ERR:		fprintf(stdout, "Identificator Error\n"); break;
			case INT_ERR:		fprintf(stdout, "Integer Error\n"); break;
			case DEC_ERR:		fprintf(stdout, "Decimal Error\n"); break;
			case STR_ERR:		fprintf(stdout, "String Error\n"); break;
			default:			fprintf(stdout, "%d\n", tmp->unit_type); break;
		}
		fprintf(stdout," | data_size: %ld\n",tmp->data_size);
		fprintf(stdout," | data: ");
		switch(tmp->unit_type){
			case INTEGER:		fprintf(stdout, "%ld\n", *((size_t*)tmp->data)); break;
			case DECIMAL:		fprintf(stdout, "%f\n", *((double*)tmp->data)); break;
			case STRING:		fprintf(stdout, "\"%s\"\n", (char*)tmp->data); break;
			default:			fprintf(stdout, "%s\n", (char*)tmp->data); break;
		}
		tmp=tmp->next;
		fprintf(stdout," +-------------------\n");
	}
}

FILE* Creating_file(const char *filename,const char *text){

	//only accepts .go files
	if(strcmp(strrchr(filename,'.'),".go")!=0)Error("Wrong file");
	//trying to open file w+
	FILE* go_file=fopen(filename,"w+");

	if(go_file==NULL)Error("Could not open file"); 
	if(fputs(text,go_file)<0)Error("Could not write to file"); 

    rewind(go_file);//to reset the pointer to the start of the file
	
	int chr = fgetc(go_file); 
	 fprintf(stdout,"text in file:\n");
    while (chr != EOF) //prints text in file
    {  	
        fprintf(stdout,"%c",chr); 
        chr = fgetc(go_file); 
    } 
    fprintf(stdout,"\n");

    rewind(go_file);

	return go_file;
}

lex_unit_t* Loading_lex_units(FILE * go_file){

	if(go_file==NULL)Error("file gone wild");

	lex_unit_t *act = malloc(sizeof(lex_unit_t));
	if(act==NULL) Error("Lexical unit allocation failed");
	LexUnitCtor(act);
	lex_unit_t *first=act; // first ptr of lex_units
	lex_unit_t* last_act = NULL;
	while(Analyze(go_file, act) != NULL){ // loading units
		act->next = malloc(sizeof(lex_unit_t));
		if(act->next == NULL) Error("Lexical unit allocation failed");
		LexUnitCtor(act->next);
		last_act = act;
		act=act->next;
	}
	LexUnitDelete(act);
	if(last_act != NULL) last_act->next = NULL;
	return first;
}

void Free_Lex_Units(lex_unit_t* first){
	/// Check function argument
	if(first == NULL) return;

	/// Loop through & free each unit in 'lex_unit_t' linked list
	lex_unit_t* tmp;
	lex_unit_t* tmp_next;
	for(tmp = first; tmp != NULL; tmp = tmp_next){
		tmp_next = tmp->next;
		LexUnitDelete(tmp);
	}
}

int main()
{

	//------------//
	/* TEST01_ID  */
	/* testing ID */	
	//------------//
	fprintf(stdout,"\n======TEST01_ID======\n");
	FILE * go_file=Creating_file("test01.go","ahoj c88888c AUTO");
	lex_unit_t* lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------//
	/* TEST02_ID  */
	/* testing ID */	
	//------------//
	fprintf(stdout,"\n======TEST02_ID======\n");
	go_file=Creating_file("t.go","_id   ___420var\n ____ Return  return 420blazeitvar");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first);
	Free_Lex_Units(lex_first);
	fclose(go_file);
		return 0;
}
