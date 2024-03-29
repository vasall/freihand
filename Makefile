# Name of the created executable
TARGET     := lfreihand.a

# Get the absolute path to the directory this makefile is in
MKFILE_PTH := $(abspath $(lastword $(MAKEFILE_LIST)))
MKFILE_DIR := $(dir $(MKFILE_PTH))

# All subdirectories in the lib-folder
LIB_PTH    := ../
LIB_DIRS   := $(sort $(dir $(wildcard $(MKFILE_DIR)$(LIB_PTH)/*/)))

# The compiler to use
CC         := gcc
# Error flags for compiling
WARNFLAGS   := -Wall -Wextra -Wmissing-prototypes -Wstrict-prototypes -Wold-style-definition
# Compiling flags here
CFLAGS     := -g -O0 -ansi -std=c89 -I./source -pedantic -D_POSIX_C_SOURCE=200809L -I./$(LIB_PTH)/

# The linker to use
LINKER     := gcc

# Make does not offer a recursive wildcard function, so here's one:
# https://stackoverflow.com/a/12959694
rwildcard   = $(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

# Change these to proper directories where each file should be
SRCDIR     := source
OBJDIR     := obj

SOURCES    := $(call rwildcard,$(SRCDIR)/,*.c)
OBJECTS    := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

rm         := rm -f

$(TARGET): $(OBJECTS)
	@ar -r -o $@ $(addprefix $(OBJDIR)/, $(notdir $^))
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	@$(CC) $(CFLAGS) $(WARNFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)
	@echo "Compiled $<"

