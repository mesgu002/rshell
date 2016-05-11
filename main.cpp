#include "assn1.h"
#include <boost/tokenizer.hpp>
#include <stdlib.h>

using namespace std;
using namespace boost;

void parse(string s, vector<string> &x) 
{
    string separator1("");//dont let quoted arguments escape themselves
    string separator2(";");//split on semicolon
    string separator3("\"");//let it have quoted arguments

    escaped_list_separator<char> temp(separator1,separator2,separator3);
    tokenizer<escaped_list_separator<char> > tok(s, temp);

    for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
    {
        x.push_back(*beg);
    }
}

void parseConnectors(vector<string> x, vector<string> &y)                   //split up parsed strings
{
    typedef tokenizer<char_separator<char> > tokenizer;
    char_separator<char> sep("", "|&");                                     //splits at | and &
    for(unsigned i = 0; i < x.size(); i++)
    {
        tokenizer tokens(x.at(i), sep);                                                                 
        for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)       
        {
            y.push_back(*tok_iter);                                         //pushes back on new vector of strings
        }
    }
}

// ls -a; echo hello && echo world || echo hello world
    //expect: ls -a
    //        hello
    //        world
//echo hello || echo world && echo hello world
    //expect: hello
//ech hello || echo world && echo hello world
    //expect: world
    //        hello world

int main() 
{
    string command;
    vector<string> tempString1;
    vector<string> parsedString;
    cout << "$ ";
    getline(cin, command);
    parse(command, tempString1);
    vector<Base *> commands;
    for(unsigned i = 0; i < tempString1.size(); i++) 
    {
        cout << tempString1.at(i) << endl;
    }
    cout << "---------" << endl;
    parseConnectors(tempString1,parsedString);
    vector<Base* > tempString(parsedString.size() - 1);
    for(unsigned i = 0; i < parsedString.size(); i++)
    {
        tempString.push_back(0);
        if(parsedString.at(i) != "|" && parsedString.at(i) != "&")
        {
            Executable* x = new Executable(parsedString.at(i), true, false);
            tempString.at(i) = x;
        }
    }
    for(unsigned i = 0; i < parsedString.size() - 2; i++) 
    {
        if(parsedString.at(i + 1) == "|" && parsedString.at(i + 2) == "|")
        {
            Or* z = new Or(tempString.at(i), tempString.at(i + 3));
            commands.push_back(z);
        }
        else if(parsedString.at(i + 1) == "&" && parsedString.at(i + 2) == "&")
        {
            And* a = new And(tempString.at(i), tempString.at(i + 3));
            commands.push_back(a);
        }
        else if(parsedString.at(i) != "|" && parsedString.at(i) != "&")
        {
            commands.push_back(tempString.at(i));
        }
    }
    for(unsigned i = 0; i < commands.size(); i++) 
    {
        commands.at(i)->execute();
    }
    
    return 0;
}