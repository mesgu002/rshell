#ifndef ASSN1_H
#define ASSN1_H

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

class Base {
    private:
        bool executed;
        bool hasBeenExecuted;
        string arguement;
    
    public:
        Base() { }
        virtual void execute() = 0;
        bool getExecuted();
        void setExecuted(bool x);
        bool gethasBeenExecuted();
        void sethasBeenExecuted(bool x);
        virtual void multihasBeenExecuted(bool x, bool y) = 0;
        virtual void multiExecuted() = 0;
        void setArguement(string x);
        string getArguement();
};

class Or:public Base 
{
    private:
        Base* left;
        Base* right;
        
    public:
        Or(Base *x, Base* y) 
        {
            left = x;
            right = y;
        }
        void execute();
        void multihasBeenExecuted(bool x, bool y);
        void multiExecuted();
};

class And:public Base 
{
    private:
        Base* left;
        Base* right;
        
    public:
        And(Base *x, Base* y) 
        {
            left = x;
            right = y;
        }
        void execute();
        void multihasBeenExecuted(bool x, bool y);
        void multiExecuted();
};

class Comment:public Base
{
    private:
        Base* child;
        
    public:
        Comment(Base* x)
        {
            child = x;
        }
        void execute();
        void multihasBeenExecuted(bool x, bool y);
        void multiExecuted();
};

class Executable:public Base
{
    public:
        Executable(string x, bool y, bool z)
        {
            this->setArguement(x);
            this->setExecuted(y);
            this->sethasBeenExecuted(z);
        }
        void execute();
        void multihasBeenExecuted(bool x, bool y);
        void multiExecuted();
};

#endif