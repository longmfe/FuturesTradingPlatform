#include "Message.h"
#include <string>
#include <list>
#include "StringUtil.h"
#include "MsgType.h"
using namespace std;

class RspMdMsg:Message{
public:
	static const int split;
	
	RspMdMsg(string recvmsg){
		MsgID = MsgType::RspMarketData;

		StringUtil su;
		recvL = su.stringAnalysis(recvmsg, MsgType::Split);

		ConName = getStrFromL(1);
		Status = getStrFromL(2);

		message = getStrFromL(3);
		Date = getStrFromL(4);
		Time = getStrFromL(5);

		LastPrice = atof(getStrFromL(6).c_str());
        LastVol = atof(getStrFromL(7).c_str());
        AskPrice = atof(getStrFromL(8).c_str());
        AskVol = atof(getStrFromL(9).c_str());
        BidPrice = atof(getStrFromL(10).c_str());
        BidVol = atof(getStrFromL(11).c_str());
        OpenI = atof(getStrFromL(12).c_str());
        TradeVol = atof(getStrFromL(13).c_str());
        Other = getStrFromL(14);


	}
	RspMdMsg(string conname, string status, string message, 
		     string date, string time, double lp, double lv,
			 double askp, double askv, double bidp, double bidv,
			 double openi, double tradevol, string other);
	~RspMdMsg(){
	}

	
	string& getConName(){
		return ConName;
	}
	string& getStatus(){
		return Status;
	}
	string& getBackMessage(){
		return message;
	}
	string& getDate(){
		return Date;
	}
	string& getTime(){
		return Time;
	}
	string& getOther(){
		return Other;
	}
	double& getLastPrice(){
		return LastPrice;
	}
	double& getLastVol(){
        return LastVol;
    }
    
    double& getAskPrice(){
        return AskPrice;
    }
    
    double& getAskVol(){
        return AskVol;
    }
    
    double& getBidPrice(){
        return BidPrice;
    }
    
    double& getBidVol(){
        return BidVol;
    }
    
    double& getOpenI(){
        return OpenI;
    }
    
    double& getTradeVol(){
        return TradeVol;
    }

	virtual string getStringToSend(){
		return "";
	}
	virtual string getMsgID(){
		return  MsgID;
	}
private :
	string MsgID;
    string ConName;
    string Status;
    string message;
    string Date;
    string Time;
    double LastPrice;
    double LastVol;
    double AskPrice;
    double AskVol;
    double BidPrice;
    double BidVol;
    double OpenI;
    double TradeVol;
    string Other;
        
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