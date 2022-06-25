#pragma once
#include <string>
using namespace std;
class IndexValue
{
public:
	IndexValue(void);
	~IndexValue(void);
	IndexValue(string date, string time, double value);
	/**
	 * @return the date
	 */
	string getDate();
	/**
	 * @param date the date to set
	 */
	void setDate(string date);
	/**
	 * @return the time
	 */
	string getTime() ;
	/**
	 * @param time the time to set
	 */
	void setTime(string time);
	/**
	 * @return the value
	 */
	double getValue();
	/**
	 * @param value the value to set
	 */
	void setValue(double value);
private:
	string date;
	string time;
	double value;
};

