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
        
void Or::execute() 
{
    if(!left->gethasBeenExecuted())
    {
        left->execute();
    }
    if(left->getExecuted() == false) 
    {
        right->execute();
    }
    else
    {
        right->setExecuted(false);
    }
}

void Or::multihasBeenExecuted(bool a, bool b)
{
    left->sethasBeenExecuted(a);
    right->sethasBeenExecuted(b);
}

void Or::multiExecuted()
{
    if(left->getExecuted())
    {
        right->setExecuted(false);
    }
}

void And::execute() 
{
    if(!left->gethasBeenExecuted())
    {
    left->execute();
    }
    if(left->getExecuted()) 
    {
        right->execute();
    }
    else
    {
        right->setExecuted(false);
        //right->sethasBeenExecuted(true);
    }
}

void And::multihasBeenExecuted(bool a, bool b)
{
    left->sethasBeenExecuted(a);
    right->sethasBeenExecuted(b);
}

void And::multiExecuted()
{
    if(!left->getExecuted()) {
        right->setExecuted(false);
    }
}

void Comment::execute()
{
    child->setArguement("");
    child->execute();
}

void Comment::multihasBeenExecuted(bool a, bool b)
{ a = b; b = a; }

void Comment::multiExecuted()
{ }

void Executable::execute()
{
    string temp1 = this->getArguement();
    vector<string> mytok;
    char_separator<char> space(" ");
    tokenizer<char_separator<char> > toks(temp1, space);
    //cout << temp1 << endl;
    for (tokenizer<char_separator<char> >::iterator it = toks.begin(); it 
        != toks.end(); it++)
	{
		mytok.push_back(*it);
	}
    char** temp = new char*[mytok.size() + 1];
    for (unsigned i = 0; i < mytok.size(); ++i) {
        temp[i] = new char[mytok.at(i).size()];
        strcpy(temp[i], mytok.at(i).c_str());
    }
    if (!this->gethasBeenExecuted())
    {
        if (execvp(temp[0], temp) == -1) {
            perror("execvp");
            this->setExecuted(false);
            exit(1);
        }
    }
}

void Executable::multihasBeenExecuted(bool a, bool b)
{ a = b; b = a; }

void Executable::multiExecuted()
{ }
