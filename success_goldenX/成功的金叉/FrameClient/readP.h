#include <string>
#include <iostream>
#include <fstream> 
#include <list>
using namespace std;

class readP{
public:
	readP(){
		p = -1;
		x = -1;
		filename = "./p.txt";
		readSingleIni();
	}
	~readP(){
	}

	
	double getP(){
	    return p;
	}

	double getX(){
	    return x;
	}

private:
	double p;
	double x;
	string filename;

	void readSingleIni(){
		char buf[128];                //��ʱ�����ȡ�������ļ�����
		ifstream infile;
		infile.open(filename);
		if(infile.is_open())          //�ļ��򿪳ɹ�,˵������д�������
		{
			while(infile.good() && !infile.eof())
			{
				memset(buf,0,sizeof(buf));
				infile.getline(buf,sizeof(buf));
				if('#' == buf[0]){
					continue;
				}
				if( p < 0){
					p = atof(buf);
				}
				else
				{
					x = atof(buf);
				}
			}
		}
		infile.close();
	}

};