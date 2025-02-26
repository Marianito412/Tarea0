# Compiler name
CC = gcc

# Compiler flags
CFLAGS = `pkg-config --cflags gtk+-3.0 cairo` -Wall -Wextra -std=c11
LIBS = `pkg-config --libs gtk+-3.0 cairo` -lm

# Name of the executable
APPNAME = main

# Source file
SRC = $(APPNAME).c

# Object file
OBJ = $(SRC:.c=.o)

# Default rule: compile the executable
all: $(APPNAME)

# Rule to link the object file and create the executable
$(APPNAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile the .c file into a .o file
$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean generated files
.PHONY: clean
clean:
	rm -f $(OBJ) $(APPNAME)
