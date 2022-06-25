#include "StdAfx.h"
#include "IndexDetailUnit.h"



IndexDetailUnit::~IndexDetailUnit(void)
{
}
IndexDetailUnit::IndexDetailUnit(string date, string time, int seq, double value)
{
        this->Date = date;
        this->Time = time;
        this->Seq = seq;
        this->Value = value;
}
string IndexDetailUnit::getDate()
{
        return Date;
}
string IndexDetailUnit::getTime()
{
        return Time;
}
    
int IndexDetailUnit::getSeq()
{
        return Seq;
}
    
double IndexDetailUnit::getValue()
{
        return Value;
}
