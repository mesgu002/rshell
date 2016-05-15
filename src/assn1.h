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
        bool executed;          //did it execute?
        bool hasBeenExecuted;   //whether it has been executed yet
        string arguement;       //command
    
    public:
        Base() { }
        virtual void execute(int x) = 0;
        virtual bool isOr() = 0;
        virtual bool isAnd() = 0;
        virtual void setChildExecuted(bool a, int b) = 0;
        virtual void setChildBeenExecuted(bool a, int b) = 0;
        bool getExecuted();
        void setExecuted(bool x);
        bool gethasBeenExecuted();
        void sethasBeenExecuted(bool x);
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
        void execute(int x);
        bool isOr();
        bool isAnd();
        void setChildBeenExecuted(bool b, int a);
        void setChildExecuted(bool a, int b);
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
        void execute(int x);
        bool isOr();
        bool isAnd();
        void setChildBeenExecuted(bool b, int a);
        void setChildExecuted(bool a, int b);
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
        void execute(int x);
        bool isOr();
        bool isAnd();
        void setChildBeenExecuted(bool b, int a);
        void setChildExecuted(bool a, int b);
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
        void execute(int x);
        bool isOr();
        bool isAnd();
        void setChildBeenExecuted(bool a, int b);
        void setChildExecuted(bool a, int b);
};

#endif