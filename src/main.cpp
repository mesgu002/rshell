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
    //int curr;
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
    bool loop = true;
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
    while (loop)
    {    
        vector<string> tempString1;
        vector<string> parsedString;
        if (getlogin() != NULL)
        {
            cout << login << "@" << host;
        }
        cout << "$ ";
        getline(cin, command);
        parse(command, tempString1);
        vector<Base *> commands;
        for (unsigned i = 0; i < tempString1.size(); i++) 
        {
            find = tempString1.at(i).find("exit");
            if (find != string::npos)
            {
                cout << "Goodbye!" << endl;
                exit(0);
            }
        }
        vector<string> mytok;
    	char_separator<char> space(" ");
    	for (unsigned i = 0; i < tempString1.size(); ++i)
    	{
            size_t found = tempString1.at(i).find(" ");
            if (found == 0)
    	    {
    	        tempString1.at(i).replace(tempString1.at(i).find(" "),1 , "");
    	    }
    	}
        parseConnectors(tempString1, parsedString);
        vector<Base* > tempString(parsedString.size() - 1);
        for (unsigned i = 0; i < parsedString.size(); i++)
        {
            tempString.push_back(0);
            if (parsedString.at(i) != "|" && parsedString.at(i) != "&")
            {
                Executable* x = new Executable(parsedString.at(i), true, false);
                tempString.at(i) = x;
            }
        }
        
        if (parsedString.size() > 2) 
        {
            for (unsigned i = 0; i < parsedString.size() - 2; i++) 
            {
                if(parsedString.at(i + 1) == "|" && parsedString.at(i + 2) == 
                    "|")
                {
                    Or* z = new Or(tempString.at(i), tempString.at(i + 3));
                    commands.push_back(z);
                }
                else if (parsedString.at(i + 1) == "&" && parsedString.at(i + 2)
                    == "&")
                {
                    And* a = new And(tempString.at(i), tempString.at(i + 3));
                    commands.push_back(a);
                }
                else if (parsedString.at(i) != "|" && parsedString.at(i) != "&")
                {
                    commands.push_back(tempString.at(i));
                }
            }
            commands.push_back(NULL);
            loop = callingExecute(commands, false, 0);
        }
        
        else if (parsedString.size() == 2)
        {
            commands.push_back(tempString.at(0));
            commands.push_back(tempString.at(1));
            int status = 0;
            int pid = fork();
            if (pid == -1) 
            {
                perror("fork");
            }
            else if (pid == 0) //child
            {
                commands.at(0)->execute();
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
                else
                {
                    commands.at(1)->execute();
                }
            }
        }
        
        else
        {
            commands.push_back(tempString.at(0));
            commands.push_back(NULL);
            commands.at(0)->execute();
        }
        
    }
    return 0;
}