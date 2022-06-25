#pragma once
#include <string>
#include <list>
#include "ReqMdUnit.h"
#include "ReqIndexUnit.h"
using namespace std;
class RequestStore{
public:
	RequestStore(){
	}

	~RequestStore(){
	}

	void addMdRequest(int type, list<string> cons, list<string> dates,int tickint, int dayint){
		ReqMdUnit req(type, cons, dates,tickint, dayint);
		reqMdL.push_back(req);
	}

	void addIndexRequest(int type, string indexname, string extendname, string args, string endofdate, int len){
		ReqIndexUnit req(type, indexname, extendname, args, endofdate,  len);
		req.setExIndexName(indexname+extendname);
        
		reqIndexL.push_back(req);
	}

	list<ReqMdUnit>& getReqMdUnits(){
		return reqMdL;
	}

	list<ReqIndexUnit>& getReqIndexUnits(){
		return reqIndexL;
	}


private:
	list<ReqMdUnit> reqMdL;
	list<ReqIndexUnit> reqIndexL;
};