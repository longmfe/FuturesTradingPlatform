#include "StdAfx.h"
#include "DoubleIndexValue.h"

DoubleIndexValue::DoubleIndexValue()
{
	value = 0;
}
DoubleIndexValue::~DoubleIndexValue(){}
DoubleIndexValue:: DoubleIndexValue(double value)
{
	this->value = value;
}
void DoubleIndexValue::setValue(double value)
{
	this->value = value;
}
double DoubleIndexValue::getValue()
{
    return this->value;
}
