#include "../headers/parser.h"

using namespace std;

void Parser::parse(string command, bool &previous)
{
    vector<string> fixedCommand;        //vector of parsed strings
    string temp;                        //takes char one at a time from command
    for (unsigned i = 0; i < command.size(); ++i)
    {
        //check for quotes
        if (command[i] == '"')
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
                temp.insert(temp.size(), command, i + 1, j - i - 1);
                //if command wasn't ""
                if (!temp.empty())               
                {
                    fixedCommand.push_back(temp);
                    temp = "";
                }
                i = j + 1;
            }
            
            //no end quote
            else
            {
                cout << "Error: No matching quote" << endl;
                exit(1);
            }
        }
        
        //check for "(" and ")"
        else if (command[i] == '(')
        {
            int checkParen = 0;
            ++checkParen;
            unsigned j = i + 1;
            for (; j < command.size(); ++j)
            {
                if (command.at(j) == '(')
                {
                    ++checkParen;
                }
                else if (command.at(j) == ')')
                {
                    --checkParen;
                    if (checkParen == 0)
                    {
                        break;
                    }
                    // break;
                }
            }
            //both "(" and ")" were found so push the connectors and everything
            //in between into fixedCommand
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
            
            //no end ")"
            else
            {
                cout << "Error: No matching parenthesis" << endl;
                exit(1);
            }
        }
        
        //check for ';' and push onto fixedCommand
        else if (command[i] == ';')
        {
            //make sure command isn't ""
            if (!temp.empty())
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            fixedCommand.push_back(";");
            ++i;
        }
        
        //Check for "|" and push onto fixedCommand
        else if (command[i] == '|' && command[i + 1] == '|')
        {
            //make sure command isn't ""
            if (!temp.empty())
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            fixedCommand.push_back("||");
            ++i;
            ++i;
        }
        
        //Check for "&" and push onto fixedCommand
        else if (command[i] == '&' && command[i + 1] == '&')
        {
            //make sure command isn't ""
            if (!temp.empty())
            {
                fixedCommand.push_back(temp);
                temp = "";
            }
            fixedCommand.push_back("&&");
            ++i;
            ++i;
        }
        
        //Check for redirection or pipe command
        else if (command[i] == '<' || command[i] == '>' || command[i] == '|')
        {
            //if double redirection won't push 2 '>' to fixedCommand
            if (command[i + 1] == '>')
            {
                temp.push_back(command[i]);
                ++i;
            }
            //push everything till reaches connector or eof to buffer
            temp.push_back(command[i]);
            
            //ensure connector isn't pushed to fixedCommand twice
            if (fixedCommand.size() == 0 || 
                fixedCommand.at(fixedCommand.size() - 1) != ">")
            {
                fixedCommand.push_back(">");
            }
        }
        
        //Check for '#' and only push everything before the '#' onto
        //fixedCommand
        else if (command[i] == '#')
        {
            fixedCommand.push_back(temp);
            break;
        }
        
        //not a character that requires a break so continue to next character
        else
        {
            temp.push_back(command[i]); //Not a connector so is part of a
        }                               //command that shouldn't be split
    }
    
    //make sure command isn't ""
    if (!temp.empty())
    {
        fixedCommand.push_back(temp);
    }
    
    for (unsigned i = 0; i < fixedCommand.size(); ++i)
    {
        cout << fixedCommand.at(i) << endl;
    }
    
    vector<Base* > execute;
    
    //iterate through fixedCommand to create the appropriate object type based
    //on the connector before each command
    for (unsigned i = 0; i < fixedCommand.size(); ++i)
    {
        //test for test command
        size_t found = fixedCommand.at(i).find("test");
        size_t bracket = fixedCommand.at(i).find("[");
        
        //test command was found
        if (found == 0 || bracket == 0)
        {
            Test* x = new Test();
            string buffer;
            
            //tokenize at spaces
            stringstream ss(fixedCommand.at(i));
            vector<string> tokens;
            while (ss >> buffer)
            {
                tokens.push_back(buffer);
            }
            
            //check to see if a flag was input by the user
            buffer = "-";
            size_t checkFlag = tokens.at(1).find(buffer);
            
            //flag was input
            if (checkFlag != string::npos)
            {
                x->run(fixedCommand.at(i), tokens.at(2), previous);
            }
            
            //no flag so append '-e' flag
            else
            {
                fixedCommand.at(i).append(" -e");
                x->run(fixedCommand.at(i), tokens.at(1), previous);
            }
            ++i;
        }
        
        else if (fixedCommand.at(i) == ">")
        {
            Pipe* x = new Pipe();
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
        
        //check to see if parenthesis
        else if (fixedCommand.at(i) == "(")
        {
            Paren* x = new Paren();
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
        
        //run first command irregardless of connector
        else if (i == 0)
        {
            Executable* x = new Executable();
            x->run(fixedCommand.at(i), previous);
        }
        
        //command is an "Or" command
        else if (fixedCommand.at(i) == "||")
        {
            Executable* y = new Executable();
            Or* x = new Or(y);
            if (fixedCommand.at(i + 1) != "(" && fixedCommand.at(i + 1) != ">")
            {
                x->run(fixedCommand.at(i + 1), previous);
                ++i;
            }
        }
        
        //command is an "And" command
        else if (fixedCommand.at(i) == "&&")
        {
            Executable* y = new Executable();
            And* x = new And(y);
            if (fixedCommand.at(i + 1) != "(" && fixedCommand.at(i + 1) != ">")
            {
                x->run(fixedCommand.at(i + 1), previous);
                ++i;
            }
        }
        
        //command is a "Semicolon" command
        else if (fixedCommand.at(i) == ";")
        {
            Executable* x = new Executable();
            x->run(fixedCommand.at(i + 1), previous);
            ++i;
        }
    }
}