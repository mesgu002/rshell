#include <iostream>
#include <cstring>
#include <vector>
#include <cstdio>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;


int main() 
{
    string temp;
    const char delim[2] = ";";
    char *parsedString;
    vector<char*> parsedStrings;
    cout << "Enter command: ";
    getline(cin, temp);
    int index1 = temp.find("\"");
    int index2 = temp.find("\"", index1 + 1);
    int found = temp.find(";", index1, 1);
    if(found < index2) {
        
    }
    cout << index1 << " " << index2 << " " << found << endl;
    char* userCommand = &temp[0];
    parsedString = strtok(userCommand, delim);
    
    while(parsedString != NULL) 
    {
        parsedStrings.push_back(parsedString);
        //printf ( " %s\n", parsedString );
        parsedString = strtok(NULL, delim);
    }
    
    cout << "Vector of separated commands: " << endl;
    
    for(int i = 0; i < parsedStrings.size(); i++) {
        printf ("%s",parsedStrings.at(i));
        cout << endl;
    }
    //string str = "ls -a;echo "hello;world" && mkdir test || echo world;git status";
    return 0;
}