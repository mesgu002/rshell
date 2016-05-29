#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "base.h"

using namespace std;

class connector : public base
{
	protected:

	base* left; //Used to access the left child of a connector
	base* right; //Used to access the right child of a connector

	public:

	connector ( base* lhs , base* rhs );
	connector ();
	//virtual bool execute = 0; //execute function for derived classes.
};

#endif

