CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../structs/inc/ -I../symtable/inc/ -I../d_tree/inc/ -Iinc/ -I../lexical_analyzer/inc/ -I../lex_list/inc -I../sym_list/inc -I../prec_parser/inc/ -I../prec_table/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = syntactic_analyzer.o

H =	syntactic_analyzer.h 

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)syntactic_analyzer.o: $(SRC_PATH)syntactic_analyzer.c $(HEADERS) ./../structs/inc/structs.h ./../symtable/inc/symtable.h ./../d_tree/inc/d_tree.h ./../syntactic_analyzer/inc/syntactic_analyzer.h ./../lexical_analyzer/inc/lexical_analyzer.h ./../lex_list/inc/lex_list.h ./../sym_list/inc/sym_list.h ./../prec_parser/inc/prec_parser.h ./../prec_table/inc/prec_table.h
	@echo [MESSAGE] CREATING  SYNTACTIC_ANALYZER.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

clean:
	@echo [MESSAGE] DELETING $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
	rm -f $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------





