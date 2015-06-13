.PHONY: all
all: build run

.PHONY: build
build:
	g++ main.cpp lib/*.cpp -I. -L./GL -lGL -lGLU -lglut -o bin/game

.PHONY: run
run:
	bin/game

