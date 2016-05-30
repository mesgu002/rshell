CC = g++
CFLAGS = -Wall -Werror -ansi -pedantic
DIRECT = bin
SRCS = src
SRC := $(wildcard src/*.cpp)
OBJS :=  $(notdir $(SRC:.cpp=.o))

all: rshell

rshell: $(DIRECT)/rshell

$(DIRECT)/rshell: $(DIRECT)/$(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(DIRECT)/%.o: $(SRCS)/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIRECT)/:
	mkdir -p $@

clean:
	rm -r $(DIRECT)
