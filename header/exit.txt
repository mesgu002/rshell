#ifndef EXIT_H
#define EXIT_H
#include "Command.h"
class Exit : public Command {
	public:
		Exit();
		void run(bool &previous, vector<string> &commands);
};

#endif // EXIT_H