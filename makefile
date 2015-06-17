COMPILER = g++
SOURCES = main.cpp lib/*.cpp
INCLUDES = -I.
LIBRARIES = -L./GL -lGL -lGLU -lglut
WARNINGS = -Wall -Wno-comment

.PHONY: all
all: build run

.PHONY: build
build:
	g++ -std=c++11 $(SOURCES) $(INCLUDES) $(LIBRARIES) $(WARNINGS) -o bin/game

.PHONY: run
run:
	bin/game

