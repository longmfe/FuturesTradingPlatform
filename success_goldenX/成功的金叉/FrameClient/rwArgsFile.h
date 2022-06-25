#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class rwArgsFile{
private:
	int buyBar;
	double buygap;
	int sellBar;
	double sellgap;
	string filename;
public:
	rwArgsFile(){
		filename = "./args.txt";

		buyBar = -10000;
	    buygap = 0;
	    sellBar = -10000;
	    sellgap = 0;
	}
	~rwArgsFile(){
	}

	void readArgsFile(){
		char buf[128];                //��ʱ�����ȡ�������ļ�����
		ifstream infile;
		infile.open(filename);
		if(infile.is_open())          //�ļ��򿪳ɹ�,˵������д�������
		{
			memset(buf,0,sizeof(buf));
			infile.getline(buf,sizeof(buf));
			sscanf(buf, "%d,%lf,%d,%lf", & buyBar, & buygap, & sellBar, & sellgap);			
			infile.close();
		}
		infile.close();
	}

	void writeArgsFile(int buybar,double buygap, int sellbar, double sellgap){
		//char buf[128];                //��ʱ�����ȡ�������ļ�����
		ofstream of;
		of.open(filename);
		if(of.is_open())          //�ļ��򿪳ɹ�,˵������д�������
		{
			of << buybar << "," << buygap << ","<< sellbar << ","<< sellgap<< endl;		
			of.close();
		}
		
		of.close();
	}

	int getBuyBar(){
		return buyBar;
	}

	double getBuyGap(){
		return buygap;
	}

	int getSellBar(){
		return sellBar;
	}

	double getSellGap(){
		return sellgap;
	}
};