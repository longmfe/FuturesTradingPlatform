#include <iostream>
#include <string>
#include "TickData.h"
#include "DataManager.h"
#include <list>

;using namespace std;

extern char *ppInstrumentID[];			// 行情订阅列表
extern int iInstrumentID;									// 行情订阅数量



DataManager::DataManager()
{
	TickData * tmp = new TickData;

	RcMarketData.push_back(tmp);

	RcMarketDataMap.insert(pair<string,list<TickData*>>("start",this->RcMarketData));

	DManager = this;

};

DataManager::~DataManager()
{
};

void DataManager::recordMarketData(TickData *value)
{
	list<TickData*> tmpl;
	list<TickData*>::iterator ptmpl;
	map<string,list<TickData*>>::iterator mapit;
	mapit = RcMarketDataMap.find(value->sec);
	if (mapit == RcMarketDataMap.end())
	{
		RcMarketDataMap.insert(pair<string,list <TickData*>>(value->sec,this->RcMarketData));
		mapit = RcMarketDataMap.find(value->sec);
		mapit->second.push_back(value);
	}
	else
	{
		mapit->second.push_back(value);
	};



};

list<TickData*>::iterator DataManager::pgetdata(string contractname)
{
	map<string,list<TickData*>>::iterator mapit;
	mapit = RcMarketDataMap.find(contractname);
	list<TickData*> ::iterator listit;
	listit = mapit->second.end();
	return listit;
};

void DataManager::checkfunc()
{
	map<string,list<TickData*>>::iterator mapit;
	int i=0;
	for (mapit = this->RcMarketDataMap.begin();mapit != this->RcMarketDataMap.end();mapit++)
	{ i = i+1;
	};
	cout<<"现在合约有"<<i<<endl;
	
	TickData* tmp;
	list <TickData*> ::iterator ptmpl;

	tmp = this->RcMarketData.back();

//	tmp = *ptmpl;
	cout<<tmp->sec<< tmp->AskPrice<<endl;


	for (mapit = this->RcMarketDataMap.begin();mapit != this->RcMarketDataMap.end();mapit++)
	{
		 tmp = mapit->second.back();
		
		 cout<<tmp->AskPrice<<endl;
	}
}

TickData* DataManager::pRcMarketData(string contractname)
{
	map<string,list<TickData*>>::iterator mapit;
	mapit = RcMarketDataMap.find(contractname);
	list<TickData*> ::iterator listit;
	listit = mapit->second.end();
	return *listit;
}

void DataManager::setlist(TickData * value)
{
	this->RcMarketData.push_back(value);
}