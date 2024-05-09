#############################################################################
#	Simple Build System - Jason Schmidt
#############################################################################

#############################################################################
#	1:	Environment Setup
#############################################################################

# Project Name
PROJECT_NAME=m2m

# Subdirectory Control
SOURCE_DIR=src
INCLUDE_DIR=include
OUTPUT_DIR=build

# Toolchain Control
CC=gcc
CPPC=g++

# Compiler Variables
CPPFLAGS =  -Wall -g -std=c++17
CFLAGS = -Wall -g -std=c2x

# Linker Stuff
LDFLAGS+= -lm -lboost_serialization

# Autogen

SOURCE_FILES_C := $(shell find $(SOURCE_DIR) -name "*.c")
SOURCE_FILES_CPP := $(shell find $(SOURCE_DIR) -name "*.cpp")
OBJS_C := $(addprefix $(OUTPUT_DIR)/, $(SOURCE_FILES_C:%.c=%.o))
OBJS_CPP := $(addprefix $(OUTPUT_DIR)/, $(SOURCE_FILES_CPP:%.cpp=%.o))
OBJS := $(OBJS_C) $(OBJS_CPP)
AUTODEP_FILES = $(OBJS:%.o=%.d)

CFLAGS += -I$(INCLUDE_DIR)
CPPFLAGS += -I$(INCLUDE_DIR)

.DEFAULT_GOAL := $(PROJECT_NAME)

#############################################################################
#	2:	Rules
#############################################################################

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJS)
	@printf " [ LD  ]\t   $@\n"
	@$(CPPC) $(CPPFLAGS) $(OBJS) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	@printf " [CLEAN]\n"
	@-rm -rf $(OUTPUT_DIR)
	@-rm -rf $(DOC_DIR)
	@-rm -rf $(PROJECT_NAME)

.PHONY: run
run: $(PROJECT_NAME)
	@printf " [$(PROJECT_NAME)]\n--------------------\n\n"
	@./$(PROJECT_NAME)

-include $(AUTODEP_FILES)


$(OBJS_C): $(OUTPUT_DIR)/%.o : %.c
	@printf " [ CC  ]\t   $<\n"
	@mkdir -p $(dir $@)
	@$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(OBJS_CPP): $(OUTPUT_DIR)/%.o : %.cpp
	@printf " [ CC  ]\t   $<\n"
	@mkdir -p $(dir $@)
	@$(CPPC) -MMD $(CPPFLAGS) -c -o $@ $<


#############################################################################
#	2:	Custom Rules
#############################################################################

