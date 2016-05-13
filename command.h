#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>

using namespace std;

class Base //Base Class
{	
	public:
		virtual void execute() = 0; //pure virtual function
};

class Command : public Base  // The Command Class inherits from the Base Class
{
	public:
		void execute() {};
};

class Connector : public Command // The Connector Class inherits from both the Command class and the Base Class.
{
	private:
		Command left;
		Command right;
	
	public:
		virtual void execute() = 0;
};

#endif
