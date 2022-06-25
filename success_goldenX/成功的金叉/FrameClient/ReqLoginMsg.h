#include "Message.h"
#include <string>
#include <list>
#include "MsgType.h"
using namespace std;

class ReqLoginMsg:Message{
public:
	ReqLoginMsg(string userid){
		MsgID = MsgType::ReqLogin;
		userID = userid;
	}
	ReqLoginMsg(string msg, string flag){
	}
	~ReqLoginMsg(){
	}
	virtual string getStringToSend(){
		char strs[12];
		memset(strs, 0 , sizeof(strs));

		string res = "";

		res += MsgID + MsgType::Split + userID;

		return res;

	}
	virtual string getMsgID(){
		return MsgID;
	}

private :
	string MsgID;
	string userID;
};
