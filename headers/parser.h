#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include <iostream>
#include "or.h"
#include "and.h"
#include "executable.h"
#include "test.h"
#include "paren.h"

using namespace std;

class Parser 
{
    public:
        Parser() { }
        void parse(string command, bool &previous);
};

#endif