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

bool Base::gethasBeenExecuted()
{
    return hasBeenExecuted;
}

void Base::sethasBeenExecuted(bool x)
{
    hasBeenExecuted = x;
}

// string Base::getName() 
// {
//     return name;
// }

// void Base::setName(string x)
// {
//     name = x;
// }

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
    if(left->getExecuted() == false) 
    {
        right->execute();
    }
    else
    {
        right->setExecuted(false);
        right->sethasBeenExecuted(true);
    }
}

void And::execute() 
{
    left->execute();
    if(left->getExecuted()) 
    {
        right->execute();
    }
    else
    {
        right->setExecuted(false);
        right->sethasBeenExecuted(true);
    }
}

void Comment::execute()
{
    child->setArguement("");
    child->execute();
}

void Executable::execute()
{
    if(!this->gethasBeenExecuted())
    {
        if(system(this->getArguement().c_str()) != 0) {
            this->setExecuted(false);
        }
        else {
            this->setExecuted(true);
        }
        this->sethasBeenExecuted(true);
    }
}