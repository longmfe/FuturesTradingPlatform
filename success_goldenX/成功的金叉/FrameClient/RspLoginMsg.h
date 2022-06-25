#include "Message.h"
#include <string>
#include <list>
#include "MsgType.h"
#include "StringUtil.h"
using namespace std;

class RspLoginMsg:Message{
public:
	RspLoginMsg(string recvmsg){
		split = ",";
		MsgID = MsgType::RspLogin;

		StringUtil su;
		recvL = su.stringAnalysis(recvmsg, MsgType::Split);

		userID = getStrFromL(1);
		EndOfdate = getStrFromL(2);
		Status = getStrFromL(3);
		Msg = getStrFromL(4);
		conL = su.stringAnalysis(getStrFromL(5), split);
	}
	RspLoginMsg(string useridd, string endOfdate, string status, string message, list<string> cons);
	~RspLoginMsg(){
	}

	virtual string getMsgID(){
		return MsgID;
	}
	string getUserID(){
		return userID;
	}
	string getEndOfdate(){
		return EndOfdate;
	}
	bool getStatus(){
		return strncmp(Status.c_str(), MsgType::OK.c_str(), 2) == 0;
	}
	string getMsg(){
		return Msg;
	}
	list<string> getContracts(){
		return conL;
	}

	virtual string getStringToSend(){
		return "";
	}
private :
	string MsgID;
	string userID;
	string EndOfdate;

	string Status;
	string Msg;
	list<string> conL;

	string split;

	list<string> recvL;
	string getStrFromL(int index){
		if(index > recvL.size()){
			return "";
		}
		list<string>::iterator iter = recvL.begin();
		for(int i = 0 ; i <= index && iter != recvL.end(); i ++){
			if(i == index){
				return *iter;
			}
			iter++;
		}

		return "";
	}
};