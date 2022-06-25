#pragma once
#include <string>
#include <list>

using namespace std;

class ReqMdUnit{
private:
	int Type;
	list<string> ConsL;
	list<string> DatesL;
	int TickInt;
	int DayInt;
public:
	ReqMdUnit(int type, list<string> cons, list<string> dates, int tickint, int dayint){
	    Type = type;
		ConsL = cons;
		DatesL = dates;
		TickInt = tickint;
		DayInt = dayint;
	}
	~ReqMdUnit(){
	}

	int getType(){
		return Type;
	}

	list<string> getConsL(){
		return ConsL;
	}

	list<string> getDates(){
		return DatesL;
	}

	int getTickInt(){
		return TickInt;
	}

	int getDayInt(){
		return DayInt;
	}
};