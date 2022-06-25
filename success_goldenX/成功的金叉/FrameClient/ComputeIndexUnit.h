#pragma once
#include "FlowUnits.h"
#include <string>

class ComputeIndexUnit : FlowUnits{
public:
	ComputeIndexUnit(){};
	~ComputeIndexUnit(){};
	virtual void init()=0;
	virtual void run()=0;
};