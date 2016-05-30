#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include "or.h"
#include "and.h"
#include "executable.h"

using namespace std;

class Parser {
    public:
        Parser() { }
        void parse(string command);
};

#endif