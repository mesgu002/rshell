#ifndef COMMAND_H
#define COMMAND_H
#include <vector>
#include <string>
using namespace std;
class Command {
	public:
		Command();
		virtual void run(bool &previous, vector<string> &commands) = 0;
};

#endif // COMMAND_H