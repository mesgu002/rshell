#include "../headers/paren.h"

using namespace std;

void Paren::run(string command, bool &previous)
{
    //command should be combination of more than one command
    //recall parse to execute these first then program will move
    //on to the rest of the user input not in the parenthesis
    Parser* parse = new Parser();
    parse->parse(command, previous);
}