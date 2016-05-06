#include <iostream>
#include <boost/tokenizer.hpp>
#include <string>

int main()
{
    using namespace std;
    using namespace boost;
    string s = "ls -a;echo \"hello;world\" && mkdir test || echo world;git status";
    string separator1("");//dont let quoted arguments escape themselves
    string separator2(";");//split on semicolon
    string separator3("\"");//let it have quoted arguments

    escaped_list_separator<char> temp(separator1,separator2,separator3);
    tokenizer<escaped_list_separator<char> > tok(s, temp);

    for(tokenizer<escaped_list_separator<char> >::iterator beg=tok.begin(); beg!=tok.end();++beg)
    {
        cout << *beg << "\n";
    }
}