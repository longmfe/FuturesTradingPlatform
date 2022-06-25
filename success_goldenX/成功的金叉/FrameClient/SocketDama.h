#pragma once
#include <string>
#include <WinSock2.h>
#include <fstream>
#include  "AlgoManager.h"
using namespace std;

class AlgoManager;

class SocketDama{
public:
	SocketDama(AlgoManager* ae);
	~SocketDama();

	void setServerIP(std::string ip){
		serverIP = ip;
    }
	void setPort(int pport){
		serverport = pport;
	}

	void setClientID(string clientid);
	bool IsConnected();
	bool Clientservice();
	void updateNetStatus(bool flag);
	void setHaveLogin(bool value);
	bool getHaveLogin();

	void updateMsg(string msg);
	bool CancelMd();
private:
	string serverIP;
	int serverport;
	SOCKET sockClient;
	//AlgoTSEng* m_pAl;
	AlgoManager* alMag;
};