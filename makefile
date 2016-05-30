CC = g++
FLAGS = -W -Wall -Werror -pedantic -ansi


all: main.o and.o executable.o or.o parser.o
	mkdir -p ./bin
	$(CC) $(FLAGS) main.o and.o executable.o or.o parser.o -o ./bin/rshell
	rm *o

main.o: src/main.cpp
	$(CC) $(FLAGS) src/main.cpp src/parser.cpp -c
	
and.o: src/and.cpp
	$(CC) $(FLAGS) src/and.cpp -c
	
executable.o: src/executable.cpp
	$(CC) $(FLAGS) src/executable.cpp -c
	
or.o: src/or.cpp
	$(CC) $(FLAGS) src/or.cpp -c
	
parser.o: src/parser.cpp
	$(CC) $(FLAGS) src/parser.cpp -c
	
clean:
	rm -rf bin