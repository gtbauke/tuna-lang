# Generic Makefile example for C projects
# CAUSSE Raphael, 11/2021
# raphael.causse2@gmail.com

# Directories set up
INCLDIR = src
BINDIR  = bin
OBJDIR  = obj
SRCDIR  = src

# Executable name
TARGET	= $(BINDIR)/tuna

# Project set up, compiler flags and linker flags
# TODO: add release optimization flags
CC      = gcc
CFLAGS  = -std=c99 -g -Wall
LFLAGS  = -Wall -lm -Wextra -pedantic

# Files set up
SRC     = $(wildcard $(SRCDIR)/*.c)
INCL    = $(wildcard $(INCLDIR)/*.h)
OBJ     = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Compile binary and object files
all: $(TARGET)
	
$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)	
	$(CC) $(LFLAGS) $(OBJ) -o $@
	@echo "Linking completed !"

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled successfully !"

# Run executable
run:
	clear
	./$(TARGET)

# Clean entire project directory
.PHONY: clean

clean:
	@rm -rf $(BINDIR) $(OBJDIR)
	@echo "Cleanup completed !"
