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
    
	list <TickData*> RcMarketData;//��¼ĳһ��contract��ÿһ��tick���ݡ�

	void recordMarketData(TickData* value);//��¼����õ���������

	void putintomap(TickData *value);

	map<string, list<TickData*>> RcMarketDataMap;//��¼һ���ж��ٸ�contract������contract�ֱ��¼�б�

	DataManager *DManager;//ָ���Լ�

	list<TickData*>::iterator pgetdata(string contractname);//���������Լ���������ݵ��б�ָ��

	void checkfunc();

	void setlist(TickData * value);

	TickData* pRcMarketData(string contractname);
}
