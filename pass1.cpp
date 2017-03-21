#include<cstring>
#include<iostream>
#include <fstream>
#include<sstream>
#include<map>
#include<iomanip>

using namespace std;
map<string,int>optab;
string operand,label,opcode,str;
class pass1
{
	public: 
	void split(string str)
	{
		stringstream iss(str);		
		getline(iss,label,'\t');
		getline(iss,opcode,'\t');
		getline(iss,operand,'\t');
	}

	public:
	void input()
	{
		ifstream file1,file2;
		int loc,len,op;
		map<string,int>::iterator it;
		file1.open("input_1.txt");
		file2.open("optab.txt");
		ofstream file3;
		file3.open("symtab.txt");		
		while(!file2.eof())
		{
			getline(file2,str);
               	 	label=str.substr(0,str.find("\t"));
                	operand=str.substr(str.find("\t")+1,str.length());
                	stringstream s(operand);
                	s>>hex>>optab[label];	
		}
		getline(file1,str);	
		stringstream iss(str);		
		split(str);
		if(label.compare("START"))
		{
			stringstream s(operand);
			s>>hex>>loc;
			//cout<<label<<"\t"<<opcode<<"\t"<<hex<<loc<<endl;
			
		}
		
		else
		{loc=0;}
		
		while(!file1.eof())
		{
			getline(file1,str);
			split(str);
			cout<<hex<<loc<<"\t"<<label<<"\t"<<opcode<<"\t"<<operand<<endl;
			if(label.compare("-")!=0)
			{
				file3<<label<<"\t"<<hex<<loc<<endl;	
			}
			it=optab.find(opcode);
			if(it!=optab.end())
			{
				loc=loc+3;
			}
			else{}
			
			if(opcode.compare("WORD")==0)
			{
				loc=loc+3;
				
			}	
			if(opcode.compare("RESW")==0)
			{
				stringstream s(operand);
				int no;
				s>>no;
				
				loc+=3*no;	
			}
			if(opcode.compare("RESB")==0)
			{
				stringstream s(operand);
				int n;
				s>>n;
				loc+=n;
				
			}
			if(opcode.compare("BYTE")==0)
			{
				if(operand[0]=='X')
				loc+=1;
				else
				{
					loc+=operand.length()-2;
				}	
			}
		}
	}	
};

int main()
{

pass1 p;
p.input();	
return 0;
}
