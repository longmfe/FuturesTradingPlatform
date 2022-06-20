#include <iostream>
#include <string>
#include "TickData.h"
#include <list>
#include <map>
using namespace std;


class DataManager
{public :
    DataManager();

    ~DataManager();

    TickData *TD;
    
	list <TickData*> RcMarketData;//记录某一个contract的每一个tick数据。

	void recordMarketData(TickData* value);//记录所获得的行情数据

	void putintomap(TickData *value);

	map<string, list<TickData*>> RcMarketDataMap;//记录一共有多少个contract，按照contract分别记录列表。

	DataManager *DManager;//指向自己

	list<TickData*>::iterator pgetdata(string contractname);//返回所需合约的最新数据的列表指针

	void checkfunc();

	void setlist(TickData * value);

	TickData* pRcMarketData(string contractname);
}
