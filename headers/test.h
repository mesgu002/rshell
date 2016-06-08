#ifndef TEST_H
#define TEST_H

#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

using namespace std;

class Test 
{
    public:
        Test() { }
        void run(string command, string path, bool &previous);
};

#endif