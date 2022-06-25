#include <string>
#include <iostream>
#include <fstream>
using namespace std;

class readContracts{
public :
	readContracts(){
		filename = "./contracts.txt";
		readSingleIni();
	}
	~readContracts(){
	}
   /*
	string getThisMonthContract(){
		return ThisMonthContract;
	}

	string getNextMonthContract(){
		return NextMonthContract;
	}
	*/
	string getProductName(){
		return productName;
	}
private:
	/*
	void readSingleIni(){
		char buf[128];                //临时保存读取出来的文件内容
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
				if(index > 0){
					NextMonthContract = string(buf);
				}
				else{
					ThisMonthContract = string(buf);
				}

				index++;
			}
			
		}
		infile.close();
	}
	string filename;
	string ThisMonthContract;
	string NextMonthContract;
};*/
	void readSingleIni()
	{
		char buf[128];                //临时保存读取出来的文件内容
		ifstream infile;
		infile.open(filename);
		if(infile.is_open())          //文件打开成功,说明曾经写入过东西
		{
			while(infile.good() && !infile.eof())
			{
				memset(buf,0,sizeof(buf));
				infile.getline(buf,sizeof(buf));
				if(strcmp(buf, "") == 0)
				{ 
					break;
				}
				else
				{   
				   productName = string(buf);
				}
			}
			
		}
		infile.close();
	}
	  string filename;	
	  string productName;
	};