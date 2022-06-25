#include "Message.h"
#include <string>
#include <list>

using namespace std;

class ReqLogoutMsg:Message{
public:
	ReqLogoutMsg();
	~ReqLogoutMsg();

	virtual string getStringToSend();
	virtual string getMsgID();
private:
	string MsgID;
};