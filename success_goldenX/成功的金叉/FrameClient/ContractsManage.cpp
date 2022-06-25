#include "StdAfx.h"
#include "ContractsManage.h"
#include <iostream>
using namespace std;

extern ContractsManage* g_conM;

ContractsManage::ContractsManage(bool isglobal )
{
	map<string ,Contract*>::iterator it = conMap.begin();
	for(;it!=conMap.end();it++)
	{
		if(it->second!=NULL)
		{
			delete it->second;
			it->second = NULL;
		}
	}
	isGlobal = isglobal;
}


ContractsManage::~ContractsManage(void)
{
	map<string ,Contract*>::iterator it = conMap.begin();
	for(;it!=conMap.end();it++)
	{
		delete it->second;
	}
}
void ContractsManage::addContract(Contract* con)
{
	map<string ,Contract*>::iterator it = conMap.find(con->getConid());
	if(it==conMap.end())
	{
		conMap.insert(pair<string,Contract*>(con->getConid(),con));
	}else
	{
		delete con;
	}

	//////////
	/*if(!isGlobal){
		Contract* c  = new Contract(con->getConid(),con->getMinPrice(),
					  con->getMulty(),con->getCommissionRate(),con->getRatioOfmargin());
		g_conM->addContract(c);
	}*/
}
//if contract ID is -1 , not find it ,or u can check if the refence ID and return ID are the same
Contract* ContractsManage::getContractByID(string str)
{
	map<string ,Contract*>::iterator it = conMap.find(str);		
	if(it==conMap.end())
	{
		return NULL;
	}else
	{
		return it->second;
	}
}
void ContractsManage::modifyContract(Contract con)
{
		 
}
