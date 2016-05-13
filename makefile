target = bin/rshell
objects = main.o assn1.o
FLAGS = -Wall -Werror -ansi -pedantic

vpath %.cpp src

all: $(target)

$(target): $(objects)
	mkdir bin; $(FLAGS) -c
