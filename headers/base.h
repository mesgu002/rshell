#ifndef BASE_H
#define BASE_H

#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <sys/wait.h>
#include <unistd.h>
#include <boost/tokenizer.hpp>

using namespace std;

class Base 
{
    public:
        Base() { }
        virtual void run(string command, bool &previous) = 0;
    
};


#endif