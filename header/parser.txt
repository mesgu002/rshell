#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include "Command.h"
#include "Connector.h"
using namespace std;

class Parser {

    private:
        Command* command;
        Connector* connector;

    public:
        Parser();
        void parse(string input);
};

#endif // PARSER_H