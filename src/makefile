all: main.o assn1.o
	g++ -W -Wall -Werror -pedantic -ansi main.o assn1.o

main.o: assn1.cpp assn1.h main.cpp
	g++ -W -Wall -Werror -pedantic -ansi -c main.cpp
	
assn1.o: assn1.cpp assn1.h
	g++ -W -Wall -Werror -pedantic -ansi -c assn1.cpp
	
clean:
	rm *o a.out