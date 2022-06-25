//#include"trading_test.h"

//#include <unistd.h>
//#pragma warning(disable:4996)
//using namespace std;

#include"CtpUserApiInfo.h"
#include"CtpMdApiInfo.h"
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <string>
#include <vector>
#include<list>
#include<string.h>
#include <fstream>
#include <stdio.h>
#include "MarketApplication.h"
//#include <unistd.h>
using namespace std;

const char* g_helpInfo[] = {
	"auth","auth\n\t 无参数，穿透式认证.[traderapi登录前需先认证]",
	"login","login 无参数 \n\t请求登录\n",
	"logout","logout\n\t无参数\n",
	"chgpwd","chgpwd oldpass newpass\n\t修改密.\n\toldpass\t旧密码.\n\tnewpass\t新密码.",
	"sub","sub contract1 [constract2] [contract3][..]\n\t可订阅多个合约，以空格隔开\n",
	"unsub","unsub contract1 [constract2] [contract3][..]\n\t可取消订阅多个合约，以空格隔开\n",
	"confirmsett","confirmsett [date]\n\t确认上日账单。日期格式为yyyymmdd，若不指定，则为上一交易日\n",
	"qryorder","qryorder\n\t无参数 查询委托\n",
	"qrytrade","qrytrade\n\t无参数 查询成交\n",
	"cancel","cancel /n orderid /c contract [cffex|shfe|czce|dce] 根据报单编号、合约代码、交易所编号进行撤单"\
	"\n\teg:cancel /n       408425 /c T1909 cffex(orderid报单编号要与查询的结果完全一致，空格不能省略)",
	"order","order /c contract [cffex|shfe|czce|dce] /v volume [ /p price|0] buy|sell open|close|closetoday|closeyes [hedge|spec|arbi|marketmaker] [/cancel] /f seatID"\
	"\n\tcontract\t合约信息"\
	"\n\tvolume\t手数"\
	"\n\tprice\t指定价格，如果不输或者输0，则表示为市价"\
	"\n\tbuy,sell\t代表买和卖，必须填写其中之一"\
	"\n\topen,close,closetoday,closeyes\t代表开仓、平仓、平今仓和平老仓，必须填写其中之一"\
	"\n\thedge,spec,arbi,marketmaker\t分别表示套保、投机、套利和做市，默认为投机"\
	"\n\teg:order cffex /c T1909 buy open /v 1 /p 97.125",
	"exit","exit\n\t无参数,退出登录\n",
};

bool flag = true;
CUser *tsh; // trade api
CMd *msh; // market api
MarketApplication* cctphandle = new MarketApplication();

std::string Trim(const std::string & str1, const string& token= " \t\n\r", int type=0)
{
	std::string str = str1;
	if (type == 0) {
		std::string::size_type pos = str.find_last_not_of(token);
		if (pos != std::string::npos) {
			str.erase(pos + 1);
			pos = str.find_first_not_of(token);
			if (pos != std::string::npos) str.erase(0, pos);
		}
		else str.erase(str.begin(), str.end());
	}
	else if (type == 1) {
		std::string::size_type pos = str.find_first_not_of(token);
		if (pos != std::string::npos) str.erase(0, pos);
		else str.erase(str.begin(), str.end());
	}
	else if (type == 2) {
		std::string::size_type pos = str.find_last_not_of(token);
		if (pos != std::string::npos) {
			str.erase(pos + 1);
		}
		else str.erase(str.begin(), str.end());
	}
	return str;
}
bool DelimitString(const std::string& str, std::list<std::string>& lst)
{
	size_t i, len;
	std::string temp = str, t;
	char lastChar;

	Trim(temp);
	const char* ptr = temp.c_str();
	len = temp.length();
	if (len < 1)
		return false;

	lst.clear();
	lastChar = ' ';
	i = 0;
	while (i < len)
	{
		if (ptr[i] == ' ' || ptr[i] == '\t' || ptr[i] == '\r' || ptr[i] == '\n') //为空格
		{
			if (lastChar != ' ')//一个段结束
			{
				t = temp.substr(0, i);
				lst.push_back(t);
				temp = temp.substr(i, len - i);
				len = len - i;
				i = 0;
				ptr = temp.c_str();
				lastChar = ' ';
			}
			else
			{
				i = i + 1;
			}
		}
		else
		{
			if (lastChar == ' ') //一个段开始
			{
				lastChar = ptr[i];
				temp = temp.substr(i, len - i);
				ptr = temp.c_str();
				len = len - i;
				i = 0;
			}
			else
			{
				i = i + 1;
			}
		} //end if
	} // end while
	if (len > 0)
		lst.push_back(temp);
	return true;
}
int DoCmd(const list<string>& cmdlst, list<string>::const_iterator it) {	
	/*if (*it == "auth")
	{
		tsh->ReqAuthenticate();
		return 0;
	}
		
	else*/ if (*it == "login")
	{
		tsh->ReqUserLogin();
		msh->ReqUserLogin();
		return 0;
	}
		
	else if (*it == "logout")
	{
		msh->ReqUserLogout();
		tsh->ReqUserLogout();
		return 0;
	}
	else if ("chgpwd" == *it) {
		const char* oldPass = NULL;
		const char* newPass = NULL;
		++it;
		if (it != cmdlst.end()) {
			oldPass = it->c_str();
		}
		else {
			cerr << "命令不正确：必须输入原密码";
			return -1;
		}
		++it;
		if (it != cmdlst.end()) {
			newPass = it->c_str();
		}
		else {
			cerr << "命令不正确：必须输入新密码";
			return -1;
		}
		int re = 0;
		re = tsh->ReqChgPwd(oldPass, newPass);
		return re != 0;
	}
	else if (*it == "sub") {
		cout << "sub" << endl;
		vector<const char*> lst;
		for (it++; it != cmdlst.end(); it++) {
			lst.push_back(it->c_str());
		}
		
		msh->Sub(lst);
		return 0;
	}
	else if (*it == "unsub") {
		cout << "unsub" << endl;
		vector<const char*> lst;
		for (it++; it != cmdlst.end(); it++) {
			lst.push_back(it->c_str());
		}
		msh->UnSub(lst);
		return 0;
	}
	else if (*it == "order") //下单
	{
		//先设置默认值
		SOrderParam param;
		param.hedgeType = _CHF_Speculation;
		param.offset = _OF_CloseToday;
		param.direction = _DT_Null;
		param.orderType = _OT_Normal;
		param.volume = 1;
		param.price = 0;
		param.exchangeId = "CFFEX";
		param.instrumentId = "";
		it++;
		while (it != cmdlst.end()) {
			if (*it == "/c") { //合约
				it++;
				param.instrumentId = it->c_str();
			}
			else if (*it == "/p") { //价格
				it++;
				param.price = atof(it->c_str());
			}
			else if (*it == "/v") { //数量
				it++;
				param.volume = atoi(it->c_str());
			}
			else if (*it == "buy") { //买
				param.direction = _DT_Buy;
			}
			else if (*it == "sell") { //卖
				param.direction = _DT_Sell;
			}
			else if (0 == strcmp(it->c_str(), "cffex")) {
				param.exchangeId = "CFFEX";
			}
			else if (0 == strcmp(it->c_str(), "dce")) {
				param.exchangeId = "DCE";
			}
			else if (0 == strcmp(it->c_str(), "czce")) {
				param.exchangeId = "CZCE";
			}
			else if (0 == strcmp(it->c_str(), "shfe")) {
				param.exchangeId = "SHFE";
			}
			else if (0 == strcmp(it->c_str(), "ine")) {
				param.exchangeId = "INE";
			}
			else if (0 == strcmp(it->c_str(), "SZSE")) {
				param.exchangeId = "SZSE";
			}
			else if (*it == "open") { //开仓
				param.offset = _OF_Open;
			}
			else if (*it == "close") { //平仓
				param.offset = _OF_Close;
			}
			else if (*it == "closetoday") { //平今
				param.offset = _OF_CloseToday;
			}
			else if (*it == "closeyes") { //平老
				param.offset = _OF_CloseYesterday;
			}
			else if (*it == "hedge") { //套保
				param.hedgeType = _CHF_Hedge;
			}
			else if (*it == "spec") { //投机
				param.hedgeType = _CHF_Speculation;
			}
			else if (*it == "arbi") { //套利
				param.hedgeType = _CHF_Arbitrage;
			}
			else if (*it == "marketmaker") {//做市商
				param.hedgeType = _CHF_MarketMaker;
			}
			else if (0 == strcmp(it->c_str(), "fak")) {
				param.orderType = _OT_FAK;
			}
			else if (0 == strcmp(it->c_str(), "fok")) {
				param.orderType = _OT_FOK;
			}
			it++;
		}

		cout << "order" << endl;
		cout << "instrumentID" << param.instrumentId << endl;
		cout << "exchangeID" << param.exchangeId << endl;

		int iResult = tsh->PutOrder(&param);
		Sleep(200);
		tsh->ReqQryPosition(&param);

		return iResult != 0;



	}
	else if (*it == "cancel")//撤单
	{
		it++;
		char ordersysId[13] = {0};
		const char* instrumentId="";
		const char* exchangeId="";
		while (it != cmdlst.end()) {

			if (*it == "/n") { //报单编号
				it++;
				sprintf(ordersysId, "%12s", it->c_str());
			}
			if (*it == "/c") { //合约
				it++;
				instrumentId=it->c_str();
			}
			else if (0 == strcmp(it->c_str(), "cffex")) {
				exchangeId = "CFFEX";
			}
			else if (0 == strcmp(it->c_str(), "dce")) {
				exchangeId = "DCE";
			}
			else if (0 == strcmp(it->c_str(), "czce")) {
				exchangeId = "CZCE";
			}
			else if (0 == strcmp(it->c_str(), "shfe")) {
				exchangeId = "SHFE";
			}
			else if (0 == strcmp(it->c_str(), "ine")) {
				exchangeId = "INE";
			}
			it++;
		}
		int re = tsh->CancelOrder(ordersysId, instrumentId, exchangeId);
		return re != 0;
	}
	else if (*it == "qryorder") //查询委托
	{
		int iResult = tsh->QryOrder();
		return iResult != 0;
	}
	else if (*it == "qrytrade") //查询成交
	{
		int iResult = tsh->QryTrade();
		return iResult != 0;
	}
	else if (*it == "qryinst")
	{
		return tsh->QryInst();
	}
	else if (*it == "confirmsett") {
		cout << "confirmsett" << endl;
		tsh->ConfirmSett("");
		return 0;
	}
	else if (*it == "exit")
		return -2;
	else
	{
		cout << "以下命令可用：\n";
		for (size_t i = 0; i < sizeof(g_helpInfo) / sizeof(char*); i += 2) {
			cout << g_helpInfo[i] << "\n\t";
			cout << g_helpInfo[i + 1] << "\n";
		}
	}
	return 0;


}
int DoCmd(const string& cmd) {

	list<string> cmdlst;
	if (!DelimitString(cmd, cmdlst) || cmdlst.size() < 1) {
		cerr << "命令格式无法解析\n";
		return -1;
	}
	return DoCmd(cmdlst, cmdlst.cbegin());
}
void getConfig() 
{
	ifstream infile;
	const char* t1;
	char line[1024] = { 0 };
	infile.open("config.txt");
	char buffer[100];
	//getcwd(buffer, 100);
	cout << "读取配置文件" << endl;
	while (infile.getline(line, sizeof(line)))
	{
		list<string> linelst;
		cout << line << endl;
		if (!DelimitString(line, linelst) || linelst.size() < 2)
		{
			cout << "2222" << linelst.size() <<" "<< linelst.front() <<" "<< linelst.back()<< endl;
			cerr << "配置文件有误" << endl;
		}
		list<string>::iterator itor = linelst.begin();
		list<string>::iterator itor2 = ++itor;
		if (linelst.front()==( "marketaddr")) {
			MARKET_ADDRESS = *itor2;
		}
		else if (linelst.front() == ("traderaddr")) {
			TRADER_ADDRESS = *itor2;
		}
		else if (linelst.front() == ("brokerid")) {
			BROKERID = *itor2;
		}
		else if (linelst.front() == ("userid")) {
			USERID = *itor2;
			INVESTORID = *itor2;
		}
		else if (linelst.front() == ("pwd")) {
			PASSWORD = *itor2;
		}
		else if (linelst.front() == ("appid")) {
			APPID = *itor2;
		}
		else if (linelst.front() == ("authcode")) {
			AUTHCODE = *itor2;
		}
		else if (linelst.front() == ("instrument")) {
			INSTRUMENTID = *itor2;
		}
	}
	cout << USERID<<" " << PASSWORD << " " << APPID << " " << AUTHCODE << " " << INSTRUMENTID << endl;
}
int main() {
	getConfig();

	tsh = new CUser();
	msh = new CMd(cctphandle);
	tsh->Init();
	msh->Init();
	tsh->ReqUserLogin();
	msh->ReqUserLogin();
	//msh->Sub("i2207");

	
	char buf[1024];
	string cmd;
	while (flag)
	{
		
		fgets(buf, sizeof(buf), stdin);
		cmd = (buf);

		cmd = Trim(cmd);
		if (cmd.empty()) {
			continue;
		}

		if (-2 == DoCmd(cmd))
			break;

	}
	
	system("pause");
	return  0;

}
void order(string cmd) 
{
	DoCmd(cmd);
}
