#include "../headers/executable.h"

using namespace std;
using namespace boost;

void Executable::run(string command, bool &previous)
{
    const string Exit = "exit";
    vector<string> mytok;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > toks(command, space);
    for (tokenizer<char_separator<char> >::iterator it = toks.begin(); it 
        != toks.end(); it++)
	{
		mytok.push_back(*it);
	}
	if (mytok.at(0) == Exit)
	{
	    cout << "GOODBYE" << endl;
	    exit(0);
	}
    char** temp = new char*[mytok.size() + 1];  //convert tokens to Char**
    unsigned i = 0;
    for (; i < mytok.size(); ++i) {
        temp[i] = new char[mytok.at(i).size()];
        strcpy(temp[i], mytok.at(i).c_str());   //copy tokens to Char**
    }
    temp[i] = NULL;
    
    int status = 0;
    pid_t pid = fork();
    
    if (pid == -1)
    {
        perror("fork");
    }
    if (pid == 0)
    {
        if (execvp(temp[0], temp) == -1) {
            perror("execvp");                   //execvp() failed
            previous = false;
            exit(EXIT_FAILURE);
        }
    }
    if (pid > 0)
    {
        int wpid;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (wpid == -1 && errno == EINTR);
        
        if (wpid == -1)
        {
            perror("Error with wait().");
        }
        if (status > 0)
        {
            previous = false;
        }
        else if (WEXITSTATUS(status) == 0)
        {
            previous = true;
        }
        else if (WEXITSTATUS(status) == 1)
        {
            previous = false;
        }
    }
}