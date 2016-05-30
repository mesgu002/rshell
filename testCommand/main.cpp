#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

void findFile(string userInput, string path)
{
    struct stat sb;
    if(stat(path.c_str(), &sb) == -1)
    {
        perror("stat");
        cout << "(false)" << endl;
        exit(EXIT_FAILURE);
    }
    string str2 = "-";
    size_t found = userInput.find(str2);
    if (found != string::npos)
    {
        if(userInput.at(found + 1) == 'e')
        {
            cout << "E flag found" << endl;
            cout << "(true)" << endl;
        }
        else if (userInput.at(found + 1) == 'f')
        {
            cout << "F flag found" << endl;
            if (S_ISREG(sb.st_mode))
            {
                cout << "(true)" << endl;
            }
            else
            {
                cout << "(false)" << endl;
            }
        }
        else if (userInput.at(found + 1) == 'd')
        {
            cout << "D flag found" << endl;
            if (S_ISDIR(sb.st_mode))
            {
                cout << "(true)" << endl;
            }
            else
            {
                cout << "(false)" << endl;
            }
        }
    }
}

int main() {
    string input;
    cout << "Enter command: ";
    getline(cin, input);
    string checkTest = "test";
    string leftBracket = "[";
    string rightBracket = "]";
    size_t found = input.find(checkTest);
    size_t brackets = input.find(leftBracket);
    if (brackets != string::npos)
    {
        brackets = input.find(rightBracket);
    }
    if (found != string::npos || brackets != string::npos)
    {
        string buffer;
        stringstream ss(input);
        vector<string> tokens;
        while (ss >> buffer)
        {
            tokens.push_back(buffer);
        }
        buffer = "-";
        size_t checkFlag = tokens.at(1).find(buffer);
        if (checkFlag != string::npos)
        {
            findFile(input, tokens.at(2));
        }
        else
        {
            input.append(" -e");
            findFile(input, tokens.at(1));
        }
    }
    return 0;
}