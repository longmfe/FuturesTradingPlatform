#ifndef MESSAGE_H
#define MESSAGE_H
#include <string>
using namespace std;

class Message{
public:
	virtual string getStringToSend() = 0;
	virtual string getMsgID() = 0;

	Message(){
	};

	~Message(){
	}
};
#endif