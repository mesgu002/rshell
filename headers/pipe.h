#ifndef PIPE_H
#define PIPE_H

#include "base.h"
#include "parser.h"

using namespace std;

class Pipe : public Base
{
	public:
		Pipe () {};

		void run (string command, bool &previous);

};
	






#endif
