#include "StdAfx.h"
#include "KeepRecord.h"
#include "FrameClientDlg.h"
extern CFrameClientDlg* pUIDlg;

KeepRecord::KeepRecord(string name )
{
	fileName = name;
	isInital =false;
	splitField = ",";
	filepath  = "";
	dir = "Record\\";
	filepostfix = ".csv";
	fw = NULL;
	init();
}
KeepRecord::~KeepRecord(void)
{
	if(fw!=NULL)
	{
		delete fw;
	}
}

void KeepRecord::init()
{
	isInital = true;
	filepath = dir + fileName + filepostfix;
	fw = new ofstream(filepath,ios::app);//(filename, ios::app)
}
 void KeepRecord::printInit()
{
	if(isInital == false)
	{
		init();
	}
	*fw<<"Time,LastPrice,LVolume,BidPrice,BidVolume,AskPrice,AskVolume,OpenInterest,TradeVolume"<<endl;
    fw->flush();
}

void KeepRecord::appendData(string time,double lastPrice,double lVolume,
                            double BidPrice,double BidVolume,double AskPrice,
                            double AskVolume,double OpenInterest,double TradeVolume)
{
	 toolsClass ct1;
	 string wt = time + splitField + ct1.doubleToString(lastPrice) + splitField
                     + ct1.doubleToString(lVolume) + splitField + ct1.doubleToString(BidPrice) + splitField + 
					 ct1.doubleToString(BidVolume)
                     + splitField + ct1.doubleToString(AskPrice) + splitField + ct1.doubleToString(AskVolume)
                     + splitField + ct1.doubleToString(OpenInterest) + splitField + ct1.doubleToString(TradeVolume);
	 *fw<< wt<<endl;
}
void KeepRecord::appendData(string rec)
{
	pUIDlg->addTipsMsg(rec);
     *fw<< rec<<endl;
}
void KeepRecord::Finish()
{
	fw->close();
}

void KeepRecord::writeLine(string parm)
{
	*fw<<parm<<endl;
	fw->flush();
}
