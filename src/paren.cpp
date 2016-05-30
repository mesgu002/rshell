#include "../headers/paren.h"

using namespace std;

void Paren::run(string command, bool &previous)
{
    Parser* parse = new Parser();
    parse->parse(command, previous);
}