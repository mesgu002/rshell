#ifndef __CONNECTOR_H
#define __CONNECTOR_H

#include "sysCommand.h"
#include "exit.h"
#include "test.h"
#include "parenthesis.h"
#include "command.h"
#include <cstdlib>

using namespace std;

class Connector : public Base
{
	protected:
	
		Command* Command;
	/*
	Base* left; //Used to access the left child of a connector
	Base* right; //Used to access the right child of a connector
	*/
	public:
	
		Connector();
		~Connector();
		virtual void execute (bool &previous, vector <string> &commands);
	/*
	Connector ( Base* lhs , Base* rhs );
	*/
};

#endif

