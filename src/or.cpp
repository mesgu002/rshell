#include "../headers/or.h"

using namespace std;
using namespace boost;

void Or::run(string command, bool &previous)
{
    //test for "test" and "[" in command
    size_t found = command.find("test");
    size_t bracket = command.find("[");
    
    //if the previous command succeeded don't execute command
    if (previous)
    {
        previous = true;
    }
    
    //if the command is a "test" or "[" command then don't run normally
    //with execvp, waitpid, and fork syscalls
    else if (bracket == 0 || found == 0)
    {
        Test* x = new Test();
        string buffer;
        
        //tokenize at spaces
        stringstream ss(command);
        vector<string> tokens;
        while (ss >> buffer)
        {
            tokens.push_back(buffer);
        }
        
        //check to see if there is a flag
        buffer = "-";
        size_t checkFlag = tokens.at(1).find(buffer);
        
        //if there is a flag run normally
        if (checkFlag != string::npos)
        {
            x->run(command, tokens.at(2), previous);
        }
        
        //no flag from user input so append the '-e' flag
        else
        {
            command.append(" -e");
            x->run(command, tokens.at(1), previous);
        }
    }
    
    //run the command with execvp, waitpid, and fork syscalls
    else 
    {
        this->cmd->run(command, previous);
    }
}