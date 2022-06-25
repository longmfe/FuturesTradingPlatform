#include <string>
#include <iostream>
#include <fstream>
#include "StringUtil.h"
#include <list>
using namespace std;

class readIni{
public:
	readIni(){
		serverip = "127.0.0.1";
		ServerIp = "ServerIp";
    
		serverPort = 9998;
		ServerPort = "ServerPort";
    
		clientId = "default";
		ClientId = "ClientID";

		filename = "./config.ini";
		readSingleIni();
	}
	~readIni(){
	}

	
	string getServerIp(){
	    return serverip;
	}

	int getServerPort(){
		return serverPort;
	}

	string getClientId(){
		return clientId;
	}

private:
	string serverip;
	string ServerIp;

	string ServerPort;
	int serverPort;

	string clientId;
	string ClientId;

	string filename;

	void readSingleIni(){
		char buf[128];                //临时保存读取出来的文件内容
		ifstream infile;
		infile.open(filename);
		if(infile.is_open())          //文件打开成功,说明曾经写入过东西
		{
			while(infile.good() && !infile.eof())
			{
				memset(buf,0,sizeof(buf));
				infile.getline(buf,sizeof(buf));
				StringUtil su;
				list<string> tmpl = su.stringAnalysis(string(buf),"=");
				if(strcmp(tmpl.front().c_str(), ServerIp.c_str()) == 0){
					serverip = tmpl.back();
				}

				if(strcmp(tmpl.front().c_str(), ServerPort.c_str()) == 0){
					serverPort = atoi(tmpl.back().c_str());
				}

				if(strcmp(tmpl.front().c_str(), ClientId.c_str()) == 0){
					clientId = tmpl.back();
				}
			}
		}
		infile.close();
	}

};