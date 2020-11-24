CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../structs/inc/ -I../symtable/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = d_tree.o test_d_tree.o
TARGET = test_d_tree
H =	d_tree.h 

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)d_tree.o: $(SRC_PATH)d_tree.c $(HEADERS) ./../structs/inc/structs.h ./../symtable/inc/symtable.h
	@echo [MESSAGE] CREATING D_TREE.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(OBJ_PATH)test_d_tree.o: ./tests/test_d_tree.c
	@echo [MESSAGE] CREATING LEX_TEST.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(TARGET): $(OBJ_FILES) ./../structs/obj/structs.o
	@echo [MESSAGE] CREATING EXE LEX_TEST
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) $^
	@echo ----------------------------------------------------------------------------------

clean:
	@echo [MESSAGE] DELETING $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
	rm -f $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------




