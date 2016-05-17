# rshell

![rshell](/img/rshellLogo.png) 

A **c++** implementation of **shell**
##Authors and Contributors

	Glenn Cochran [Glenn's github](https://www.github.com/gcoch001)
	[I'm an inline-style link](https://www.google.com)
	Montana Esguerra [Montana's github](https://www.github.com/mesgu002)
	[link text itself]: http://www.reddit.com
---

##To Run rshell:
Enter these commands in the following order:  
```
1. git clone https://github.com/mesgu002/rshell
2. cd rshell
3. make
4. ./bin/rshell
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
3. Connectors within quotes are not treated as comments. The parser still treats them as connectors.
---
