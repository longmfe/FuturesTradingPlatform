#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include "StringUtil.h"
#include <list>
using namespace std;

class readInstanceCfg{
public:
	readInstanceCfg(){ 
		
	}
	~readInstanceCfg(){
	}

	
	list<string> getArgs(bool bRealMarket){
		if(bRealMarket)
			filename = "./instancecfg.csv";
		else filename = "./instancecfg_his.csv";
		readSingleCfg();
	    return args;
	}
  
	bool writeSingleCfg(list<string> newargs){
		ofstream outfile(filename,ios::app); 
		outfile.clear();
		list<string>::iterator it;
		if(outfile.is_open()){
			for(it = newargs.begin(); it != newargs.end(); it++){			
					outfile<<*it<<endl;			
			}
		}
		outfile.close();
		return true;
	}

private:
	list<string> args; 

	string filename;

	void readSingleCfg(){
		char buf[512];                //临时保存读取出来的文件内容
		ifstream infile;
		infile.open(filename);
		
		if(infile.is_open())          //文件打开成功,说明曾经写入过东西
		{
			//略过第一行
			if(infile.good() && !infile.eof())
			{
				infile.getline(buf,sizeof(buf)); 
			}

			while(infile.good() && !infile.eof())
			{
				memset(buf,0,sizeof(buf));
				infile.getline(buf,sizeof(buf)); 
				string  str = buf ;
				if(str.length() < 1)
				{
					continue;
				}
				args.push_back(str);
			}
		}
		infile.close();
	}

};