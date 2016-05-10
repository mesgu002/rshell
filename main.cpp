#include "assn1.h"
#include <boost/tokenizer.hpp>
#include <stdlib.h>

using namespace std;
using namespace boost;

void parse(string s, vector<string> &x) 
{
    string separator1("");//dont let quoted arguments escape themselves
    string separator2(";||&&");//split on semicolon
    string separator3("\"");//let it have quoted arguments

    escaped_list_separator<char> temp(separator1,separator2,separator3);
    tokenizer<escaped_list_separator<char> > tok(s, temp);

    for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
    {
        x.push_back(*beg);
    }
}

int main() 
{
    string command;
    vector<string> parsedString;
    cout << "$ ";
    getline(cin, command);
    parse(command, parsedString);
    vector<Base *> commands;
    for(unsigned i = 0; i < parsedString.size(); i++) 
    {
        Executable* x = new Executable(parsedString.at(i), NULL);
        commands.push_back(x);
        //cout << parsedString.at(i) << endl;
        //system(parsedString.at(i).c_str());
    }
    cout << commands.at(0)->getArguement() << endl;
    //system(parsedString.at(0));
    
    return 0;
}