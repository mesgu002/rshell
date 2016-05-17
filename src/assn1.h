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

using namespace std;

class Base 
{
    private:
        bool executed;          //did it execute correctly
        bool hasBeenExecuted;   //whether it has been executed yet
        string arguement;       //command
    
    public:
        Base() { }                      //Default constructor
        virtual void execute(int x) = 0;//execute commands
        virtual bool isOr() = 0;        //is 'this' an Or object
        virtual bool isAnd() = 0;       //is 'this' an And object
        virtual void setChildExecuted(bool a, int b) = 0;   //Change 'executed'
        //Change 'hasBeenExecuted'
        virtual void setChildBeenExecuted(bool a, int b) = 0;
        bool getExecuted();     //return 'executed'
        void setExecuted(bool x); //set 'executed'
        bool gethasBeenExecuted(); //return 'hasBeenExecuted'
        void sethasBeenExecuted(bool x); //set 'hasBeenExecuted'
        void setArguement(string x); //set 'arguement'
        string getArguement();      //get 'arguement'
};

class Or:public Base 
{
    private:
        Base* left;
        Base* right;
        
    public:
        Or(Base *x, Base* y)    //constructor
        {
            left = x;
            right = y;
        }
        void execute(int x);    //execute Or command
        bool isOr();            //return true
        bool isAnd();           //return false
        //change left(a=0) or right(a=1) child's 'executed' to b
        void setChildBeenExecuted(bool b, int a);
        //change left(a=0) or right(a=1) child's 'hasBeenExecuted' to b
        void setChildExecuted(bool a, int b);
};

class And:public Base 
{
    private:
        Base* left;
        Base* right;
        
    public:
        And(Base *x, Base* y)   //constructor
        {
            left = x;
            right = y;
        }
        void execute(int x);    //execute And command
        bool isOr();            //return false
        bool isAnd();           //return true
        //change left(a=0) or right(a=1) child's 'executed' to b
        void setChildBeenExecuted(bool b, int a);
        //change left(a=0) or right(a=1) child's 'hasBeenExecuted' to b
        void setChildExecuted(bool a, int b);
};

class Comment:public Base
{
    private:
        Base* child;
        
    public:
        Comment(Base* x)        //constructor
        {
            child = x;
        }
        //Rest of commands not needed, but need to be declared because they
        //are pure virtual in the base class
        void execute(int x);
        bool isOr();
        bool isAnd();
        void setChildBeenExecuted(bool b, int a);
        void setChildExecuted(bool a, int b);
};

class Executable:public Base
{
    public:
        Executable(string x, bool y, bool z)    //constructor
        {
            this->setArguement(x);
            this->setExecuted(y);
            this->sethasBeenExecuted(z);
        }
        void execute(int x);                    //execute Executable command
        bool isOr();                            //return false
        bool isAnd();                           //return false
        //Rest of commands not needed, but need to be declared because they
        //are pure virtual in the base class
        void setChildBeenExecuted(bool a, int b);
        void setChildExecuted(bool a, int b);
};

#endif