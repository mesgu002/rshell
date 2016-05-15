#include "assn1.h"

using namespace std;
using namespace boost;

void parse(string s, vector<string> &x) 
{
    string separator1("");//dont let quoted arguments escape themselves
    string separator2(";");//split on semicolon
    string separator3("\"");//let it have quoted arguments

    escaped_list_separator<char> temp(separator1,separator2,separator3);
    tokenizer<escaped_list_separator<char> > tok(s, temp);

    for (tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); 
        beg!=tok.end();++beg)
    {
        x.push_back(*beg);
    }
}

void parseConnectors(vector<string> x, vector<string> &y)
{                                                   //split up parsed strings
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("", "|&");             //splits at | and &
    for (unsigned i = 0; i < x.size(); i++)
    {
        tokenizer tokens(x.at(i), sep);                                                                 
        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != 
            tokens.end(); ++tok_iter)       
        {                                   //pushes back on new 
            y.push_back(*tok_iter);         //vector of strings        
        }
    }
}

void callingExecute(vector<Base *> x, unsigned location, int child) 
{
    if (location >= x.size() - 1)
    {
        return;
    }
    int status = 0;
    int pid = fork();
    if (pid == -1) 
    {
        perror("fork");
    }
    else if (pid == 0) //child
    {
        x.at(location)->execute(child);
    }
    else //parent
    {
        int wpid;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        }
        while (wpid == -1 && errno == EINTR);

        if (wpid == -1)
        {
            perror("Error with wait().");
        }
        if (WIFSIGNALED(status))
        {
            x.at(location)->setChildExecuted(false, child);
            // last call failed
            if (x.at(location)->isOr())
            {
                x.at(location)->setChildBeenExecuted(true, child);
                if(child == 0)
                {
                    callingExecute(x, location, 1); // run the other side
                }
                else
                {
                    callingExecute(x, ++location, 0);
                }
            }
            else if(x.at(location)->isAnd())
            {
                x.at(location)->setChildBeenExecuted(true, child);
                if (child == 0)
                {
                    x.at(location)->setChildExecuted(false, 1);
                    x.at(location)->setChildBeenExecuted(true, 1);
                }
                callingExecute(x, ++location, 0); // run next command
            }
            else
            {
                callingExecute(x, ++location, 0);
            }
        }
        else
        {
            if (x.at(location)->isOr())
            {
                x.at(location)->setChildBeenExecuted(true, child);
                if (child == 0)
                {
                    x.at(location)->setChildExecuted(false, 1);
                    x.at(location)->setChildBeenExecuted(true, 1);
                }
                callingExecute(x, ++location, 1);
            }
            else if (x.at(location)->isAnd())
            {
                x.at(location)->setChildBeenExecuted(true, child);
                if (child == 0)
                {
                    callingExecute(x, location, 1);
                }
                else
                {
                    callingExecute(x, ++location, 0);
                }
            }
            else
            {
                callingExecute(x, ++location, 0);
            }
        }
    }
}

int main() 
{
    string command;
    size_t find;
    string login;
    if (getlogin() == NULL)
    {
        login = "";
        perror("get login failed");
    }
    else
    {
        login = getlogin();
    }
    char host[100];
    if (gethostname(host, 100) == -1)
    {
        perror("get host name failed");
    }
    while (true)
    {
        vector<string> tempString1;
        vector<string> parsedString;
        vector<Base *> commands;
        
        while (command == "")
        {
            if (getlogin() != NULL)
            {
                cout << login << "@" << host;
            }
            cout << "$ ";
            getline(cin, command);    
        }
        
        parse(command, tempString1);
        
        for (unsigned i = 0; i < tempString1.size(); i++) 
        {
            find = tempString1.at(i).find("exit");
            if (find != string::npos)
            {
                cout << "Goodbye!" << endl;
                exit(0);
            }
        }
        
        parseConnectors(tempString1, parsedString); //parses at connectors
        
        vector<string> mytok;       //check to make sure first char isnt ' '
    	char_separator<char> space(" ");
    	for (unsigned i = 0; i < parsedString.size(); ++i)
    	{
            size_t found = parsedString.at(i).find(" ");
            if (found == 0)
    	    {
    	        parsedString.at(i).replace
    	            (parsedString.at(i).find(" "), 1 , "");
    	    }
    	}
    	
    	vector<Base* > test;
    	
    	for (unsigned i = 0; i < parsedString.size(); ++i)
    	{
    	    if (parsedString.at(i) != "|" && parsedString.at(i) != "&")
    	    {
    	        Executable* x = new Executable(parsedString.at(i), true, false);
    	        test.push_back(x);
    	    }
    	    else
    	    {
    	        test.push_back(0);
    	    }
    	}
    	   
    	if (parsedString.size() == 1)
    	{
    	    Executable* x = new Executable(parsedString.at(0), true, false);
    	    commands.push_back(x);
    	}
    	else
    	{
            for (unsigned i = 1; i < parsedString.size(); ++i)
            {
                if (parsedString.at(i) == "|" && i < parsedString.size() - 2)
                {
                    if (parsedString.at(i + 1) == "|")
                    {
                        // Executable* x = new Executable(parsedString.at(i - 1),
                        //     true, false);
                        // Executable* y = new Executable(parsedString.at(i + 2),
                        //     true, false);
                        // Or* cmd = new Or(x, y);
                        Or* cmd = new Or(test.at(i - 1), test.at(i + 2));
                        commands.push_back(cmd);
                        i += 2;
                    }
                }
                else if (parsedString.at(i) == "&" && 
                    i < parsedString.size() - 2)
                {
                    if (parsedString.at(i + 1) == "&")
                    {
                        // Executable* x = new Executable(parsedString.at(i - 1),
                        //     true, false);
                        // Executable* y = new Executable(parsedString.at(i + 2),
                        //     true, false);
                        // And* cmd = new And(x, y);
                        And* cmd = new And(test.at(i - 1), test.at(i + 2));
                        commands.push_back(cmd);
                        i += 2;
                    }
                }
                else if (parsedString.at(i) == "#")
                {
                    break;
                }
                else
                {
                    if (i == 1)
                    {
                        Executable* x = new Executable(parsedString.at(0), 
                            true, false);
                        commands.push_back(x);
                    }
                    Executable* x = new Executable(parsedString.at(i), 
                        true, false);
                    commands.push_back(x);
                }
            }
        }
        commands.push_back(NULL);

        callingExecute(commands, 0, 0);

        command = "";
    }
    return 0;
}