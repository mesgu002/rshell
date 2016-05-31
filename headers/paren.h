#ifndef PAREN_H
#define PAREN_H

#include "base.h"
#include "parser.h"

using namespace std;

class Paren : public Base 
{
    public:
        Paren() { }
        void run(string command, bool &previous);
};

#endif