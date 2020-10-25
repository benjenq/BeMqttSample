CXX		  := g++

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -lpaho-mqtt3cs -lpaho-mqtt3as -lstdc++ -lpaho-mqttpp3 #-lpthread -lm /usr/local/lib/libpaho-mqttpp3.so.1.1.0 #-lpaho-mqttpp3
else
    CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -lpaho-mqtt3cs -lpaho-mqtt3as -lstdc++ -lpaho-mqttpp3 #-lpthread -lm /usr/local/lib/libpaho-mqttpp3.so.1.1.0 #-lpaho-mqttpp3
endif

BIN		:= bin
SRC		:= src
INCLUDE	:= include
LIB		:= lib

LIBRARIES	:=
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(INCLUDE)/*.cpp
	$(CXX) -I$(INCLUDE) -L$(LIB) $^ -o $@ $(LIBRARIES) $(CXX_FLAGS)

.PHONY : clean, clear
clean:
	-rm -R $(BIN)/*

clear:
	-rm -R $(BIN)/*
	-clear
