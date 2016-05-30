#ifndef OR_H
#define OR_H

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
#include "base.h"

using namespace std;

class Or : public Base {
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