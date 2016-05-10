#include "assn1.h"

using namespace std;

bool Base::getExecuted() 
{
    return executed;
}

void Base::setExecuted(bool x) 
{
    executed = x;
}

string Base::getName() 
{
    return name;
}

void Base::setName(string x)
{
    name = x;
}

void Base::setArguement(string x)
{
    arguement = x;
}

string Base::getArguement()
{
    return arguement;
} 
        
void Or::execute() 
{
    left->execute();
    if(!left->getExecuted()) 
    {
        right->execute();
    }
}

void And::execute() 
{
    left->execute();
    if(left->getExecuted()) 
    {
        right->execute();
    }
}

void Comment::execute()
{
    child->setArguement("");
    child->execute();
}

void Executable::execute()
{
    //syscalls
}