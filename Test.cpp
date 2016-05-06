#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>
#include <cstring>
#include <vector>

using namespace std;
using namespace boost;

int main()
{
    string s;
    vector<string> parsedString;
    cout << "Enter command: ";
    getline(cin, s);
    string separator1("");//dont let quoted arguments escape themselves
    string separator2(";");//split on semicolon
    string separator3("\"");//let it have quoted arguments

    escaped_list_separator<char> temp(separator1,separator2,separator3);
    tokenizer<escaped_list_separator<char> > tok(s, temp);

    for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
    {
        parsedString.push_back(*beg);
    }
//    for(int i = 0; i<parsedString.size(); i++) {
//	cout << parsedString.at(i) << endl;
//    }
}
