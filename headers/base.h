#ifndef BASE_H
#define BASE_H

#include <iostream>

using namespace std;

class Base 
{
    public:
        Base() { }
        virtual void run(string command, bool &previous) = 0;
    
};


#endif