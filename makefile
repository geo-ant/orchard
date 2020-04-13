ifneq ($(MAKECMDGOALS),test)
APP=orchard
else
APP=test
endif

SRC_DIR=src
INC_DIR=include
OBJ_DIR=obj
BIN_DIR=bin

CC=g++
LD=g++
CFLAGS=-c -Wall -std=c++17
OPTIM_LEVEL = -O3 -march=native -msse4 -mmmx -fopenmp
LFLGAS= -lpthread -ltbb -std=c++17 -lgomp
DFLAGS=-g3 -O0 -c -DDEBUG -Wall -std=c++17
INCFLAGS=-I$(INC_DIR)

SOURCES=$(wildcard $(SRC_DIR)/*.cpp)
HEADERS=$(wildcard $(INC_DIR)/*.hpp)
OBJECTS=$(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
DEPENDS=$(OBJ_DIR)/.depends

.PHONY: all
all:CFLAGS+=$(OPTIM_LEVEL)
all: $(BIN_DIR)/$(APP)

.PHONY: debug
debug: CFLAGS+=$(DFLAGS)
debug: all

$(BIN_DIR)/$(APP): $(OBJECTS) | $(BIN_DIR)
	$(LD) -o $@ $^ $(LFLGAS) 

$(OBJ_DIR)/%.o: | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCFLAGS) -o $@ $<

$(DEPENDS): $(SOURCES) | $(OBJ_DIR)
	$(CC) $(INCFLAGS) -MM $(SOURCES) | sed -e 's!^!$(OBJ_DIR)/!' >$@

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif


$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
