#include "StdAfx.h"
#include "ComputeIndex.h"
#include "Bar.h"
#include "BarDaily.h"
#include "MAUnit.h"

ComputeIndex::ComputeIndex(GlobalDataManage* ma, string conID)
{
	m_ma = ma;
	contractID = conID;
	init();
}

ComputeIndex::~ComputeIndex(void)
{

}
void ComputeIndex::run()
{
	 TickData* sec = m_ma->getMdMana()->getTickData();
	 im->compute(*sec);
}
void ComputeIndex::init()
{
	im = m_ma->getIndexMana();

	Bar* bar1 = new Bar(1,900, contractID); //"IF1303"
    im->registerUnit(bar1);

	Bar* bar5 = new Bar(5,900, contractID); //"IF1303"
	im->registerUnit(bar5);

	BarDaily* bardaily1 = new BarDaily("Not_CFFEX",contractID);
	im->registerUnit(bardaily1);

	MAUnit* ma_1_1 = new MAUnit("ma_1_1",1,1,m_ma);
	im->registerUnit(ma_1_1);
	MAUnit* ma_1_3 = new MAUnit("ma_1_3",1,3,m_ma);
	im->registerUnit(ma_1_3);
	MAUnit* ma_1_5=new MAUnit("ma_1_5", 1, 5, m_ma);
	im->registerUnit(ma_1_5);
	MAUnit* ma_1_10=new MAUnit("ma_1_10", 1, 10, m_ma);
	im->registerUnit(ma_1_10);

	MAUnit* ma_5_5=new MAUnit("ma_5_5", 5, 5, m_ma);
	im->registerUnit(ma_5_5);
	MAUnit* ma_5_10=new MAUnit("ma_5_10", 5, 10, m_ma);
	im->registerUnit(ma_5_10);

	MACD* macd1 = new MACD(contractID, 12, 26, 9, m_ma->getEngineer(), 1);
	im->registerUnit(macd1);
}