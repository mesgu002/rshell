#include "../headers/parser.h"

using namespace std;

void Parser::parse(string command)
{
    
    vector<string> fixedCommand;        //vector of parsed strings
    string temp;                        //takes char one at a time from command
    int checkQuotes = 0;
    for (unsigned i = 0; i < command.size(); ++i)
    {
        if (command[i] == '"')          //check for quotes
        {
            ++checkQuotes;
            unsigned j = i + 1;
            for (; j < command.size(); ++j)
            {
                if (command.at(j) == '"')
                {
                    --checkQuotes;
                    break;
                }
                else {
                    
                }
            }
            temp.insert(0, command, i + 1, j - i - 1);
            if (!temp.empty())               //if command wasn't ""
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            i = j + 1;
        }
        else if (command[i] == ';')     //check for ';'
        {
            if (!temp.empty())           //make sure command isn't ""
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            fixedCommand.push_back(";");
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
            fixedCommand.push_back("||");
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
            fixedCommand.push_back("&&");
            ++i;
            ++i;
        }
        else if (command[i] == '#')     //Check for '#'
        {
            fixedCommand.push_back(temp);
            break;
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
    
    vector<Base* > execute;
    bool previous = true;
    for (unsigned i = 0; i < fixedCommand.size(); ++i)
    {
        if (i == 0)
        {
            Executable* x = new Executable();
            x->run(fixedCommand.at(i), previous);
        }
        else if (fixedCommand.at(i) == "||")
        {
            Executable* y = new Executable();
            Or* x = new Or(y);
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
        else if (fixedCommand.at(i) == "&&")
        {
            Executable* y = new Executable();
            And* x = new And(y);
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
        else if (fixedCommand.at(i) == ";")
        {
            Executable* x = new Executable();
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
    }
}