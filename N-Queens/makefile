all: Program

compiler = g++
flags = -std=c++0x -g

Evolution.o: Evolution.cpp
	$(compiler) $(flags) -c Evolution.cpp

main.o: main.cpp
	$(compiler) $(flags) -c main.cpp

Program: Evolution.o main.o
	$(compiler) $(flags) Evolution.o main.o -o Evolution

clean:
	rm -rfv *o Evolution
