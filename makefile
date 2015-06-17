COMPILER = g++
SOURCES = main.cpp lib/*.cpp
INCLUDES = -I.
LIBRARIES = -L./GL -lGL -lGLU -lglut
WARNINGS = -Wall -Wno-comment

.PHONY: all
all: build run

.PHONY: mac
mac: buildmac run



.PHONY: build
build:
	mkdir -p bin
	g++ -std=c++11 $(SOURCES) $(INCLUDES) $(LIBRARIES) $(WARNINGS) -o bin/game

.PHONY: buildmac
buildmac:
	mkdir -p bin
	g++ -std=c++11 $(SOURCES) -I. -L lib -framework OpenGL -framework GLUT -o bin/game



.PHONY: run
run:
	bin/game