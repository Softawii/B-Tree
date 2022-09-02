# Name of the project
PROJ_NAME=bptree

# .c files
C_SOURCE=$(wildcard src/*.c)

# .o files
OBJ:=$(C_SOURCE:.c=.o)

# Compiler
CC=gcc

# Flags
CC_FLAGS= -Wall -O3

################################
# Compilation
################################

all: $(PROJ_NAME)

$(PROJ_NAME): $(OBJ) lib compile clean

lib:
	@echo "\nGenerating library..."
	ar -cvq libbptree.a $(OBJ)

compile:
	@echo "\nCompiling..."
	$(CC) main.c -o tests.out libbptree.a -DHASH_DEBUG

clean:
	@echo "\nRemoving libs and objects..."
	rm libbptree.a
	rm  src/*.o

$(OBJ): %.o: %.c
	@echo "\nCompiling $(<F)..."
	$(CC) $(CC_FLAGS) -c $< -o $@

debug: CC_FLAGS += -DBPTREE_DEBUG
debug: all