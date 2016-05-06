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
        
        bool getExecuted() 
        {
            return executed;
        }
        
        void setExecuted(bool x) 
        {
            executed = x;
        }
        
        string getName() 
        {
            return name;
        }
        
        void setName(string x)
        {
            name = x;
        }
        
        void setArguement(string x) {
            arguement = x;
        }
        
        string getArguement() {
            return arguement;
        } 
        
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
        void execute() 
        {
            left->execute();
            if(!left->getExecuted) 
            {
                right->execute();
            }
        }
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
        void execute() 
        {
            left->execute();
            if(left->getExecuted) 
            {
                right->execute();
            }
        }
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
        void execute()
        {
            child->setArguement("");
            child->execute();
        }
        
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
        
        void execute()
        {
            //implement syscalls
        }
}

class Parser
{
    vector<string> Parse(string x) 
    {
        //implement parse with strtok 
        //return vector of separated x
    }
};

int main() 
{
    // vector<string>userInput;
    // string temp;
    // cout << "Enter command: " << endl;
    // getline(cin, temp);
    // userInput = Parse(temp);
    // for(int i = 0; i < userInput.size(); i++) 
    // {
    //        construct objects
    // }

    return 0;
}