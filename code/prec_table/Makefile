CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../structs/inc/ -I../symtable/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ = prec_table.o

H =	prec_table.h 

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)prec_table.o: $(SRC_PATH)prec_table.c $(HEADERS) ./../structs/inc/structs.h ./../symtable/inc/symtable.h
	@echo [MESSAGE] CREATING  PREC_TABLE.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

clean:
	@echo [MESSAGE] DELETING $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------
	rm -f $(OBJ_FILES)
	@echo ----------------------------------------------------------------------------------




