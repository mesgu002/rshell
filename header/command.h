#ifndef __COMMAND_H
#define __COMMAND_H

#include <vector>
#include <string>
using namespace std;

class Command
{
	public:
		Command();
		virtual void execute( bool &previous, vector <string> &commands) = 0;
};

#endif
