#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
using namespace std;
class readProperty
{
public:
	readProperty(void);
	~readProperty(void);
	string getUserName();
	string getPassword();
	string getTradeLink();
	string getMDLink();
	string getbrokerID();
private:
	string filename;
	void Ini();
	string username;
	string password;
	string tradeLink;
	string mdLink;
	string brokerID;
};

