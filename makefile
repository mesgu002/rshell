all: main.o assn1.o
	mkdir -p ./bin
	g++ -W -Wall -Werror -pedantic -ansi main.o assn1.o -o ./bin/rshell

main.o: src/main.cpp
	mkdir -p ./bin
	g++ -W -Wall -Werror -pedantic -ansi src/main.cpp -c
	
assn1.o: src/assn1.cpp src/assn1.h
	mkdir -p ./bin
	g++ -W -Wall -Werror -pedantic -ansi src/assn1.cpp -c
	
clean:
	rm *o -rf bin