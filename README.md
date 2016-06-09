# rshell

![rshell](/img/rshellLogo.png) 

A **c++** implementation of **shell**
Authors
------------
[Glenn Cochran](https://github.com/gcoch001 "Loves long distance running")

[Montana Esguerra](https://github.com/mesgu002 "Enjoys long walks on the beach")




##To Run rshell:
Enter these commands in the following order:  
```
1. git clone https://github.com/mesgu002/rshell
2. cd rshell
3. make
4. ./bin/rshell
```
You are now ready to use **rshell**

##How it works:
rshell takes in commands on one line and executes them from left to right.  
Currently, it supports ls, mkdir, and echo. It also supports the connectors &&, ||, and ;

###&& Connector
If a command is followed by the && connector, then the next command is executed only if the first one succeeds.

###|| Connector
If a command is followed by the || connector, then the next command is executed only if the first one fails.

###; Connector
If a command is followed by the ; connector, then the next command is always executed.

### Precedence
rshell handles precedence using the parentheses operators. 

For example, 
```
$ echo A && echo B || echo C && echo D
```
would print:
```
A
B
D
```

However, we can add parentheses to change the precedence of the connectors
```
$ (echo A && echo B) || (echo C && echo D)
```
which would print
```
A
B
```
### The Test Command
The test command allows users to do various tests and sets its exit code to 0 (*TRUE*) or 1 (*FALSE*) whenever such a test succeeds or not.
This command allows users to run tests using the following flags:

```
-e checks if the file/directory exists
-f checks if the file/directory exists and is a regular file 
-d checks if the fiel/directory exists and is a directory
```
If a user does not specify a flag, the -e functionality will be used by default. The result of the test will be printed to stdout as either *True* or *False*.

Alternatively, the test command can be run by using the [] operator.

```
i.e. test -e makefile is the same as [-e makefile]
```

## Test Scripts:
We have also included a set of test scripts for you to try with rshell!  
To run them, enter this command within the src directory:

```
./test/script
```
where script can be replaced by any of these following provided scripts:
```
1. single_command.sh // tests single commands
2. multi_command.sh // tests commands with ;, &&, or ||
3. commented_command.sh // tests commands with comments
4. exit.sh             //tests exit  and commands with exit
```
##Partner Info:

Partner info .txt:
```
    name1=Glenn Cochran
    ucrnetid1=gcoch001
    name2=Montana Esguerra
    ucrnetid2=mesgu002
	repourl= https://github.com/mesgu002/rshell
```
##License
GNU General Public License.
Version 3, 29 June 2007

##Bugs:

1. rshell can't handle the cd command.
2. Can't execute commands on the same line exit() is called
3. Can't change user passwords using pwd
4. Doesn't color text like bash
5. rshell can't handle if exit is the first command
6. Pipe Bug: Can't hand ls | cat | wc
