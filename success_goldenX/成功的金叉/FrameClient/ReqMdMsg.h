#include "Message.h"
#include <string>
#include <list>
#include "MsgType.h"
using namespace std;

class ReqMdMsg:Message{
public:
	static const int get = 1;
	static const int cancel = 2;



	ReqMdMsg(string recvmsg){
	}

	ReqMdMsg(string clientid, int reqid, int type, list<string> cons, list<string> dates, int tickint, int dayint){
		splitL = ",";
		MsgID = MsgType::ReqMarketData;
		ClientID = clientid;
		ReqID = reqid;
		Type = type;
		for(int i = 0 ; i < cons.size(); ){
			conL.push_back(cons.front());
			cons.pop_front();
		}

		for(int i = 0 ; i < dates.size(); ){
			dateL.push_back(dates.front());
			dates.pop_front();
		}

		TickInt = tickint;
		DayInt = dayint;
	}
	~ReqMdMsg(){
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

		int orignalsize  = conL.size();
		for(int i = 0 ; i < orignalsize-1 ; i++){
			res += conL.front() + splitL;
			conL.pop_front();
        }
		cout<< res << endl;
        res += conL.front() + MsgType::Split;
		cout<< res << endl;
		for(int i = 0 ; i < dateL.size() - 1; i++){
			res += dateL.front() + splitL;
			dateL.pop_front();
        }
        res += dateL.front() + MsgType::Split;

		memset(strs, 0 , sizeof(strs));
		_itoa(TickInt, strs, 10);
		res += strs + MsgType::Split;

		memset(strs, 0 , sizeof(strs));
		_itoa(DayInt, strs, 10);
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
	list<string> conL;
	list<string> dateL;
	int TickInt;
	int DayInt;

	string splitL;
};