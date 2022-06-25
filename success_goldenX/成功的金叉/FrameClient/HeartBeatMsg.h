#include "Message.h"
#include <string>
#include "MsgType.h"

using namespace std;

class HeartBeatMsg:Message{
public:
	HeartBeatMsg(){
		MsgID = MsgType::HeartBeat;
	};
	~HeartBeatMsg(){
	};

	virtual string getStringToSend(){
		return MsgID;
	};
	virtual string getMsgID(){
		return MsgID;
	};
private:
	string MsgID;
};