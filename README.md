# rshell
##A c++ implementation of shell
##Authors and Contributors
Glenn Cochran and Montana Esguerra
---

##To Run rshell:
Enter these commands in the following order:  
```
1. git clone https://github.com/mesgu002/rshell
2. cd rshell
3. make
4. ./a.out
```
You are now ready to use rshell!

##How it works:
rshell takes in commands on one line and executes them from left to right.  
Currently, it supports ls, mkdir, and echo. It also supports the connectors &&, ||, and ;

###&& Connector
If a command is followed by the && connector, then the next command is executed only if the first one succeeds.

###|| Connector
If a command is followed by the || connector, then the next command is executed only if the first one fails.

###; Connector
If a command is followed by the ; connector, then the next command is always executed.


Parser:
    Parser unable to handle quotes("")

Syscalls:
    Not implemented

To run rshell:
    git clone https://github.com/yourusername/rshell.git
    cd rshell
    git checkout hw2
    make
    bin/rshell

Partner info .txt:
    name1=Glenn Cochran
    ucrnetid1=gcoch001
    name2=Montana Esguerra
    ucrnetid2=mesgu002

Make a directory called src

Make a makefile:
    targets:
    	-all
	-rshell

To compile:
    g++ -Wall -Werror -ansi -pedantic

Readme:
    summarize file
    known bugs

~Read intro to writing readme files~

Directory named test:
    contains bash script that fully tests each segment
    single_command.sh
    multi_command.sh
    commented_command.sh
    exit.sh


~Comment code~
Follow CalTech coding guidelines
