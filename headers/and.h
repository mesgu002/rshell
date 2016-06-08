#ifndef AND_H
#define AND_H

#include "base.h"
#include "test.h"
#include <boost/tokenizer.hpp>

using namespace std;

class And : public Base 
{
    private:
        Base* cmd;
    public:
        And(Base* temp) 
        {
            cmd = temp;
        }
        void run(string command, bool &previous);
};

#endif