#include "../header/pipe.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>

using namespace std;

Pipe :: Pipe () {}

void Pipe :: run ( string commands, bool &previous)
{
	cout << "Pipe test..." << endl;
	previous = false;
}

