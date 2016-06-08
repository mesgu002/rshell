#ifndef OR_H
#define OR_H

#include "base.h"
#include "test.h"
#include <boost/tokenizer.hpp>

using namespace std;

class Or : public Base 
{
    private: 
        Base* cmd;
    public:
        Or(Base* temp) 
        { 
            cmd = temp;
        }
        void run(string command, bool &previous);
};

#endif