#include "../headers/parser.h"
#include <stdio.h>
#include <unistd.h>

using namespace std;

int main()
{
    Parser* cmds = new Parser();
    string command;     //Used for user input
    string login;       //Get the login name of the user
    bool previous = true;
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
        if (getlogin() != NULL)     //Just display "$ " if getlogin() failed
        {
            cout << login << "@" << host;
        }
        cout << "$ ";
        getline(cin, command);      //Get user input
        cmds->parse(command, previous);
    }
    return 0;
}