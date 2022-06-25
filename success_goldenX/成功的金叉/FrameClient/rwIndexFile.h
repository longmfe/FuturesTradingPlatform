#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include "StringUtil.h"
using namespace std;

class rwIndexFile{
private:
	string filename;

	list<double> IndexList1;
	list<double> IndexList2;

public:
	rwIndexFile(){
		filename = "./index.txt";
	}
	~rwIndexFile(){
	}

	void readIndexFile(){
		char buf[128];                //临时保存读取出来的文件内容
		ifstream infile;
		infile.open(filename);
		if(infile.is_open())          //文件打开成功,说明曾经写入过东西
		{
			while(infile.good() && !infile.eof())
			{
				memset(buf,0,sizeof(buf));
				infile.getline(buf,sizeof(buf));
				if(strcmp(buf, "") == 0) break;
				StringUtil su;
				list<string> tmpl = su.stringAnalysis(string(buf),",");
			
				IndexList1.push_back(atof(tmpl.front().c_str()));
				IndexList2.push_back(atof(tmpl.back().c_str()));
			}			
		}
		infile.close();

	}
	void writeIndexFile(list<double>& index1, list<double>& index2){
		ofstream of;
		of.open(filename);
		list<double>::iterator it1 = index1.begin();
		list<double>::iterator it2 = index2.begin();
		if(of.is_open())          //文件打开成功,说明曾经写入过东西
		{
			for( ; it1 != index1.end() && it2 != index2.end(); it1++, it2++){
				of << *it1 << "," << *it2 << endl;
			}
		}

		of.close();
	}

	list<double> getIndexList1(){
		return IndexList1;
	}

	list<double> getIndexList2(){
		return IndexList2;
	}
};