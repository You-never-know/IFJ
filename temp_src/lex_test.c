#include "lexical_analyzer.h"

int Error(const char *msg){
	fprintf(stderr,"%s\n",msg);
	exit(1); 
}
int Prints_lex(lex_unit_t*First){
	lex_unit_t * tmp=First;

	while(tmp!=NULL){

		if(tmp->unit_type==IDENTIFICATOR || tmp->unit_type==KEYWORD || //string formats 
		   tmp->unit_type==STRING){
			fprintf(stdout,"unit_type: %d \n",tmp->unit_type);
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
	lex_unit_t *first=act;// first ptr of lex_units
	while((act=Analyze(go_file))!=NULL){//loading units 
		act->next=malloc(sizeof(lex_unit_t));
		act=act->next;
	}
	return first;
}

int main()
{

	//-------------/
	/* TEST01	  */
	/* testing ID */	
	//-------------/
	FILE * go_file=Creating_file(go_file,"test01.go","ahoj int aa");
	lex_unit_t* lex_first=Loading_lex_units(go_file);
	printf("%ld\n",lex_first->data_size);
	//Prints_lex(lex_first);

	




		return 0;
}
