#include "../headers/test.h"

using namespace std;

void Test::run(string command, string path, bool &previous)
{
    struct stat sb;
    if (stat(path.c_str(), &sb) == -1)
    {
        perror("stat");
        cout << "(False)" << endl;
        previous = false;
        return;
    }
    string str2 = "-";
    size_t found = command.find(str2);
    if (found != string::npos)
    {
        if(command.at(found + 1) == 'e')
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
        else
        {
            cout << "Error: Invalid flag" << endl;
            previous = false;
        }
    }
}