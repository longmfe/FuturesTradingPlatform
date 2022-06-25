#include "stdafx.h"
#include "SocketDama.h"
#include <WinSock2.h>
#include <stdio.h>
#include <iostream>
#include "StringUtil.h"
#include "MsgType.h"
#include "RspLoginMsg.h"
#include "ReqLoginMsg.h"
#include "RspMdMsg.h"
#include "RspIndexMsg.h"
#include "GlobalDataManage.h"
#include "ReqIndexUnit.h"
#include "ReqMdUnit.h"
#include "ReqIndexMsg.h"
#include "ReqMdMsg.h"
#include "Lock.h"
#include <map>
#include <cstring>
using namespace std;

HANDLE hThreadM ;
SOCKET sc;
string ClientIDD;
bool bLoginSucc;
map<int, string> ReqIDIndexMap;
string ClientID;
list<int> ReqIDMdL;
CLock lock;
int ReqSeq = 100;
bool  bReqMarket = true;
ofstream of;
ofstream ofmd;

extern RequestStore reqStore;

//产生不重复的请求
int GenerateReqID(){
    lock.lock();
    int tmp = ReqSeq++;
    lock.unlock();
    return tmp;
}

void sendMsg(string msg){
	char tmp[256]={0};
	strncpy(tmp, msg.c_str(), sizeof(tmp));
	tmp[strlen(msg.c_str())] = 13;
	tmp[strlen(msg.c_str()) + 1] = 10;
	send(sc, tmp, msg.length() + 2, 0);
}

int ReqIndex(){ //AlgoTSEng* al
    //reqid 内部产生
    list<ReqIndexUnit> reqL = reqStore.getReqIndexUnits();
	for(list<ReqIndexUnit>::iterator it = reqL.begin() ; it != reqL.end(); it++){
        int reqid = GenerateReqID();
        ReqIndexUnit reqi = *it;
        ReqIndexMsg req(ClientIDD,reqid,  reqi.getType(), reqi.getIndexName(), reqi.getArgs(), reqi.getEndOfDate(), reqi.getLen());
        sendMsg(req.getStringToSend());
		ReqIDIndexMap.insert(map<int, string> :: value_type(reqid, reqi.getExIndexName()));
    }
    return 1;
}

int ReqMarketData(){ //AlgoTSEng* al
	if(bReqMarket){
		bReqMarket = false;
		list<ReqMdUnit> reqL = reqStore.getReqMdUnits();
		for(list<ReqMdUnit>::iterator it = reqL.begin() ; it != reqL.end(); it++){
			int reqid = GenerateReqID();
			ReqMdUnit reqi = *it;
			ReqMdMsg req(ClientIDD,reqid,  reqi.getType(), reqi.getConsL(), reqi.getDates(), reqi.getTickInt(), reqi.getDayInt());
			string reqStr=req.getStringToSend();
			sendMsg(reqStr);
			cout<<reqStr<<endl;
			ReqIDMdL.push_back(reqid);
		}
		return 1;
	}

	return 1;
}

void ReqCancelMarketData(int reqid){
    list<string> cons;
	cons.push_back("default");
    list<string> dates;
    dates.push_back("default");
	ReqMdMsg req(ClientIDD, reqid, 2, cons, dates, 0, 0);
    sendMsg(req.getStringToSend());
}

void parMsg(string& recvmsg, AlgoManager* al){
	StringUtil su;
	list<string> recvL = su.stringAnalysis(recvmsg, MsgType::Split);
	string head = recvL.front();

	if(strcmp(head.c_str(), MsgType::RspLogin.c_str()) == 0){
		cout<<"RspLogin!!"<<endl;

		RspLoginMsg rsp (recvmsg);
		ClientIDD = rsp.getUserID();
		if(rsp.getStatus()){
			bLoginSucc = true;
			ReqIndex(); //al
			if(ReqIDIndexMap.size() == 0){
				ReqMarketData(); //al
			}
			//login successfull				 
		}
		else{
			//login fail
		}
	}
	else if(strcmp(head.c_str(), MsgType::RspMarketData.c_str()) == 0){
		cout<<"RspMarketData!!"<<endl;

        RspMdMsg rsp(recvmsg);

		if(strcmp(rsp.getStatus().c_str(),MsgType::NotLastFlag.c_str()) == 0){
            //回调 process
			if(rsp.getDate().find("R",0)!= string::npos)
	        {
		          cout<< "exception" <<endl;
		          cout<< rsp.getDate() <<endl;
				  cout << recvmsg << endl;
	        }
	        else if(rsp.getDate().length() !=8 )
	        {
	              cout<< "exception" <<endl;
				  cout << recvmsg << endl;		       
	        }
	        else if(rsp.getTime().find("R",0)!= string::npos)
	        {
	              cout<< "exception" <<endl;
		          cout<< rsp.getTime() << endl;
				  cout << recvmsg << endl;		        
	        }
			al->OnMarketData(rsp.getConName(), rsp.getLastPrice(), rsp.getLastVol(), 
                                       rsp.getAskPrice(), rsp.getAskVol(), rsp.getBidPrice(), rsp.getBidVol(), 
                                       rsp.getOpenI(), rsp.getTradeVol(), rsp.getDate(), rsp.getTime());

        }
        else if(strcmp(rsp.getStatus().c_str(),MsgType::IsLastFlag.c_str()) == 0){
			//al->getAlogEng(rsp.getConName())->pGlobalDm->finishedOneDayWork(false, rsp.getDate() + "---" + rsp.getTime(), rsp.getConName());
			al->finishTheWork(false, rsp.getDate() + "---" + rsp.getTime());
		}
        else{
                
        }

		//ofmd << rsp.getAskPrice() << rsp.getAskVol()<<endl;
    }
	/*else if(strcmp(head.c_str(), MsgType::RspIndex.c_str()) == 0){
		RspIndexMsg rsp(recvmsg);
            
		//添加数据
		if(!rsp.getIsError()){                
			list<string> valL = rsp.getValues();
			if(ReqIDIndexMap.find(rsp.getReqID()) != ReqIDIndexMap.end()){
				IndexUnit* iu =  al->pGlobalDm->getIndexMana()->getIndexUnit(ReqIDIndexMap.find(rsp.getReqID())->second); 
                    
				int i = 0;
				for(list<string>::iterator it =  valL.begin(); it != valL.end(); it++){
					IndexDetailUnit  idu(rsp.getDateRange(), "", rsp.getRangeBegin() + i, atof((*it).c_str()));
					iu->addIndexDetails(idu);
					i ++;
					of << *it << endl;
				}                
			}               
		}
        //并启动发送行情请求
        if(rsp.getIsLast() || rsp.getIsError()){
			if(ReqIDIndexMap.find(rsp.getReqID()) != ReqIDIndexMap.end()){
                ReqIDIndexMap.erase(ReqIDIndexMap.find(rsp.getReqID()));
            }
            if(ReqIDIndexMap.size() == 0){
                ReqMarketData(al);
            }
        }
    }
    else if (ReqIDIndexMap.size() == 0){
        ReqMarketData(al);  
    }*/		    
}

DWORD WINAPI ThreadDealSocket(void* ptr)
{
	cout << "ThreadDealSocket starting!";
	//3.和服务器进行通信，接收或是发送数据
	AlgoManager* al = (AlgoManager*)ptr;
	char recvBuf[256] = {0};
	int sizeofrecvbuf = sizeof(recvBuf);

	//登陆
	ReqLoginMsg req(ClientID);
	sendMsg(req.getStringToSend());
	if(SOCKET_ERROR == WSAGetLastError()){
		cout << "Error" << endl;
	}
	
	string recvmsg = "";
	bool flag = true;
	while(SOCKET_ERROR != recv(sc,recvBuf,sizeofrecvbuf - 1,0) )
	{
		for(int i = 0; i < sizeofrecvbuf - 1; i ++){			
			 if(recvBuf[i] == 10 || recvBuf[i] == 0){
				 if(recvmsg.find("Rs",3)!= string::npos)
				{
					cout<< "exception" <<endl;
					cout << recvmsg << endl;		        
				}
				parMsg(recvmsg, al);
				recvmsg = "";
			 }
			 else{
				recvmsg += recvBuf[i];
			 }			
		 }		
		 memset(recvBuf, 0, sizeof(recvBuf));
	}
	return 0;
}

SocketDama::SocketDama(AlgoManager* pAl){
	alMag = pAl;
	WORD wVersion=MAKEWORD(2,0);
	WSADATA wsData;
	of.open("./out.txt",ios::trunc); 
	ofmd.open("./md.txt",ios::trunc); 
	int nResult= WSAStartup(wVersion,&wsData);
	if(nResult !=0)
	{
		//错误处理
	}
	//1.创建套接字
    sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sc = sockClient;
    if(INVALID_SOCKET == WSAGetLastError()){
		cout << "Error_INVALID_SOCKET" << endl;
	} 
	else{	
	}
}

SocketDama::~SocketDama(){
	//4.关闭套接字的资源，以及卸载套接字动态链接库
    closesocket(sockClient);
    WSACleanup();
	//delete m_pAl;
	delete alMag;
}

//异步出来
bool SocketDama::Clientservice(){
	//2.向服务器发送连接请求
    SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr = inet_addr(serverIP.c_str());
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(serverport);
    int res = connect(sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
	if(0 == res){
		cout << "connect successfull" << endl;
		//Market data thread
		::CreateThread(NULL,0,ThreadDealSocket,(void *)alMag,0,nullptr);
	} 
	return true;  
}

bool SocketDama::CancelMd(){
	for(list<int>::iterator it =  ReqIDMdL.begin(); it != ReqIDMdL.end(); it++){
        int reqid = *it;
        ReqCancelMarketData(reqid);
    }
	return true;
}

void SocketDama::setClientID(string clientid){
	ClientID = clientid;
}