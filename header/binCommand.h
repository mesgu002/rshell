#ifndef BINCOMMAND_H
#define BINCOMMAND_H
#include "Command.h"
class BinCommand : public Command {
	public:
		BinCommand();
		void run(bool &previous, vector<string> &commands);
};

#endif // BINCOMMAND_H
