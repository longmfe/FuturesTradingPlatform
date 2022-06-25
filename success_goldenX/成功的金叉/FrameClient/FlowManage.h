#pragma once
#include "FlowUnits.h"
#include <list>
using namespace std;

class FlowManage{
public:
	FlowManage(){
	}
	~FlowManage(){
	}
	void init(){
	}
	bool registerFu(FlowUnits* pfu){
		fuList.push_back(pfu);
		return true;
	}
	void refresh(){
		for(list<FlowUnits*>::iterator it = fuList.begin(); it != fuList.end(); it++ ){
			(*it)->run();
		}
	}
private:
	list<FlowUnits*> fuList;
};