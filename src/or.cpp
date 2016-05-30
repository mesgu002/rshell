#include "../headers/or.h"

using namespace std;
using namespace boost;

void Or::run(string command, bool &previous)
{
    if (previous)
    {
        previous = true;
    }
    else {
        this->cmd->run(command, previous);
    }
}