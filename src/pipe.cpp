#include "../headers/pipe.h"

using namespace std;
using namespace boost;

struct pipes
{
    string left;
    int fd[2];
};

vector <pipes> args;
int save[2];

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
  	}
}

void setpipe(bool first, bool last, pipes a, pipes b, bool &previous)
{
    if (first)
	{
		if((save[0] = dup(0))==-1)
		{
			perror("dup");
			previous = false;
		}
		if((save[1] = dup(1))==-1)
		{
			perror("dup");
			previous = false;
		}
	}
	if (!first)
	{
		if(close(0)==-1)
		{
			perror("close");
			previous = false;
		}
		if(dup(b.fd[0])==-1)
		{
			perror("dup");
			previous = false;
		}
		if(close(b.fd[0])==-1)
		{
			perror("close");
			previous = false;
		}
	}
	if (!last)
	{
		if(close(1)==-1)
		{
			perror("close");
			previous = false;
		}
		if(dup(a.fd[1])==-1)
		{
			perror("dup");
			previous = false;
		}
		if(close(a.fd[1])==-1)
		{
			perror("close");
			previous = false;
		}

	}
	if (last)
	{
		if(close(1)==-1)
		{
			perror("close");
			previous = false;
		}
		if(dup2(save[1], 1)==-1)
		{
			perror("dup2");
			previous = false;
		}
		if(close(save[1])==-1)
		{
			perror("close");
			previous = false;
		}
		
	}
}

void closing(int in, int out, int error, bool &previous)
{
    if (in > -1)
    {
        if (close(0) == -1)
        {
            perror("close");
            previous = false;
        }
        if (dup(in) == -1)
        {
            perror("dup");
            previous = false;
        }
    }
    if (out > -1)
    {
        if (close(1) == -1)
        {
            perror("close");
        }
        if (dup(out) == -1)
        {
            perror("dup");
            previous = false;
        }
    }
    if (error > -1)
    {
        if (close(2) == -1)
        {
            perror("close");
        }
        if (dup(error) == -1)
        {
            perror("dup");
            previous = false;
        }
    }
}

void reset(bool finish, bool &previous)
{
    if (finish)
    {
        if (close(0) == -1)
        {
            perror("close");
            previous = false;
        }
        if (dup2(save[0], 0) == -1)
        {
            perror("dup2");
            previous = false;
        }
        if (close(save[0]) == -1)
        {
            perror("close");
            previous = false;
        }
    }
}

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
  	}
}

void Pipe::run(string command, bool &previous)
{
    if (command.find("<") != string::npos)
    {
        int out = -1, in = -1;
        string left = command.substr(0, command.find("<"));
        string right = command.substr(command.find("<") + 1);
        string prev = right;
        in = openFile(prev, O_RDWR, previous);
        if (in == -1)
        {
            perror("in");
            previous = false;
        }
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
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            previous = false;
        }
        else if (pid == 0)
        {
            execute(left, in, out, -1, previous);
        }
        else
        {
            if (in != -1)
            {
                if (close(in) == -1)
                {
                    perror("close");
                    previous = false;
                }
            }
            if (out != -1)
            {
                if (close(out) == -1)
                {
                    perror("close");
                    previous = false;
                }
            }
        }
    }
    else if (command.find("|") != string::npos)
    {
        bool out = false;
        bool append = false;
        if (command.find("<") != string::npos)
        {
            int temp = command.find("<");
            command.erase(temp, 1);
        }
        if (command.find(">") != string::npos)
        {
            out = true;
        }
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
        args.clear();
        bool done = true;
        pipes pipe1;
        size_t index;
        while ((index = command.find("|")) != string::npos)
        {
            pipe1.left = command.substr(0, index);
            if (pipe(pipe1.fd) == -1)
            {
                perror("pipe");
                previous = false;
            }
            args.push_back(pipe1);
            command = command.substr(command.find("|") + 1);
        }
        pipe1.left = command;
        if (pipe(pipe1.fd) == -1)
        {
            perror("pipe");
            previous = false;
        }
        args.push_back(pipe1);
        index = 0;
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
            
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
            }
            else if (pid == 0)
            {
                executePipe(args.at(index).left, previous);
            }
            ++index;
        }
        int status = 0;
        while (wait(&status) > 0);
        reset(done, previous);
    }
    else if (command.find(">>") != string::npos)
    {
        append:
            int permissions = O_RDWR|O_CREAT|O_APPEND;
            int index = command.find(">>");
            string left = command.substr(0, index);
            string right = command.substr(command.find(">>") + 2);
            int opened = openFile(right, permissions, previous);
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                previous = false;
            }
            else if (pid == 0)
            {
                execute(left, -1, opened, -1, previous);
            }
            else 
            {
                if (wait(0) == -1)
                {
                    perror("wait");
                    previous = false;
                }
                if (close(opened) == -1)
                {
                    perror("close");
                    previous = false;
                }
            }
    }
    else if (command.find(">") != string::npos)
    {
        out:
            int permissions = O_RDWR|O_CREAT|O_TRUNC;
            int index = command.find(">");
            string left = command.substr(0, index);
            string right = command.substr(command.find(">") + 1);
            int opened = openFile(right, permissions, previous);
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                previous = false;
            }
            else if (pid == 0)
            {
                execute(left, -1, opened, -1, previous);
            }
            else
            {
                if (wait(0) == -1)
                {
                    perror("wait");
                    previous = false;
                }
                if (close(opened) == -1)
                {
                    perror("close");
                    previous = false;
                }
            }
    }
}