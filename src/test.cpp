#include "../headers/test.h"

using namespace std;

void Test::run(string command, string path, bool &previous)
{
    //use stat to see if the path exists
    struct stat sb;
    
    //error with stat() or path doesn't exist
    if (stat(path.c_str(), &sb) == -1)
    {
        perror("stat");
        cout << "(False)" << endl;
        previous = false;
        return;
    }
    
    //determine which flag was input and use S_ISREG and S_ISDIR macros
    //to test if the path is a directory or normal file
    string str2 = "-";
    size_t found = command.find(str2);
    if (found != string::npos)
    {
        if (command.at(found + 1) == 'e')
        {
            cout << "(True)" << endl;
            previous = true;
        }
        else if (command.at(found + 1) == 'f')
        {
            if (S_ISREG(sb.st_mode))
            {
                cout << "(True)" << endl;
                previous = true;
            }
            else
            {
                cout << "(False)" << endl;
                previous = false;
            }
        }
        else if (command.at(found + 1) == 'd')
        {
            if (S_ISDIR(sb.st_mode))
            {
                cout << "(True)" << endl;
                previous = true;
            }
            else
            {
                cout << "(False)" << endl;
                previous = true;
            }
        }
        
        //user input a flag that doesn't exist
        else
        {
            cout << "Error: Invalid flag" << endl;
            previous = false;
        }
    }
}