#ifndef CONNECTOR_H
#define CONNECTOR_H
#include "BinCommand.h"
#include "Exit.h"
class Connector {
	protected:
		Command* command;
	public:
		Connector();
		~Connector();
		virtual void run(bool &previous, vector<string> &commands) = 0;
};

#endif // CONNECTOR_H
