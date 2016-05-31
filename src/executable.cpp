#include "../headers/executable.h"

using namespace std;
using namespace boost;

void Executable::run(string command, bool &previous)
{
    const string Exit = "exit";
    
    //tokenize at spaces
    vector<string> mytok;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > toks(command, space);
    for (tokenizer<char_separator<char> >::iterator it = toks.begin(); it 
        != toks.end(); it++)
	{
		mytok.push_back(*it);
	}
	
	//exit program if the command is "exit"
	if (mytok.at(0) == Exit)
	{
	    exit(0);
	}
	
	//convert tokens to Char** to use with execvp()
    char** temp = new char*[mytok.size() + 1];
    unsigned i = 0;
    for (; i < mytok.size(); ++i) 
    {
        temp[i] = new char[mytok.at(i).size()];
        strcpy(temp[i], mytok.at(i).c_str());
    }
    temp[i] = NULL;
    
    //call fork so the program doesn't terminate from execvp() call
    int status = 0;
    pid_t pid = fork();
    
    //error with fork()
    if (pid == -1)
    {
        perror("fork");
    }
    
    //child process
    if (pid == 0)
    {
        if (execvp(temp[0], temp) == -1) 
        {
            
            //error with execvp(0)
            perror("execvp");
            previous = false;
            exit(EXIT_FAILURE);
        }
    }
    
    //parent process
    if (pid > 0)
    {
        //wait for child process to finish
        int wpid;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (wpid == -1 && errno == EINTR);
        
        //error with waitpid()
        if (wpid == -1)
        {
            perror("Error with wait().");
        }
        
        //if child process ran an invalid command
        if (status > 0)
        {
            previous = false;
        }
        
        //child process success
        else if (WEXITSTATUS(status) == 0)
        {
            previous = true;
        }
        
        //child process failed
        else if (WEXITSTATUS(status) == 1)
        {
            previous = false;
        }
    }
}