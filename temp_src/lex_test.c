#include "lexical_analyzer.h"

int Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(1); 
}
int Prints_lex(lex_unit_t*First){
	lex_unit_t * tmp=First;
	fprintf(stdout,"--------------------\n");
	fprintf(stdout,"lexemes:\n");
	while(tmp!=NULL){
		fprintf(stdout,"--------------------\n");
		if(tmp->unit_type==IDENTIFICATOR || tmp->unit_type==KEYWORD || //string formats 
		   tmp->unit_type==STRING){
			fprintf(stdout,"unit_type: %d \n",tmp->unit_type);
			fprintf(stdout,"data_size: %ld \n",tmp->data_size);
			char *arr=(char *)tmp->data;
			fprintf(stdout,"data:");
		for(int i=0;i<tmp->data_size;i++)fprintf(stdout,"%c",arr[i]);
			fprintf(stdout,"\n");	
		}

		if(tmp->unit_type==INTEGER || tmp->unit_type==DECIMAL){ //numbers 
			fprintf(stdout,"unit_type: %d \n",tmp->unit_type);
			if(tmp->unit_type==INTEGER)fprintf(stdout,"%d",*((int*)tmp->data));
			else fprintf(stdout,"%lf",*((float*)tmp->data));

		}
		tmp=tmp->next;

	}
	
}
FILE* Creating_file(FILE * go_file,const char *filename,const char *text){

	//only accepts .go files
	if(strcmp(strrchr(filename,'.'),".go")!=0)Error("Wrong file");
	//trying to open file w+
	 go_file=fopen(filename,"w+");

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

	lex_unit_t *act=malloc(sizeof(lex_unit_t));
	if(act==NULL)Error("malloc failed");
	lex_unit_t *first=act;// first ptr of lex_units
	while((act=Analyze(go_file,act))!=NULL){//loading units 
		act->next=malloc(sizeof(lex_unit_t));
		if(act->next==NULL)Error("malloc failed");
		act=act->next;
	}
	return first;
}

int main()
{

	//-------------/
	/* TEST01_ID  */
	/* testing ID */	
	//-------------/
	fprintf(stdout,"----TEST01_ID----\n");
	FILE * go_file=Creating_file(go_file,"test01.go","ahoj c88888c AUTO ");
	lex_unit_t* lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first);

	//-------------/
	/* TEST02_ID  */
	/* testing ID */	
	//-------------/
	fprintf(stdout,"----TEST02_ID----\n");
	go_file=Creating_file(go_file,"t.go","c66444X cc AXXXXX8 ");
	lex_first=Loading_lex_units(go_file);
	Prints_lex(lex_first);

	




		return 0;
}
