#include <string>
#include <iostream>
#include <fstream>
#include <list>
using namespace std;

class readMe{
public:
	readMe(){
	    filename = "./readme.txt";
		readSingleTxt();
	}
	~readMe(){
	}

	list<string> getContents(){
		return contents;
	}

private:
	void readSingleTxt(){
		char buf[256];                //临时保存读取出来的文件内容
		ifstream infile;
		infile.open(filename);
		int index = 0;
		if(infile.is_open())          //文件打开成功,说明曾经写入过东西
		{
			while(infile.good() && !infile.eof())
			{
				memset(buf,0,sizeof(buf));
				infile.getline(buf,sizeof(buf));
				if(strcmp(buf, "") == 0) break;
				contents.push_back(string(buf));
			}
			
		}
		infile.close();
	}
	string filename;
	list<string> contents;
};