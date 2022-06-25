#include "Message.h"
#include <string>
#include "MsgType.h"
#include <list>
#include "StringUtil.h"

using namespace std;

class ReqIndexMsg:Message{
public:
	static const int get = 1;
	static const int cancel = 2;



	ReqIndexMsg(string recvmsg){
		
	}

	ReqIndexMsg(string clientid, int reqid, int type, string indexname, string args, string endofdate, int len){
	    MsgID = MsgType::ReqIndex;
        ClientID = clientid;
        ReqID = reqid;
        Type = type;
        IndexName = indexname;
        Args = args;
        EndOfDate = endofdate;
        Length = len;
	}
	~ReqIndexMsg(){
	}

	virtual string getStringToSend(){
		char strs[12];
		memset(strs, 0 , sizeof(strs));

		string res = "";

		res += MsgID + MsgType::Split;
		res += ClientID + MsgType::Split;
		_itoa(ReqID, strs, 10);
		res += strs + MsgType::Split;

		memset(strs, 0 , sizeof(strs));
		_itoa(Type, strs, 10);
		res += strs + MsgType::Split;

		res += IndexName + MsgType::Split;
		res += Args + MsgType::Split;
		res += EndOfDate + MsgType::Split;

		memset(strs, 0 , sizeof(strs));
		_itoa(Length, strs, 10);
		res += strs;

		return res;
	}

	virtual string getMsgID(){
		return MsgID;
	}
private :
	string MsgID;
	string ClientID;
	int ReqID;
	int Type;
	string IndexName;
	string Args;
	string EndOfDate;
	int Length;
};