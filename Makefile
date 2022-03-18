build:
	g++ -Iinclude -o bin/main -Wall src/main.cpp

run:
	./bin/main

build-run: build run

precompile:
	g++ -c include/*
