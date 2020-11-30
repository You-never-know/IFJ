CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../structs/inc/ -I../symtable/inc/ -I../prec_table/inc/ -I../d_tree/inc/ -I../sym_list/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = sem_analysis.o test_sem_analysis.o
TARGET = test_sem_analysis
H =	sem_analysis.h 

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)sem_analysis.o: $(SRC_PATH)sem_analysis.c $(HEADERS) ./../structs/inc/structs.h ./../symtable/inc/symtable.h ./../prec_table/inc/prec_table.h ./../d_tree/inc/d_tree.h ./../sym_list/inc/sym_list.h
	@echo [MESSAGE] CREATING SEM_ANALYSIS.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(OBJ_PATH)test_sem_analysis.o: ./tests/test_sem_analysis.c
	@echo [MESSAGE] CREATING TEST_SEM_ANALYSIS.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

$(TARGET): $(OBJ_FILES) ./../structs/obj/structs.o ./../d_tree/obj/d_tree.o ./../symtable/obj/symtable.o ./../sym_list/obj/sym_list.o 
	@echo [MESSAGE] CREATING EXE TEST_SEM_ANALYSIS
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) $^
	@echo ----------------------------------------------------------------------------------

clean:
	@echo [MESSAGE] DELETING $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
	rm -f $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------