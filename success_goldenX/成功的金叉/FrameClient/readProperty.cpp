#include "StdAfx.h"
#include "readProperty.h"

readProperty::readProperty(void)
{
	filename = "./config.txt";
	Ini();
}


readProperty::~readProperty(void)
{

}
void readProperty::Ini()
{
	char buf[256];                //临时保存读取出来的文件内容
	ifstream infile;
	infile.open(filename);

	if(infile.is_open())          //文件打开成功,说明曾经写入过东西
	{
		while(infile.good() && !infile.eof())
		{
			memset(buf,0,sizeof(buf));
			infile.getline(buf,sizeof(buf));
			if(strcmp(buf, "") == 0) continue;
			string tmpline = string(buf);
			int getIndex = tmpline.find("=");
			string key = tmpline.substr(0,getIndex);
			string value = tmpline.substr(getIndex+1,tmpline.size());
			if(strcmp(key.c_str(),"password")==0)
			{
				this->password = value;
			}else if(strcmp(key.c_str(),"username")==0)
			{
				this->username = value;
			}else if(strcmp(key.c_str(),"tradelink")==0)
			{
				this->tradeLink = value;
			}else if(strcmp(key.c_str(),"mdlink")==0)
			{
				this->mdLink = value;
			}else if(strcmp(key.c_str(),"brokerID")==0)
			{
				this->brokerID = value;
			}
		}
			
	}
	infile.close();
}
string readProperty::getUserName()
{
	return this->username;
}
string readProperty::getPassword()
{
	return this->password;
}
string readProperty::getTradeLink()
{
	return this->tradeLink;
}
string readProperty::getMDLink()
{
	return this->mdLink;
}
string readProperty::getbrokerID()
{
	return brokerID;
}