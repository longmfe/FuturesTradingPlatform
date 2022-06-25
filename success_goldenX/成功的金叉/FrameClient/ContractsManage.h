#pragma once
#include <string>
#include <map>
#include "Contract.h"
using namespace std;
class ContractsManage
{
public:
	ContractsManage(bool);
	~ContractsManage(void);
	
	
	void addContract(Contract* con);
	
	Contract* getContractByID(string str);
	
	void modifyContract(Contract con);
private:
	map<string, Contract*> conMap;
	bool isGlobal;
};

