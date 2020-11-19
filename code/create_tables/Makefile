CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../symtable/inc/ -I../structs/inc/ -I../sym_list/inc/ -I../lexical_analyzer/inc/ -I../symtable/obj/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = test_table.o \
	create_tables.o 


H =	create_tables.h 

TARGET = test_table

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)test_table.o: ./tests/test_table.c ./../structs/inc/structs.h ./../symtable/inc/symtable.h ./../sym_list/inc/sym_list.h
	@echo [MESSAGE] CREATING TEST_TABLE.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(OBJ_PATH)create_tables.o: $(SRC_PATH)create_tables.c $(INC_PATH)create_tables.h
	@echo [MESSAGE] CREATING CREATE_TABLES.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------
$(TARGET): $(OBJ_FILES) ./../structs/obj/structs.o ./../symtable/obj/symtable.o ./../sym_list/obj/sym_list.o ./../lexical_analyzer/obj/lexical_analyzer.o 
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



