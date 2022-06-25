#using <mscorlib.dll>
#include<msclr/marshal.h>
using namespace System; 
using namespace msclr::interop;
#using <System.Windows.Forms.dll>
using namespace System::Windows::Forms;
#using "MyLib/WinRemoteClient_Alpha.dll" 
using namespace WinRemoteClient_Alpha;
#include "QRCadapter.h"
#include "TickData.h"
#include"ESproductInfo.h"
#include <sstream>
#include <fstream>
using namespace std;
void ConvertToString(System::String^ str, std::string& text)
{
     char* p = (char*)(int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
     text = p;
     System::Runtime::InteropServices::Marshal::FreeHGlobal((System::IntPtr)p);
}
public ref class AlphaContainer //
{
public:
     static  WinRemoteClient_Alpha::AlphaCommander^ t1 = gcnew WinRemoteClient_Alpha::AlphaCommander();
	 

};
QRCadapter::QRCadapter(ESproductInfo* qpi)
{
	this->QPI = qpi;
}
QRCadapter::~QRCadapter()
{

}
void QRCadapter::connect()
{
	    String^ sistr = gcnew String(this->QPI->strategyID.c_str());
		String^ qrccn = gcnew String(this->QPI->tickername.c_str());
		String^ ipstr = gcnew String(this->QPI->ip.c_str());
		String^ portstr = gcnew String(this->QPI->port.c_str());

		Console::WriteLine(L"Connecting to QRC.........."+sistr+ipstr+portstr); 

		(AlphaContainer::t1)->registAlpha(sistr,qrccn,ipstr,portstr);

		//MessageBox::Show( L"Welcome to C++/CLI", "TestApp", MessageBoxButtons::OK );
};
void QRCadapter::initReq(double x)
{
	         String^ portstr = gcnew String(this->QPI->port.c_str());
			(AlphaContainer::t1)->initRequestForSingle(portstr,x);
};
string QRCadapter::MDFeedin(TickData ss,double position)
{
    //20100416 091500  
    string ticktime = ss.getDate()+ " "+ ss.getTime().substr(0,6);
    String^ ticktime_cs = gcnew String(ticktime.c_str());

	//Futures_CSI300_201005  IF1301
	String^ tickername_cs;
	if(this->QPI->exchange.compare("CFFEX")==0 || this->QPI->exchange.compare("SHFE")==0 || this->QPI->exchange.compare("DCE")==0 )
	{
		tickername_cs =gcnew String( ("Futures,"+this->QPI->tickername+",20"+ ss.getSec().substr(this->QPI->realname.length(),4)).c_str()  );
	}
	else if(this->QPI->exchange.compare("CZCE")==0)
	{
		tickername_cs =gcnew String( ("Futures,"+this->QPI->tickername+",201"+ ss.getSec().substr(this->QPI->realname.length(),3)).c_str()  );
	}
	

	//bid
	String^ bid_cs = gcnew String(DoubleToString(ss.getBidPrice()).c_str());
	String^ bidVolume_cs = gcnew String(DoubleToString(ss.getBidVol()).c_str());

	//ask
	String^ ask_cs = gcnew String(DoubleToString(ss.getAskPrice()).c_str());
	String^ askVolume_cs = gcnew String(DoubleToString(ss.getAskVol()).c_str());

	//last
	String^ lastPrice_cs = gcnew String(DoubleToString(ss.getLastPrice()).c_str());
	String^ lastVolume_cs = gcnew String(DoubleToString(ss.getLastVol()).c_str()); 

	//existingOpenPosition  
	//TODO need to add the newest position status.
	String^ existingOpenPosition_cs = gcnew String(DoubleToString(position).c_str());


	//tradingPlatformTimeZoneIndex
	String^ tradingPlatformTimeZoneIndex_cs = gcnew String(QPI->tradingPlatformTimeZoneIndex.c_str());

	//exchangeTimeZoneIndex
	String^ exchangeTimeZoneIndex_cs = gcnew String(QPI->exchangeTimeZoneIndex.c_str());

	//tradingSessionString
	String^ tradingSessionString_cs = gcnew String(QPI->tradingSessionString.c_str());

	//exchangeRate
	String^ exchangeRate_cs = gcnew String(QPI->exchangeRate.c_str());

	//pointValue
	String^ pointValue_cs = gcnew String(QPI->pointValue.c_str());

	//minMove
	String^ minMove_cs = gcnew String(QPI->minMove.c_str());

	//lotSize
	String^ lotSize_cs = gcnew String(QPI->lotSize.c_str());

	//commissionOnRate
	String^ commissionOnRate_cs = gcnew String(QPI->commissionOnRate.c_str());

	//commissinPerShareInXxx
	String^ commissinPerShareInXxx_cs = gcnew String(QPI->commissinPerShareInXxx.c_str());

	//minCommissionInXxx
	String^ minCommissionInXxx_cs = gcnew String(QPI->minCommissionInXxx.c_str());

	//maxCommissionInXxx
	String^ maxCommissionInXxx_cs = gcnew String(QPI->maxCommissionInXxx.c_str());

	//stampDutyRate
	String^ stampDutyRate_cs = gcnew String(QPI->stampDutyRate.c_str());

	//slippagePoints
	String^ slippagePoints_cs = gcnew String(QPI->slippagePoints.c_str());

	String^ portstr = gcnew String(this->QPI->port.c_str());

	String^ rtn = (AlphaContainer::t1)->TickFeedinForSingle(portstr,ticktime_cs,
            tickername_cs, bid_cs,  bidVolume_cs, ask_cs,askVolume_cs,lastPrice_cs,lastVolume_cs,
            existingOpenPosition_cs,  tradingPlatformTimeZoneIndex_cs,  exchangeTimeZoneIndex_cs,  tradingSessionString_cs,
            exchangeRate_cs,  pointValue_cs,  minMove_cs,  lotSize_cs, commissionOnRate_cs,  commissinPerShareInXxx_cs, 
            minCommissionInXxx_cs, maxCommissionInXxx_cs,  stampDutyRate_cs, slippagePoints_cs);
	string strrtn;
	ConvertToString(rtn,strrtn);
	return strrtn;
}
string QRCadapter::BarFeedin(BarValue bv,double position,bool isEnd)
{    
	//20100416 091500 
	if( (bv.time.substr(0,1).compare("0")!=0) && (bv.time.substr(0,1).compare("1")!=0) )
	{
		cout<<" bv.time :" <<bv.time << endl;
	}
    //string bartime =  bv.time.substr(0,4)+"00";

	string bartimefinal = bv.date+ " "+bv.time;
	if(bartimefinal.compare("")==0)
	{
		return "Error";
	}

    String^ bartime_cs = gcnew String(bartimefinal.c_str());

	String^ barname_cs;
	if(this->QPI->exchange.compare("CFFEX")==0 || this->QPI->exchange.compare("SHFE")==0 || this->QPI->exchange.compare("DCE")==0 )
	{
		barname_cs =gcnew String( ("Futures,"+this->QPI->tickername+",20"+ bv.realContractID.substr(this->QPI->realname.length(),4)).c_str()  );
	}
	else if(this->QPI->exchange.compare("CZCE")==0)
	{
		barname_cs =gcnew String( ("Futures,"+this->QPI->tickername+",201"+ bv.realContractID.substr(this->QPI->realname.length(),3)).c_str()  );
	}
	//ohlcv
	String^ open = gcnew String(DoubleToString(bv.open).c_str());
	String^ high = gcnew String(DoubleToString(bv.high).c_str());
	String^ low = gcnew String(DoubleToString(bv.low).c_str());
	String^ close = gcnew String(DoubleToString(bv.close).c_str());
	String^ volume = gcnew String(DoubleToString(bv.tradevolume).c_str());

	//existingOpenPosition  
	//TODO need to add the newest position status.
	String^ existingOpenPosition_cs = gcnew String(DoubleToString(position).c_str());

	//tradingPlatformTimeZoneIndex
	String^ tradingPlatformTimeZoneIndex_cs = gcnew String(QPI->tradingPlatformTimeZoneIndex.c_str());

	//exchangeTimeZoneIndex
	String^ exchangeTimeZoneIndex_cs = gcnew String(QPI->exchangeTimeZoneIndex.c_str());

	//tradingSessionString
	String^ tradingSessionString_cs = gcnew String(QPI->tradingSessionString.c_str());

	//exchangeRate
	String^ exchangeRate_cs = gcnew String(QPI->exchangeRate.c_str());

	//pointValue
	String^ pointValue_cs = gcnew String(QPI->pointValue.c_str());

	//minMove
	String^ minMove_cs = gcnew String(QPI->minMove.c_str());

	//lotSize
	String^ lotSize_cs = gcnew String(QPI->lotSize.c_str());

	//commissionOnRate
	String^ commissionOnRate_cs = gcnew String(QPI->commissionOnRate.c_str());

	//commissinPerShareInXxx
	String^ commissinPerShareInXxx_cs = gcnew String(QPI->commissinPerShareInXxx.c_str());

	//minCommissionInXxx
	String^ minCommissionInXxx_cs = gcnew String(QPI->minCommissionInXxx.c_str());

	//maxCommissionInXxx
	String^ maxCommissionInXxx_cs = gcnew String(QPI->maxCommissionInXxx.c_str());

	//stampDutyRate
	String^ stampDutyRate_cs = gcnew String(QPI->stampDutyRate.c_str());

	//slippagePoints
	String^ slippagePoints_cs = gcnew String(QPI->slippagePoints.c_str());

	String^ isEnd_cs ;
	if(isEnd)
	{
		isEnd_cs = gcnew String("1");
	}
	else
	{
         isEnd_cs = gcnew String("0");
	}
	String^ portstr = gcnew String(this->QPI->port.c_str());
	//String^ rtn =
	String^ rtn =  (AlphaContainer::t1)->BarFeedinForSingle(portstr,bartime_cs, barname_cs, 
            open, high, low, close, volume,
            existingOpenPosition_cs,tradingPlatformTimeZoneIndex_cs,  exchangeTimeZoneIndex_cs,  tradingSessionString_cs,
            exchangeRate_cs, pointValue_cs,  minMove_cs,  lotSize_cs,  commissionOnRate_cs,  commissinPerShareInXxx_cs,
             minCommissionInXxx_cs,  maxCommissionInXxx_cs,  stampDutyRate_cs,  slippagePoints_cs, isEnd_cs,"0");
	string strrtn;
	ConvertToString(rtn,strrtn);
	return strrtn;
}
string QRCadapter::DoubleToString(double d)
{
    //Need #include <sstream> 
    using namespace std;
    string str; 
    stringstream ss;
    ss<<d;

    ss>>str;
    return str;
}
int QRCadapter::preFeedin()
{
	String^ portstr = gcnew String(this->QPI->port.c_str());
	return (AlphaContainer::t1)->preFeedForSingle(portstr);
}

string QRCadapter::preLoad(string tickername,string datetime,string open,string high,string low,string close,string vol,string pos,bool isEnd)
{
	
	string bartimefinal =datetime;
	String^ bartime_cs = gcnew String(bartimefinal.c_str());
	String^ barname_cs = gcnew String(tickername.c_str());

	//ohlcv
	String^ open_cs = gcnew String((open).c_str());
	String^ high_cs = gcnew String((high).c_str());
	String^ low_cs = gcnew String((low).c_str());
	String^ close_cs = gcnew String((close).c_str());
	String^ volume_cs = gcnew String((vol).c_str());

	String^ existingOpenPosition_cs = gcnew String((pos).c_str());

	//tradingPlatformTimeZoneIndex
	String^ tradingPlatformTimeZoneIndex_cs = gcnew String(QPI->tradingPlatformTimeZoneIndex.c_str());

	//exchangeTimeZoneIndex
	String^ exchangeTimeZoneIndex_cs = gcnew String(QPI->exchangeTimeZoneIndex.c_str());

	//tradingSessionString
	String^ tradingSessionString_cs = gcnew String(QPI->tradingSessionString.c_str());

	//exchangeRate
	String^ exchangeRate_cs = gcnew String(QPI->exchangeRate.c_str());

	//pointValue
	String^ pointValue_cs = gcnew String(QPI->pointValue.c_str());

	//minMove
	String^ minMove_cs = gcnew String(QPI->minMove.c_str());

	//lotSize
	String^ lotSize_cs = gcnew String(QPI->lotSize.c_str());

	//commissionOnRate
	String^ commissionOnRate_cs = gcnew String(QPI->commissionOnRate.c_str());

	//commissinPerShareInXxx
	String^ commissinPerShareInXxx_cs = gcnew String(QPI->commissinPerShareInXxx.c_str());

	//minCommissionInXxx
	String^ minCommissionInXxx_cs = gcnew String(QPI->minCommissionInXxx.c_str());

	//maxCommissionInXxx
	String^ maxCommissionInXxx_cs = gcnew String(QPI->maxCommissionInXxx.c_str());

	//stampDutyRate
	String^ stampDutyRate_cs = gcnew String(QPI->stampDutyRate.c_str());

	//slippagePoints
	String^ slippagePoints_cs = gcnew String(QPI->slippagePoints.c_str());

	String^ isEnd_cs ;
	if(isEnd)
	{
		isEnd_cs = gcnew String("1");
	}
	else
	{
		isEnd_cs = gcnew String("0");
	}
	String^ portstr = gcnew String(this->QPI->port.c_str());
	String^ rtn =  (AlphaContainer::t1)->BarFeedinForSingle(portstr,bartime_cs, barname_cs, 
		open_cs, high_cs, low_cs, close_cs, volume_cs,
		existingOpenPosition_cs,tradingPlatformTimeZoneIndex_cs,  exchangeTimeZoneIndex_cs,  tradingSessionString_cs,
		exchangeRate_cs, pointValue_cs,  minMove_cs,  lotSize_cs,  commissionOnRate_cs,  commissinPerShareInXxx_cs,
		minCommissionInXxx_cs,  maxCommissionInXxx_cs,  stampDutyRate_cs,  slippagePoints_cs, isEnd_cs,"1");
	string strrtn;
	ConvertToString(rtn,strrtn);
	return strrtn;
}




//like 091500->091600 or 105900->110000
string QRCadapter::minutePP(string time)
{
	int intTime = atoi(time.c_str());
	if(intTime < 10000)
	{
		//Console::WriteLine(L"QRCadapter::minutePP atoi fail:"+ time); 
		cout<< "QRCadapter::minutePP atoi fail:"+ time<<endl;
		//MessageBox::Show( L(time), "TestApp", MessageBoxButtons::OK );//L"minutePP error:"+
		return "["+time+"]";
	}
	else if(intTime > 200000)
	{
		cout<< "QRCadapter::minutePP input time error:"+ time<<endl;
		return "["+time+"]";
	}
	else
	{
		int HH = intTime/10000;
		int MM = (intTime%10000)/100;
		if(MM==59)
		{
			HH++;
			MM =0;
		}
		else
		{
			MM++;
		}
		int finalTime = HH*10000 + MM * 100;
		char chartime[10];
		itoa(finalTime,chartime,10);
		string strtime = chartime;

		//092000
		if(strtime.length()==5)
		{
			string tmpzero = "0";
	       strtime = tmpzero.append(strtime);
		}

		return strtime;
	}
}
