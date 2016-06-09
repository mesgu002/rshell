#include "../headers/pipe.h"

using namespace std;
using namespace boost;

//all the data our pipes need so code is more neat
struct pipes
{
    string left;
    int fd[2];
};

//create vector of all pipe ('|') commands
vector <pipes> args;
int save[2];

//opens a file for input and gives proper permission to the use the file
int openFile(string &cmd, int permissions, bool &previous)
{
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > tok(cmd, space);
    tokenizer<char_separator<char> >::iterator it = tok.begin();
    int valid = open((*it).c_str(), permissions, S_IRWXU|S_IRWXG|S_IRWXO);
    if (valid == -1)
    {
        perror("open");
        previous = false;
    }
    return valid;
}

//used to execute a pipe ('|') command
void executePipe(string cmd, bool &previous)
{
    char *command[999];
    int cntr = 0;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > tok(cmd, space);
	tokenizer<char_separator<char> >::iterator it = tok.begin();
	for(; it != tok.end(); ++it ,++cntr)
	{
    	command[cntr] = new char[(*it).size()];
    	strcpy(command[cntr],(*it).c_str());
  	}
  	command[cntr] = 0;
    if (execvp(command[0], command) == -1)
  	{
  	    perror("execvp");
  	    previous = false;
        exit(EXIT_FAILURE);
  	}
}

//sets all the necessary data for the pipes being created and does all the error
//checking in one place
void setpipe(bool first, bool last, pipes a, pipes b, bool &previous)
{
    if (first)
	{
		if((save[0] = dup(0))==-1)
		{
			perror("dup");
			previous = false;
            return;
		}
		if((save[1] = dup(1))==-1)
		{
			perror("dup");
			previous = false;
            return;
		}
	}
	if (!first)
	{
		if(close(0)==-1)
		{
			perror("close");
			previous = false;
            return;
		}
		if(dup(b.fd[0])==-1)
		{
			perror("dup");
			previous = false;
            return;
		}
		if(close(b.fd[0])==-1)
		{
			perror("close");
			previous = false;
            return;
		}
	}
	if (!last)
	{
		if(close(1)==-1)
		{
			perror("close");
			previous = false;
            return;
		}
		if(dup(a.fd[1])==-1)
		{
			perror("dup");
			previous = false;
            return;
		}
		if(close(a.fd[1])==-1)
		{
			perror("close");
			previous = false;
            return;
		}

	}
	if (last)
	{
		if(close(1)==-1)
		{
			perror("close");
			previous = false;
            return;
		}
		if(dup2(save[1], 1)==-1)
		{
			perror("dup2");
			previous = false;
            return;
		}
		if(close(save[1])==-1)
		{
			perror("close");
			previous = false;
            return;
		}
		
	}
}

//closes the appropriate input output or error for the files being used with
//'>>', '>', and '<' and does error checking
void closing(int in, int out, int error, bool &previous)
{
    if (in > -1)
    {
        if (close(0) == -1)
        {
            perror("close");
            previous = false;
            return;
        }
        if (dup(in) == -1)
        {
            perror("dup");
            previous = false;
            return;
        }
    }
    if (out > -1)
    {
        if (close(1) == -1)
        {
            perror("close");
            previous = false;
            return;
        }
        if (dup(out) == -1)
        {
            perror("dup");
            previous = false;
            return;
        }
    }
    if (error > -1)
    {
        if (close(2) == -1)
        {
            perror("close");
            previous = false;
            return;
        }
        if (dup(error) == -1)
        {
            perror("dup");
            previous = false;
            return;
        }
    }
}

//closes the pipe('|') commands and check for errors
void reset(bool finish, bool &previous)
{
    if (finish)
    {
        if (close(0) == -1)
        {
            perror("close");
            previous = false;
            return;
        }
        if (dup2(save[0], 0) == -1)
        {
            perror("dup2");
            previous = false;
            return;
        }
        if (close(save[0]) == -1)
        {
            perror("close");
            previous = false;
            return;
        }
    }
}

//used to execute all redirection ('>>', '>', '<') commands
void execute(string &cmd, int in, int out, int error, bool &previous)
{
    char *command[999];
    int cntr = 0;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > tok(cmd, space);
	tokenizer<char_separator<char> >::iterator it = tok.begin();
	for(; it != tok.end(); ++it ,++cntr)
	{
    	command[cntr] = new char[(*it).size()];
    	strcpy(command[cntr],(*it).c_str());
  	}
  	command[cntr] = 0;
  	closing(in, out, error, previous);
  	if (execvp(command[0], command) == -1)
  	{
  	    perror("execvp");
  	    previous = false;
        exit(EXIT_FAILURE);
  	}
}

void Pipe::run(string command, bool &previous)
{
    //check for "<"
    if (command.find("<") != string::npos)
    {
        int out = -1, in = -1;
        //string of everything left of "<"
        string left = command.substr(0, command.find("<"));
        //string of everything right of "<"
        string right = command.substr(command.find("<") + 1);
        string prev = right;
        //open the file on the right
        in = openFile(prev, O_RDWR, previous);
        if (in == -1)
        {
            perror("in");
            previous = false;
            return;
        }
        
        //check to see if ">" or ">>" command also present in the command
        //if there is gives appropriate permissions
        if (right.find(">") != string::npos)
        {
            prev = prev.substr(0, right.find(">"));
            int permissions = O_RDWR|O_CREAT;
            int next;
            if (right.find(">>") != string::npos)
            {
                permissions |= O_APPEND;
                next = right.find(">>") + 2;
            }
            else
            {
                permissions |= O_TRUNC;
                next = right.find(">") + 1;
            }
            right = right.substr(next);
            out = openFile(right, permissions, previous);
        }
        
        //execute the command and determine if it was executed correctly
        //changes the previous bool based on execution outcome
        int status = 0;
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            previous = false;
            return;
        }
        if (pid == 0)
        {
          	execute(left, in, out, -1, previous);
        }
        if (pid > 0)
        {
            int wpid;
            do
            {
                wpid = waitpid(pid, &status, WUNTRACED);
            }
            while (wpid == -1 && errno == EINTR);
            
            if (wpid == -1)
            {
                perror("wait");
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
        
        //close all inputs
        if (in != -1)
        {
            if (close(in) == -1)
            {
                perror("close");
                previous = false;
                return;
            }
        }
        
        //close all outputs
        if (out != -1)
        {
            if (close(out) == -1)
            {
                perror("close");
                previous = false;
                return;
            }
        }
    }
    
    //check for "|"
    else if (command.find("|") != string::npos)
    {
        bool out = false;
        bool append = false;
        
        //check for "<" also in the command
        if (command.find("<") != string::npos)
        {
            int temp = command.find("<");
            command.erase(temp, 1);
        }
        
        //check for ">" also in the command and jump to ">" if true
        if (command.find(">") != string::npos)
        {
            out = true;
        }
        
        //check for ">>" also in the command and jump to ">>" if true
        if (command.find(">>") != string::npos)
        {
            append = true;
        }
        if (out)
        {
            int temp = 0;
            temp = command.find("|");
            command = command.substr(temp + 1);
            goto out;
        }
        if (append)
        {
            int temp = 0;
            temp = command.find("|");
            command = command.substr(temp + 1);
            goto append;
        }
        
        //clear vector of previous data
        args.clear();
        bool done = true;
        pipes pipe1;
        size_t index;
        
        //check for multiple "|"
        while ((index = command.find("|")) != string::npos)
        {
            pipe1.left = command.substr(0, index);
            if (pipe(pipe1.fd) == -1)
            {
                perror("pipe");
                previous = false;
                return;
            }
            args.push_back(pipe1);
            command = command.substr(command.find("|") + 1);
        }
        pipe1.left = command;
        if (pipe(pipe1.fd) == -1)
        {
            perror("pipe");
            previous = false;
            return;
        }
        args.push_back(pipe1);
        index = 0;
        
        //create pipes with all the appropriate "filters"
        while (index != args.size())
        {
            bool first = false;
            bool last = false;
            pipes temp = args.at(index);
            if (index == 0)
            {
                first = true;
                setpipe(first, last, temp, temp, previous);
            }
            else if (index == args.size() - 1)
            {
                pipes temp2 = args.at(index - 1);
                last = true;
                setpipe(first, last, temp, temp2, previous);
            }
            else
            {
                pipes temp2 = args.at(index - 1);
                setpipe(first, last, temp, temp2, previous);
            }
            
            //execute commands with pipes and determine if command executed
            //correctly and update previous based on outcome
            int status = 0;
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                previous = false;
                return;
            }
            if (pid == 0)
            {
                executePipe(args.at(index).left, previous);
            }
            if (pid > 0)
            {
                int wpid;
                do
                {
                    wpid = waitpid(pid, &status, WUNTRACED);
                }
                while (wpid == -1 && errno == EINTR);
                
                if (wpid == -1)
                {
                    perror("wait");
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
            ++index;
        }
        int status = 0;
        while (wait(&status) > 0);
        reset(done, previous);
    }
    
    //check for ">>"
    else if (command.find(">>") != string::npos)
    {
        append:
            //give correct permissions
            int permissions = O_RDWR|O_CREAT|O_APPEND;
            int index = command.find(">>");
            
            //string of everything left of ">>"
            string left = command.substr(0, index);
            
            //string of everything right of ">>"
            string right = command.substr(command.find(">>") + 2);
            
            //open right
            int opened = openFile(right, permissions, previous);
            
            //execute command and update previous based on success or failure
            //of execution
            int status = 0;
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                previous = false;
                return;
            }
            if (pid == 0)
            {
              	execute(left, -1, opened, -1, previous);
            }
            if (pid > 0)
            {
                int wpid;
                do
                {
                    wpid = waitpid(pid, &status, WUNTRACED);
                }
                while (wpid == -1 && errno == EINTR);
                
                if (wpid == -1)
                {
                    perror("wait");
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
            
            //close file
            if (close(opened) == -1)
            {
                perror("close");
                previous = false;
                return;
            }
    }
    else if (command.find(">") != string::npos)
    {
        out:
            //give correct permissions
            int permissions = O_RDWR|O_CREAT|O_TRUNC;
            int index = command.find(">");
            
            //string of everything left of ">"
            string left = command.substr(0, index);
            
            //string of everything right of "<"
            string right = command.substr(command.find(">") + 1);
            
            //open file
            int opened = openFile(right, permissions, previous);
            
            //execute command and update previous based on the success or
            //failure of the execution
            int status = 0;
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                previous = false;
                return;
            }
            if (pid == 0)
            {
              	execute(left, -1, opened, -1, previous);
            }
            if (pid > 0)
            {
                int wpid;
                do
                {
                    wpid = waitpid(pid, &status, WUNTRACED);
                }
                while (wpid == -1 && errno == EINTR);
                
                if (wpid == -1)
                {
                    perror("wait");
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
            
            //close file
            if (close(opened) == -1)
            {
                perror("close");
                previous = false;
                return;
            }
    }
}