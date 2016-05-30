#include "../headers/and.h"

using namespace std;
using namespace boost;

void And::run(string command, bool &previous)
{
    if (!previous)
    {
        previous = false;
    }
    else
    {
        this->cmd->run(command, previous);
    }
}