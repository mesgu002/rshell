#include "assn1.h"

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

void Base::setArguement(string x)
{
    arguement = x;
}

string Base::getArguement()
{
    return arguement;
} 
        
void Or::execute(int x) 
{
    //Check to see if left has been executed yet and if x is 0
    if(!left->gethasBeenExecuted() && x == 0)
    {
        left->execute(x);
    }
    //Check to see if right has been executed yet, if x is 1 and if left
    //didn't execute correctly
    else if (!right->gethasBeenExecuted() && x == 1 && !left->getExecuted())
    {
        right->execute(x);
    }
    exit(0);
}

bool Or::isOr() 
{
    return true;
}

bool Or::isAnd() 
{
    return false;
}

void Or::setChildBeenExecuted(bool x, int y)
{
    if (y == 0)
    {
        left->sethasBeenExecuted(x);
    }
    else
    {
        right->sethasBeenExecuted(x);
    }
}

void Or::setChildExecuted(bool x, int y)
{
    if (y == 0)
    {
        left->setExecuted(x);
    }
    else
    {
        right->setExecuted(x);
    }
}

void And::execute(int x) 
{
    //Check to see if left has been executed yet and if x is 0
    if(!left->gethasBeenExecuted() && x == 0)
    {
        left->execute(x);
    }
    //Check to see if right has been executed yet, if x is 1 and if left
    //execute correctly
    else if (!right->gethasBeenExecuted() && x == 1 && left->getExecuted())
    {
        right->execute(x);
    }
    exit(0);
}

bool And::isOr() 
{
    return false;
}

bool And::isAnd() 
{
    return true;
}

void And::setChildBeenExecuted(bool x, int y)
{
    if (y == 0)
    {
        left->sethasBeenExecuted(x);
    }
    else
    {
        right->sethasBeenExecuted(x);
    }
}

void And::setChildExecuted(bool x, int y)
{
    if (y == 0)
    {
        left->setExecuted(x);
    }
    else
    {
        right->setExecuted(x);
    }
}

bool Comment::isOr() 
{
    return false;
}

bool Comment::isAnd() 
{
    return false;
}

void Comment::execute(int x)
{
    ++x;
    child->setArguement("");
}

bool Executable::isOr() 
{
    return false;
}

bool Executable::isAnd() 
{
    return false;
}

void Executable::execute(int x)
{
    ++x;
    string temp1 = this->getArguement();
    
    //tokenize the string at spaces
    vector<string> mytok;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > toks(temp1, space);
    for (tokenizer<char_separator<char> >::iterator it = toks.begin(); it 
        != toks.end(); it++)
	{
		mytok.push_back(*it);
	}
	
    char** temp = new char*[mytok.size() + 1];  //convert tokens to Char**
    unsigned i = 0;
    for (; i < mytok.size(); ++i) {
        temp[i] = new char[mytok.at(i).size()];
        strcpy(temp[i], mytok.at(i).c_str());   //copy tokens to Char**
    }
    temp[i] = NULL;
    if (!this->gethasBeenExecuted())            //check to see if 'this' hasn't
    {                                           //been executed
        if (execvp(temp[0], temp) == -1) {
            perror("execvp");                   //execvp() failed
            abort();                            //trigger WIFSIGNALED
        }
    }
}

//These functions aren't needed/don't do anything
void Executable::setChildBeenExecuted(bool x, int y)
{ if (x) { ++y; } }

void Executable::setChildExecuted(bool x, int y)
{ if (x) { ++y; } }

void Comment::setChildBeenExecuted(bool x, int y)
{ if (x) { ++y; } }

void Comment::setChildExecuted(bool x, int y)
{ if (x) { ++y; } }