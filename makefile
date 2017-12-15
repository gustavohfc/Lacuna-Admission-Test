SRC_DIR = src
INC_DIR = ./include
OBJ_DIR = obj

SOURCES  = $(wildcard $(SRC_DIR)/*.cpp)
INCLUDES = $(wildcard $(INC_DIR)/*.h)
OBJECTS  = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS = -g -Wall -Werror -std=c++14 -I$(INC_DIR)
EXE = hack_empire
CXX = g++

all: $(EXE)

$(EXE): $(OBJECTS) 
	$(CXX) $(CPPFLAGS) $(OBJECTS) -o $@

$(OBJECTS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm $(OBJ_DIR)/*

