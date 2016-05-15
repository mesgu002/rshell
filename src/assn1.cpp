#include "assn1.h"

using namespace std;
using namespace boost;

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
    if(!left->gethasBeenExecuted() && x == 0)
    {
        left->execute(x);
    }
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
    if(!left->gethasBeenExecuted() && x == 0)
    {
        left->execute(x);
    }
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

void Comment::setChildBeenExecuted(bool x, int y)
{ if (x) { ++y; } }

void Comment::setChildExecuted(bool x, int y)
{ if (x) { ++y; } }

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
    vector<string> mytok;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > toks(temp1, space);
    for (tokenizer<char_separator<char> >::iterator it = toks.begin(); it 
        != toks.end(); it++)
	{
		mytok.push_back(*it);
	}
    char** temp = new char*[mytok.size() + 1];
    unsigned i = 0;
    for (; i < mytok.size(); ++i) {
        temp[i] = new char[mytok.at(i).size()];
        strcpy(temp[i], mytok.at(i).c_str());
    }
    temp[i] = NULL;
    if (!this->gethasBeenExecuted())
    {
        if (execvp(temp[0], temp) == -1) {
            perror("execvp");
            abort();
        }
    }
}

void Executable::setChildBeenExecuted(bool x, int y)
{ if (x) { ++y; } }

void Executable::setChildExecuted(bool x, int y)
{ if (x) { ++y; } }