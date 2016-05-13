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

void parseConnectors(vector<string> x, vector<string> &y)                   //split up parsed strings
{
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("", "|&");                                     //splits at | and &
    for (unsigned i = 0; i < x.size(); i++)
    {
        tokenizer tokens(x.at(i), sep);                                                                 
        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != 
            tokens.end(); ++tok_iter)       
        {
            y.push_back(*tok_iter);                                         //pushes back on new vector of strings
        }
    }
}

bool callingExecute(vector<Base *> x, bool counter, unsigned location) 
{
    if (location == x.size() - 1) {
        return true;
    }
    int status = 0;
    int pid = fork();
    if (pid == -1) 
    {
        perror("fork");
    }
    else if (pid == 0) //child
    {
        x.at(location)->execute();
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
        if (!counter)
        {
            x.at(location)->multihasBeenExecuted(true, false);
            callingExecute(x, true, location);
        }
        else
        {
            x.at(location)->multihasBeenExecuted(true, true);
            x.at(location)->multiExecuted();
            callingExecute(x, false, ++location);
        }
    }
    return true;
}

// ls -a; echo hello && echo world || echo hello world
    //expect: ls -a
    //        hello
    //        world
// echo hello || echo world && echo hello world
    //expect: hello
// ech hello || echo world && echo hello world
    //expect: world
    //        hello world

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
                        Executable* x = new Executable(parsedString.at(i - 1),
                            true, false);
                        Executable* y = new Executable(parsedString.at(i + 2),
                            true, false);
                        Or* cmd = new Or(x, y);
                        cout << "TEST1" << endl;
                        commands.push_back(cmd);
                        i += 2;
                    }
                }
                else if (parsedString.at(i) == "&" && i < parsedString.size() - 2)
                {
                    if (parsedString.at(i + 1) == "&")
                    {
                        Executable* x = new Executable(parsedString.at(i - 1),
                            true, false);
                        Executable* y = new Executable(parsedString.at(i + 2),
                            true, false);
                        And* cmd = new And(x, y);
                        cout << "TEST2" << endl;
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
                        Executable* x = new Executable(parsedString.at(0), true, false);
                        commands.push_back(x);
                    }
                    Executable* x = new Executable(parsedString.at(i), true, false);
                    cout << "TEST3" << endl;
                    commands.push_back(x);
                }
            }
        }
        cout << commands.size() << endl;
        break;
    }
    return 0;
}