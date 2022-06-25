#include "StdAfx.h"
#include "IndexValue.h"


IndexValue::IndexValue(void)
{
}


IndexValue::~IndexValue(void)
{
}
IndexValue::IndexValue(string date, string time, double value) {	 
	this->date = date;
	this->time = time;
	this->value = value;
} 
/**
	* @return the date
	*/
string IndexValue::getDate() {
		return date;	
} 
/**
	* @param date the date to set
	*/
void IndexValue::setDate(string date) {
	this->date = date;	
} 
/**
	* @return the time
	*/
string IndexValue::getTime() {
	return time;	 
} 
/**
	* @param time the time to set
	*/
void IndexValue::setTime(string time) {
	this->time = time;	 
} 
/**
	* @return the value
	*/
double IndexValue::getValue() {
	return value;
} 
/**
	* @param value the value to set
	*/
void IndexValue::setValue(double value) {
	this->value = value;	 
} 
