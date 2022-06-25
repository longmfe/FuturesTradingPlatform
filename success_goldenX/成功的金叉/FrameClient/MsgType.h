#ifndef MSGTYPE_H
#define MSGTYPE_H
#include <string>
using namespace std;

namespace MsgType{
	static const string ReqLogin = "ReqL";
	static const string RspLogin = "RspL";
    
    static const string ReqReconn = "ReqRc";
    
    static const string ReqMarketData = "ReqM";
    static const string  RspMarketData = "RspM";
    
    static const string ReqIndex = "ReqI";
    static const string RspIndex = "RspI";
    
    static const string HeartBeat = "HeartBeat";
    static const string ReqLogout = "ReqLogout";
    
    static const string Split = "`";
    static const string OK = "ok";
    static const string NotOK = "fk";
    static const string IsLastFlag = "0";
    static const string NotLastFlag = "1";
    static const string ErrorFlag = "2";

};

#endif