#include "assn1.h"

vector<string> parse(string command)
{
    
    vector<string> fixedCommand;        //vector of parsed strings
    string temp;                        //takes char one at a time from command
    for (unsigned i = 0; i < command.size(); ++i)
    {
        if (command[i] == '"')          //check for quotes
        {
            ++i;
            while (command[i] != '"')    //check for end quote
            {
                temp.push_back(command[i]); //push_back on temp each char in
                ++i;                        //between the two quotes
            }
            if (!temp.empty())               //if command wasn't ""
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            ++i;
        }
        else if (command[i] == ';')     //check for ';'
        {
            if (!temp.empty())           //make sure command isn't ""
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            ++i;
        }
        //Check for "|"
        else if (command[i] == '|' && command[i + 1] == '|')
        {
            if (!temp.empty())           //make sure command isn't ""
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            fixedCommand.push_back("|"); //push_back two '|' for constructors
            fixedCommand.push_back("|"); //when assembling objects
            ++i;
            ++i;
        }
        //Check for "&"
        else if (command[i] == '&' && command[i + 1] == '&')
        {
            if (!temp.empty())           //make sure command isn't ""
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            fixedCommand.push_back("&"); //push_back two '&' for constructors
            fixedCommand.push_back("&"); //when assembling objects
            ++i;
            ++i;
        }
        else if (command[i] == '#')     //Check for '#'
        {
            if (!temp.empty())          //make sure command isn't ""
            {
                fixedCommand.push_back(temp);
                return fixedCommand;    //return because everything after is
            }                           //a comment
            return fixedCommand;
        }
        else
        {
            temp.push_back(command[i]); //Not a connector so is part of a
        }                               //command that shouldn't be split
    }
    if (!temp.empty())                   //make sure command isn't ""
    {
        fixedCommand.push_back(temp);
    }
    return fixedCommand;                //return parsed strings
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
                if (child == 0) //If on left node currently
                {
                    callingExecute(x, location, 1); //Run the right node
                }
                else    //If on right node currently
                {
                    callingExecute(x, ++location, 0); //Run next command
                }
            }
            else if (x.at(location)->isAnd())    //Check to see if And objects
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
        
        parsedString = parse(command);    //Parse at ';|&'
        
        //This loop will check for "exit" from the user's input
        //If "exit" is found the program will terminate
        for (unsigned i = 0; i < parsedString.size(); i++) 
        {
            find = parsedString.at(i).find("exit");
            if (find != string::npos)
            {
                cout << "Goodbye!" << endl;
                exit(0);
            }
        }
        
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