#include "assn1.h"


//parse the string s (user input) at ';' without interrupting quoted
//arguements
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


//parse the strings at '|' and '&' and push_back onto another vector.
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


//Use the syscalls fork, waitpid, and execvp to execute the commands
//recursively
void callingExecute(vector<Base *> x, unsigned location, int child) 
{
    
    if (location >= x.size() - 1)       //Function ends when the it goes
    {                                   //through every element in the vector
        return;                         //x.size() - 1 because last element is
    }                                   //'NULL'
    
    int status = 0;
    int pid = fork();
    
    if (pid == -1)                      //fork() error
    {
        perror("fork");
    }
    else if (pid == 0)                  //child process
    {
        x.at(location)->execute(child); //child parameter is to pick which
    }                                   //node (left or right) to execute
    
    else                                //parent process
    {
        int wpid;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED); //Parent waits for child
        }                                            //to finish
        while (wpid == -1 && errno == EINTR);

        if (wpid == -1)                              //Error with waitpid()
        {
            perror("Error with wait().");
        }
        
        if (WIFSIGNALED(status))                    //execvp() error or
        {                                           //invalid command
            x.at(location)->setChildExecuted(false, child);
            // last call failed
            if (x.at(location)->isOr())     //Check to see if Or objects
            {
                //Set node was executed
                x.at(location)->setChildBeenExecuted(true, child);
                if(child == 0) //If on left node currently
                {
                    callingExecute(x, location, 1); //Run the right node
                }
                else    //If on right node currently
                {
                    callingExecute(x, ++location, 0); //Run next command
                }
            }
            else if(x.at(location)->isAnd())    //Check to see if And objects
            {
                //Set node was executed
                x.at(location)->setChildBeenExecuted(true, child);
                if (child == 0) //If on left node currently
                {
                    //Set right node didn't succeed, but was executed
                    x.at(location)->setChildExecuted(false, 1);
                    x.at(location)->setChildBeenExecuted(true, 1);
                }
                //If on right node currently
                callingExecute(x, ++location, 0); // Run next command
            }
            else //If executable object
            {
                callingExecute(x, ++location, 0); // Run next command
            }
        }
        
        else                                        //execvp() executed
        {                                           //correctly
            if (x.at(location)->isOr())         //Check to see if Or objects
            {
                //Set node was executed
                x.at(location)->setChildBeenExecuted(true, child);
                if (child == 0) //If on left node currently
                {
                    //Set right node didn't succeed, but was executed
                    x.at(location)->setChildExecuted(false, 1);
                    x.at(location)->setChildBeenExecuted(true, 1);
                }
                //If on right node currently
                callingExecute(x, ++location, 1); //Run next command
            }
            else if (x.at(location)->isAnd())   //Check to see if And objects
            {
                //Set node was executed
                x.at(location)->setChildBeenExecuted(true, child);
                if (child == 0) //If on left node currently
                {
                    callingExecute(x, location, 1); //Run right node
                }
                else //If on right node currently
                {
                    callingExecute(x, ++location, 0); //Run next command
                }
            }
            else    //If executable object
            {
                callingExecute(x, ++location, 0);   //Run next command
            }
        }
    }
}

int main() 
{
    string command;     //Used for user input
    size_t find;        //Find if there is an exit ('exit') in 'command'
    size_t comments;    //Find if there is a comment ('#') in 'command' 
    string login;       //Get the login name of the user
    if (getlogin() == NULL)
    {
        login = "";
        perror("get login failed"); //getlogin() failed
    }
    else
    {
        login = getlogin();         //getlogin() success
    }
    char host[100];                     //Get the user's hostname
    if (gethostname(host, 100) == -1)
    {
        perror("get host name failed"); //gethostname() failed
    }
    while (true)                        //Loop until exit called
    {
        vector<string> tempString1;     //Store strings after paring ';'
        vector<string> parsedString;    //Store strings after parsing '|,&'
        vector<Base *> commands;        //Store Base* from each 'parsedString'
        
        while (command == "")           //Reloop if user types empty string
        {
            if (getlogin() != NULL)     //Just display "$ " if getlogin() failed
            {
                cout << login << "@" << host;
            }
            cout << "$ ";
            getline(cin, command);      //Get user input
        }
        
        parse(command, tempString1);    //Parse at ';'
        
        //This loop will check for "exit" and "#" from the user's input
        //If "exit" is found the program will terminate
        //If "#" is found then everything after the # will be removed from
            //tempString1
        for (unsigned i = 0; i < tempString1.size(); i++) 
        {
            find = tempString1.at(i).find("exit");
            comments = tempString1.at(i).find("#");
            if (find != string::npos)
            {
                cout << "Goodbye!" << endl;
                exit(0);
            }
            if (comments != string::npos)
            {
                tempString1.at(i).erase(tempString1.at(i).begin() + comments
                    , tempString1.at(i).end());
            }
        }
        
        parseConnectors(tempString1, parsedString); //Parse at connectors('|&')
        
        //Check to make sure first char isnt ' ' and remove it if it is
        vector<string> mytok;
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
    	
    	vector<Base* > createExecs;
    	
    	//Creates the new executables all at the same time so the executable's
    	//data will be shared even when used for two different connector pointer
    	//constructors
    	for (unsigned i = 0; i < parsedString.size(); ++i)
    	{
    	    if (parsedString.at(i) != "|" && parsedString.at(i) != "&")
    	    {
    	        Executable* x = new Executable(parsedString.at(i), true, false);
    	        createExecs.push_back(x);
    	    }
    	    else
    	    {
    	        createExecs.push_back(0);
    	    }
    	}
    	   
    	//Checks to see if only a single command was inputed
    	if (parsedString.size() == 1)
    	{
    	    //Creates new Executable
    	    Executable* x = new Executable(parsedString.at(0), true, false);
    	    commands.push_back(x);
    	}
    	//Multiple commands
    	else
    	{
            for (unsigned i = 1; i < parsedString.size(); ++i)
            {
                //If the next 2 locations are "|" connectors and i doesn't go
                //out of bounds, creates new Or object with the 2 commands
                //before and after the '|' locations
                if (parsedString.at(i) == "|" && i < parsedString.size() - 2)
                {
                    if (parsedString.at(i + 1) == "|")
                    {
                        Or* cmd = new Or(createExecs.at(i - 1), 
                            createExecs.at(i + 2));
                        commands.push_back(cmd);
                        i += 2;
                    }
                }
                //If the next 2 locations are "&" connectors and i doesn't go
                //out of bounds, creates new And object with the 2 commands
                //before and after the '&' locations
                else if (parsedString.at(i) == "&" && 
                    i < parsedString.size() - 2)
                {
                    if (parsedString.at(i + 1) == "&")
                    {
                        And* cmd = new And(createExecs.at(i - 1), 
                            createExecs.at(i + 2));
                        commands.push_back(cmd);
                        i += 2;
                    }
                }
                //If the command doesn't have a connector associated with it
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
        commands.push_back(NULL);       //push back 'NULL' in order for 
                                        //execvp() to work

        callingExecute(commands, 0, 0); //Recursive function to execute
                                        //all the commands

        command = "";                   //user input changed to empty string
                                        //for the loop to get user input
    }
    return 0;
}