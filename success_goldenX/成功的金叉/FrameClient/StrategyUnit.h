#pragma once
#include "FlowUnits.h"
#include <string>

using namespace std;
class StrategyUnit : FlowUnits{
public:
	StrategyUnit(){
	}
	~StrategyUnit(){
	}
	virtual void init(){
	}
	virtual void run(){
	}
	virtual string getStrategyName(){
		return string("StrategyName");
	}
};