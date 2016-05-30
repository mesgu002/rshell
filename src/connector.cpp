#ifndef CONNECTOR_CPP
#define CONNECTOR_CPP

#include "../header/connector.h"
#include <cstdlib>
#include <iostream>

using namespace std;

Connector :: Connector (Base* lhs, Base* rhs)
{
	left = lhs;
	right = rhs;
}

Connector :: Connector ()
{
	left = NULL;
	right = NULL;
}

bool Connector :: execute()
{
}

#endif
