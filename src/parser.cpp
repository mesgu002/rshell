#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>

#include "../header/Command.h"
#include "../header/BinCommand.h"
#include "../header/Exit.h"
#include "../header/Parenthesis.h"
#include "../header/Test.h"
#include "../header/Connector.h"
#include "../header/LogicalOr.h"
#include "../header/LogicalAnd.h"
#include "../header/SemiColon.h"
#include "../header/Parser.h"


using namespace std;

Parser::Parser() { }

void Parser::parse(string input, bool &previous) {

    vector<string> str_vctr;
    queue<string> tokens;
    string temp;
    string new_str;
    const string a = "&&";
    const string o = "||";
    const string s = ";";
    const string h = "#";
    const string t = "[";
    const string tend = "]";
	const string test = "test";
    const string p = "(";
    const string pend = ")";

    //--------------------------------------------------------------
    // Tokenizing the user string inputs into small usable pieces
    //  and putting the data into a queue to sort through later on
    //--------------------------------------------------------------
	int num_paren = 0;
	for (string::iterator p_test = input.begin(); p_test != input.end(); ++p_test) {
		if(*p_test == '(') {
			++num_paren;
		} else if(*p_test == ')') {
			--num_paren;
		}
	}
	if(num_paren != 0) {
		cout<<"Error: No Matching Parenthesis"<<endl;
		return;
	}

    for (string::iterator it = input.begin(); it != input.end(); ) {
        temp = *it;
        if (temp == ";" ) {
            tokens.push(temp);
            ++it;
        }
        else if (it == input.begin() && temp == "#") {
			tokens.push(temp);
			++it;
        }
        else if(*it == '(') {
        	tokens.push(temp);
        	++it;
        	int num_p = 1;
        	while(it != input.end()) {
				if(*it == '(') {
					++num_p;
				} else if(*it == ')') {
					--num_p;
				}
				if(!num_p) {
					++it;
					break;
				}

				temp = *it;
				new_str.append(temp);
				++it;
		 	}
		    if(num_p != 0) {
        		cout<<"Error: No Matching Parenthesis"<<endl;
        		/* for(int i = 0; i < num_p - 1; ++i) {
        			new_str.append(")");
        		}*/
        		previous = false;
        		return;
        	}
			tokens.push(new_str);
        	new_str.clear();
        	tokens.push(")");
        }
		else if(*it == '[') {
        	++it;
        	if(it != input.end() && *it == ' ') {
        	tokens.push(temp);
        	while(it != input.end() && *it != ']') {
				temp = *it;
				if(*it != ' ') {
					new_str.append(temp);
				}
				else if(!new_str.empty()) {
					tokens.push(new_str);
					new_str.clear();
				}
				++it;
        	}
        	if(it != input.end() && *it == ']') {
				temp = *it;
				tokens.push(temp);
				++it;
        	} else {
        		cout<<"Error: No Matching Bracket"<<endl;
        		//previous = false;
        		return;
        	}
        	new_str.clear();
        	}
        	else {
        		new_str.append(temp);
        	}
        }
		else if(*it == ')') {
			cout<<"Error: No Matching Parenthesis"<<endl;
			//previous = false;
			return;
		}
        else if (temp == "\"") {
            new_str.append(temp);
            ++it;
            temp = *it;
            while (temp != "\"" && it != input.end()) {
                if(temp == "\\") {
                    ++it;
                    if (it != input.end()) {
                        temp = *it;
                        if (temp != "\"") {
                         new_str.append(temp);
                         ++it;
                         temp = *it;
                        }
                    }
                }
               else {
                    new_str.append(temp);
                    ++it;
                    temp = *it;
                }
            }
            if ((*it == ' ') || (it == input.end())) {
                cout << "Error: No Matching Quotation" << endl;
            }
            else {
                new_str.append(temp);
                ++it;
                tokens.push(new_str);
                new_str.clear();
            }
        }
        else if (temp == "\'") {
            ++it;
            temp = *it;
            while (temp != "\'" && it != input.end()) {
                new_str.append(temp);
                ++it;
                temp = *it;
            }
            if (it == input.end()) {
                cout << "Error: No Matching Quotation" << endl;
            }
            else {
                ++it;
                tokens.push(new_str);
                new_str.clear();
            }
        }
        else if (temp == " ") {
            ++it;
            if (it != input.end()) {
				temp = *it;
				if (temp == "#") {
					tokens.push(temp);
					++it;
				}
            }
        }
        else {
           while (temp != "\"" && temp != "\'" && temp != " " &&
                    temp != ";" &&
                it != input.end()) {
                if (temp == "\\") {
                    ++it;
                }
                if (it != input.end()) {
                    temp = *it;
                    new_str.append(temp);
                    ++it;
                    temp = *it;
                }
            }
            tokens.push(new_str);
            new_str.clear();
        }

    }

    //-----------------------------------------------------------
    // Running the first command and no matter what (even if a
    //  connector exists). The first command that's run is
    //  sequence of tokens that is (are) not a semicolon
    //  or logical or/and. Our 'previous' bool variable holds whether
    //  or not the command successfully ran.
    //-----------------------------------------------------------
/*while(!tokens.empty()) {
	cout<<tokens.front()<<endl;
	tokens.pop();
}*/
	if(!tokens.empty()) {

		if(tokens.front() == test || tokens.front() == t) {
			tokens.pop();
			while(!tokens.empty() &&
				tokens.front() != tend &&
				tokens.front() != a &&
            	tokens.front() != o &&
            	tokens.front() != h &&
            	tokens.front() != s) {
					str_vctr.push_back(tokens.front());
					tokens.pop();
			}
			if(!tokens.empty() && tokens.front() == tend) {
				tokens.pop();
			}
			command = new Test();
			command->run(previous, str_vctr);

		}
		else if(tokens.front() == p) {
			tokens.pop();
			while(!tokens.empty() &&
				tokens.front() != pend &&
				tokens.front() != a &&
            	tokens.front() != o &&
            	tokens.front() != h &&
            	tokens.front() != s) {
				str_vctr.push_back(tokens.front());
				tokens.pop();
			}
			if(!tokens.empty() && tokens.front()  == pend) {
				tokens.pop();
			}
			command = new Parenthesis();
			command->run(previous, str_vctr);

		}
		else {
			while(!tokens.empty()) {

        	if (tokens.front() != a &&
            	tokens.front() != o &&
            	tokens.front() != h &&
            	tokens.front() != s) {

                	str_vctr.push_back(tokens.front());
                	tokens.pop();
        	}
        	else if (tokens.front() == h) {
            	while (!tokens.empty()) {
                	tokens.pop();
            	}
        	}
        	else {
            	break;
        	}
    	}

    	if (!str_vctr.empty()) {
        	command = new BinCommand();
        	command->run(previous, str_vctr);
    	}
    	}

    	str_vctr.clear();
	}
    //----------------------------------------------------------
    // Running the rest of the command depending in this loop
    //  as long as the queue is not empty. It looks for double
    //  quotes, &&, ||, or semi-colons. Once it finds one of these
    //  delimiters it creates a vector of usable strings until
    //  it sees another delimiter, then it breaks out of it's loop
    //  to get run by execvp.
    //----------------------------------------------------------

    while (!tokens.empty()) {

        if(tokens.front() == a) {

            connector = new LogicalAnd();
            tokens.pop();
            while(!tokens.empty()) {

                if(tokens.front() != a && tokens.front() != o &&
                   tokens.front() != s && tokens.front() != h) {

                    str_vctr.push_back(tokens.front());
                    tokens.pop();
                }
                else {
                    break;
                }
            }
        }
        else if(tokens.front() == o) {

            connector = new LogicalOr();
            tokens.pop();
            while(!tokens.empty()) {

                if(tokens.front() != a && tokens.front() != o &&
                   tokens.front() != s && tokens.front() != h) {

                    str_vctr.push_back(tokens.front());
                    tokens.pop();
                }
                else {
                    break;
                }
            }
        }
        else if(tokens.front() == s ) {

            connector = new SemiColon();
            tokens.pop();
            while(!tokens.empty()) {

                if(tokens.front() != a && tokens.front() != o &&
                   tokens.front() != s && tokens.front() != h) {

                    str_vctr.push_back(tokens.front());
                    tokens.pop();
                }
                else {
                    break;
                }
            }
        }
        else if (tokens.front() == h) { // if #, don't run anything
                                        // just empty queue
            while (!tokens.empty()) {
                tokens.pop();
            }
        }
        if(!str_vctr.empty()) {         // run last command if not empty
			connector->run(previous, str_vctr);
        }

        str_vctr.clear();
    }
}
