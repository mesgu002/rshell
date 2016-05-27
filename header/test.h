#ifndef TEST_H
#define TEST_H
#include "Command.h"
class Test : public Command {
    public:
        Test();
        void run(bool &previous, vector<string> &commands);
};

#endif // TEST_H
