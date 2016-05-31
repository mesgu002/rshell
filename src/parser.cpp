#include "../headers/parser.h"

using namespace std;

void Parser::parse(string command, bool &previous)
{
    vector<string> fixedCommand;        //vector of parsed strings
    string temp;                        //takes char one at a time from command
    for (unsigned i = 0; i < command.size(); ++i)
    {
        if (command[i] == '"')          //check for quotes
        {
            int checkQuotes = 0;
            ++checkQuotes;
            unsigned j = i + 1;
            for (; j < command.size(); ++j)
            {
                if (command.at(j) == '"')
                {
                    --checkQuotes;
                    break;
                }
            }
            if (checkQuotes == 0)
            {
                temp.insert(0, command, i + 1, j - i - 1);
                if (!temp.empty())               //if command wasn't ""
                {
                    fixedCommand.push_back(temp);
                    temp = "";
                }
                i = j + 1;
            }
            else
            {
                cout << "Error: No matching quote" << endl;
                exit(1);
            }
        }
        
        else if (command[i] == '(')
        {
            int checkParen = 0;
            ++checkParen;
            unsigned j = i + 1;
            for (; j < command.size(); ++j)
            {
                if (command.at(j) == ')')
                {
                    --checkParen;
                    break;
                }
            }
            if (checkParen == 0)
            {
                fixedCommand.push_back("(");
                temp.insert(0, command, i + 1, j - i - 1);
                if (!temp.empty())
                {
                    fixedCommand.push_back(temp);
                    temp = "";
                }
                fixedCommand.push_back(")");
                i = j + 1;
            }
            else
            {
                cout << "Error: No matching parenthesis" << endl;
                exit(1);
            }
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
    for (unsigned i = 0; i < fixedCommand.size(); ++i)
    {
        size_t found = fixedCommand.at(i).find("test");
        size_t bracket = fixedCommand.at(i).find("[");
        if (found == 0 || bracket == 0)
        {
            Test* x = new Test();
            string buffer;
            stringstream ss(fixedCommand.at(i));
            vector<string> tokens;
            while (ss >> buffer)
            {
                tokens.push_back(buffer);
            }
            buffer = "-";
            size_t checkFlag = tokens.at(1).find(buffer);
            if (checkFlag != string::npos)
            {
                x->run(fixedCommand.at(i), tokens.at(2), previous);
            }
            else
            {
                fixedCommand.at(i).append(" -e");
                x->run(fixedCommand.at(i), tokens.at(1), previous);
            }
            ++i;
        }
        else if (fixedCommand.at(i) == "(")
        {
            Paren* x = new Paren();
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
        else if (i == 0)
        {
            Executable* x = new Executable();
            x->run(fixedCommand.at(i), previous);
        }
        else if (fixedCommand.at(i) == "||")
        {
            Executable* y = new Executable();
            Or* x = new Or(y);
            if (fixedCommand.at(i + 1) == "[")
            {
                x->run(fixedCommand.at(i + 2), previous);
            }
            else
            {
                x->run(fixedCommand.at(i + 1), previous);
            }
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