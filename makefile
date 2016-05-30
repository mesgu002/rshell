CC = g++
FLAGS = -W -Wall -Werror -pedantic -ansi
PARAMS = main.o and.o executable.o or.o parser.o test.o paren.o


all: $(PARAMS)
	mkdir -p ./bin
	$(CC) $(FLAGS) $(PARAMS) -o ./bin/rshell
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
	
test.o: src/test.cpp
	$(CC) $(FLAGS) src/test.cpp -c
	
paren.o: src/paren.cpp
	$(CC) $(FLAGS) src/paren.cpp -c
	
clean:
	rm -rf bin