#ifndef EXECUTABLE_H
#define EXECUTABLE_H

#include <cstring>
#include <stdio.h>
#include <cerrno>
#include <sys/wait.h>
#include <boost/tokenizer.hpp>
#include "base.h"

using namespace std;

class Executable : public Base 
{
    public:
        Executable() { }
        void run(string command, bool &previous);
};

#endif