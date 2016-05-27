#ifndef SEMICOLON_H
#define SEMICOLON_H
#include "Connector.h"
class SemiColon : public Connector {
	public:
		SemiColon();
		~SemiColon();
		void run(bool &previous, vector<string> &commands);
};

#endif // SEMICOLON_H
