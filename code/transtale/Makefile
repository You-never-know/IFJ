CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../symtable/inc/ -I../structs/inc/ -I../sym_list/inc/ -I../lexical_analyzer/inc/ -I../symtable/inc/ -I../create_tables/inc/ -I../syntactic_analyzer/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = translate.o 


TARGET = translate

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files

all: $(OBJ_PATH)translate.o  $(TARGET) 

$(OBJ_PATH)translate.o: ./src/translate.c ./../structs/inc/structs.h ./../symtable/inc/symtable.h ./../sym_list/inc/sym_list.h ./../symtable/inc/symtable.h ./../create_tables/inc/create_tables.h ./../syntactic_analyzer/inc/syntactic_analyzer.h
	@echo [MESSAGE] CREATING TEST_TABLE.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(TARGET):  ./../structs/obj/structs.o ./../symtable/obj/symtable.o ./../sym_list/obj/sym_list.o ./../lexical_analyzer/obj/lexical_analyzer.o ./../create_tables/obj/create_tables.o ./../syntactic_analyzer/obj/syntactic_analyzer.o
	@echo [MESSAGE] CREATING EXE TEST_TABLE
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) $^
	@echo ----------------------------------------------------------------------------------

clean:
	@echo [MESSAGE] DELETING $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
	rm -f $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
move:
	@echo [MESSAGE] MOVING TEST_TABLE
	@echo ----------------------------------------------------------------------------------
	mv $(TARGET) ./../test_files/
	@echo ----------------------------------------------------------------------------------