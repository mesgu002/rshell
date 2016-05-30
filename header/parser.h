#ifndef __PARSER_H
#define __PARSER_H

#include "command.h"
#include "connector.h"
#include <iostream>
#include <string>

using namespace std;

class Connector;

class Parser
{
	private:
		Command* command;
		Connector* connector;

	public:
		Parser();
		void parse ( string input, bool $previous);
};

#endif
