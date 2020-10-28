#include "lexical_analyzer.h"
#include <assert.h>
#include <ctype.h>
#include <unistd.h>


unsigned WORD_COUNT=0;

void Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(1); 
}
void Word_count(FILE* go_file){
	if(go_file == NULL)Error("missing file");
	WORD_COUNT = 0;
	int chr=0;
	do{
		do{
			while(isspace(chr = fgetc(go_file)) && (chr != EOF && !(isOperator(chr))))fprintf(stdout,"%c",chr);//White space is not WORD
			if(chr == EOF || isOperator(chr))break;//may indicate comment
			fprintf(stdout,"%c",chr);
			WORD_COUNT++;               
			while(!(isspace(chr = fgetc(go_file))) && (chr != EOF && !(isOperator(chr))))fprintf(stdout,"%c",chr);//Reading WORD
			if(chr != EOF && !(isOperator(chr)))fprintf(stdout,"%c",chr); 
		}while(chr!=EOF && !(isOperator(chr)));

		if(chr=='/'){
			fprintf(stdout,"%c",chr);
			chr=fgetc(go_file);
			if(chr=='/')// checking for single line comment
							do{
								fprintf(stdout,"%c",chr);
								chr = fgetc(go_file);
							}while(chr != EOF && chr !='\n');

			if(chr=='*')// checking for multi line comment
							do{	
								do{
									fprintf(stdout,"%c",chr);
									chr = fgetc(go_file);
								}while(chr != EOF && chr !='*');
									if(chr != EOF)fprintf(stdout,"%c",chr);
									if(chr != EOF)chr = fgetc(go_file);
							}while(chr!=EOF && chr!='/'); //checking for end of comment

						if(WORD_COUNT==0)WORD_COUNT++;//empty file
		}
		if(chr!='/' && chr!=EOF){
			int twinkle_little_star=chr;
			while(isOperator(chr)&&isOperator(twinkle_little_star)){
			 		twinkle_little_star=chr;
					 fprintf(stdout,"%c",chr);
				 	 chr=fgetc(go_file);
				}
				WORD_COUNT++;
				if(!isOperator(chr))ungetc(chr,go_file);
		}
	}while(chr!=EOF);
	return;
}


void Prints_lex(lex_unit_t* First,unsigned number_of_units){
	if(First == NULL) return;
	lex_unit_t* tmp = First;
	unsigned counter=0;
	fprintf(stdout,"~~~~~~~~\n");
	fprintf(stdout,"Lexemes:\n");
	fprintf(stdout,"-+-------------------\n");
	while(tmp!=NULL){
		counter++;
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
	assert(counter==number_of_units);
}

FILE* Creating_file(const char *filename,const char *text){

	if(filename==NULL)Error("Could not open file");
	if(text==NULL)Error("Could not open file");
	//only accepts .go files
	if(strcmp(strrchr(filename,'.'),".go")!=0)Error("Wrong file");
	//trying to open file w+
	FILE* go_file=fopen(filename,"w+");


	if(go_file==NULL)Error("Could not open file"); 
	if(fputs(text,go_file)<0)Error("Could not write to file"); 

    rewind(go_file);//to reset the pointer to the start of the file
	 
	fprintf(stdout,"text in file:\n");
    Word_count(go_file);
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
	if(act == first && act->data_size == 0)first = NULL;
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
	FILE * go_file=Creating_file("test01.go","casa casca + 44 8454343 *** a=4*2");
	lex_unit_t* lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);


	//------------//
	/* TEST02_ID  */
	/* testing ID */	
	//------------//

	fprintf(stdout,"\n======TEST02_ID======\n");
	go_file=Creating_file("t.go","_id   ___420var\n ____ int return 420blazeitvar");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

	//------------//
	/* TEST03_ID  */
	/* testing ID */	
	//------------//

	fprintf(stdout,"\n======TEST03_ID======\n");
	go_file=Creating_file("t.go","GGG4646 VVV SSS AAA CCC VV888BB88 VVSSCC@@@@@@ ");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first,WORD_COUNT);
	Free_Lex_Units(lex_first);
	fclose(go_file);

		return 0;
}
