#include "../headers/or.h"

using namespace std;
using namespace boost;

void Or::run(string command, bool &previous)
{
    size_t found = command.find("test");
    size_t bracket = command.find("[");
    if (previous)
    {
        previous = true;
    }
    else if (bracket == 0 || found == 0)
    {
        Test* x = new Test();
        string buffer;
        stringstream ss(command);
        vector<string> tokens;
        while (ss >> buffer)
        {
            tokens.push_back(buffer);
        }
        buffer = "-";
        size_t checkFlag = tokens.at(1).find(buffer);
        if (checkFlag != string::npos)
        {
            x->run(command, tokens.at(2), previous);
        }
        else
        {
            command.append(" -e");
            x->run(command, tokens.at(1), previous);
        }
    }
    else 
    {
        this->cmd->run(command, previous);
    }
}