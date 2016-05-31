#ifndef TEST_H
#define TEST_H

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Test 
{
    public:
        Test() { }
        void run(string command, string path, bool &previous);
};

#endif