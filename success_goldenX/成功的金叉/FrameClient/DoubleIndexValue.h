#pragma once
#include "IndexValueBase.h"

class DoubleIndexValue : public IndexValueBase
{
public :
	DoubleIndexValue();
		DoubleIndexValue(double value);
	~DoubleIndexValue(void);

private :
	double value;
public:
    void setValue(double value);
	double getValue();
};