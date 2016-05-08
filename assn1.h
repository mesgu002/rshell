#include <iostream>

using namespace std;

class Base {
    private:
        bool executed;
        string name;
        string arguement;
    
    public:
        Base() { }
        virtual void execute() = 0;
        bool getExecuted();
        void setExecuted(bool x);
        string getName();
        void setName(string x);
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
};

class Executable:public Base
{
    private:
        Base* child;
        
    public:
        Executable(Base* x)
        {
            child = x;
        }
        void execute();
};