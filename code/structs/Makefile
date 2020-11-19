CFLAGS = -g -std=c99 -O2 -pedantic -Wall -Wextra -Iinc -I../symtable/inc/
COMPILATOR = gcc $(CFLAGS) -o $@

#paths to each dirictories
OBJ_PATH = ./obj/
SRC_PATH = ./src/
INC_PATH = ./inc/
#name of exe

OBJ =structs.o

H =	structs.h

OBJ_FILES = $(patsubst %,$(OBJ_PATH)%,$(OBJ)) #all obj files
HEADERS = $(patsubst %,$(INC_PATH)%,$(H)) #all headers

all:$(OBJ_FILES) $(TARGET)

$(OBJ_PATH)structs.o: $(SRC_PATH)structs.c $(INC_PATH)structs.h
	@echo [MESSAGE] CREATING  STRUCT.O
	@echo ----------------------------------------------------------------------------------
	$(COMPILATOR) -c $<
	@echo ----------------------------------------------------------------------------------

clean:
	rm -f $(OBJ_FILES)




