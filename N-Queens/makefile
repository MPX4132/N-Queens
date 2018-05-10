all: Program

compiler = g++
flags = -std=c++11 -O3 -Wall -Wno-unknown-pragmas

Evolution.o: Evolution.cpp Evolution.hpp
	$(compiler) $(flags) -c Evolution.cpp

main.o: main.cpp
	$(compiler) $(flags) -c main.cpp

Program: Evolution.o main.o
	mkdir -p bin
	$(compiler) $(flags) Evolution.o main.o -o bin/Evolution

clean:
	rm -rfv *o bin/*
