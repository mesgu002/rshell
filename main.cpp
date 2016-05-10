#include "assn1.h"
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

void parse(string s, vector<string> &x) {
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
int main() 
{
    string command;
    vector<string> parsedString;
    cout << "$ ";
    getline(cin, command);
    parse(command, parsedString);
    for(unsigned i = 0; i < parsedString.size(); i++) {
        cout << parsedString.at(i) << endl;
    }
    return 0;
}