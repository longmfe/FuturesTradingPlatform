#include "Message.h"
#include <string>
#include <list>
#include "StringUtil.h"
#include "MsgType.h"
using namespace std;

class RspIndexMsg:Message{
public:
	
	RspIndexMsg(string recvmsg){
		split = ",";
		splitField = ":";

		StringUtil su;
		MsgID = MsgType::RspIndex;
		recvL = su.stringAnalysis(recvmsg, MsgType::Split);

		ReqID = atoi(getStrFromL(1).c_str());
		IndexMsg = getStrFromL(2);
		Flag = getStrFromL(3);

		RangeBegin = 0;
		RangeEnd = 0;
		DateBegin = 0;
		DateEnd = 0;

		//valStrL = su.stringAnalysis(getStrFromL(5), MsgType::Split);
		valStrL = su.stringAnalysis(getStrFromL(5), split);
		valuesL = valStrL;

	}
	RspIndexMsg(int reqid, string indexmsg, string flag, string rengeLen, string rangeDate, list<string> values){
	}
	~RspIndexMsg(){
	}

	bool getIsLast(){
		return strcmp(Flag.c_str(),MsgType::IsLastFlag.c_str()) == 0?true:false;
	}
	bool getIsError(){
		return strcmp(Flag.c_str(),MsgType::IsLastFlag.c_str()) == 0|| strcmp(Flag.c_str(),MsgType::NotLastFlag.c_str()) == 0?false:true;
	}

	list<string> getValues(){
	    return valuesL;
	}

	int getReqID(){
        return ReqID;
    }
	string getDateRange(){
        return DateBegin + "-" + DateEnd;
    }
	int getRangeBegin(){
        return RangeBegin;
    }
	int getRangeEnd(){
        return RangeEnd;
    }

	int getDateBegin(){
        return DateBegin;
    }
	int getDateEnd(){
        return DateEnd;
    }

	string getIndexMsg(){
		return IndexMsg;
	}

	virtual string getStringToSend(){
		return "";
	}

	virtual string getMsgID(){
		return MsgID;
	}

private :

	string split;
	string splitField;
	string MsgID;
	int ReqID;
	string IndexMsg;
	string Flag;

	string RanggLen;
	string RangeDate;

	int RangeBegin;
	int RangeEnd;
	int DateBegin;
	int DateEnd;

	list<string> valuesL;
	list<string> valStrL;

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