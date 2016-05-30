#ifndef EXECUTABLE_H
#define EXECUTABLE_H

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

class Executable : public Base {
    public:
        Executable() { }
        void run(string command, bool &previous);
};

#endif