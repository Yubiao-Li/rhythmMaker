CXX		:= g++
CXX_FLAGS       := -g -std=c++17 #-Wextra -Wall

SRC		:= src
LIB		:= ./lib

LIBRARIES	:=
EXECUTABLE	:= main

build:./${EXECUTABLE}

all:./$(EXECUTABLE)

run: all
	./$(EXECUTABLE)

$(EXECUTABLE):$(SRC)/*.cpp
	$(CXX) $(CXX_FLAGS) -L$(LIB) $^ -o bin/$@ $(LIBRARIES)