#include<iostream>
#include<fstream>
#include<map>
#include<sstream>
#include<string>
#include<iomanip>
using namespace std;

map<string,string> optab_map;
map<string,string> symtab_map;
map<string,string>::iterator iter;

void readsymtab();
void parseInput();
void readOptab();
string intToHex(int);
int main()
{   
    readOptab();
    readsymtab();
    parseInput();
    return 0;
}

//Read from optab and dave into map
void readOptab()
{
    ifstream optab;
    optab.open("optab.txt");
    string opcode, value;
    while(!optab.eof() )
    {
        optab>>opcode>>value;
        optab_map[opcode] = value;
    }
    optab.close();
}

//Read from symtab and dave into map
void readsymtab()
{
    ifstream symtab;
    symtab.open("symtab.txt");
    string symbol, value;
    while(!symtab.eof() )
    {
        symtab>>symbol>>value;
        symtab_map[symbol] = value;
    }
    symtab.close();
}

void parseInput()
{
    ifstream inputFile;
    ofstream outputFile;
    inputFile.open("locctr.txt");
    outputFile.open("intermediate.txt");
    int lineno = 0;
    string operand, loc, symbol, opcode, line;
    string object_code;
    //read the file line by line till the end
    while(getline(inputFile,line))
    {
        stringstream s(line);
        getline(s, loc, '\t');
        getline(s, symbol, '\t');
        getline(s, opcode, '\t');
        getline(s, operand, '\n');
        lineno++;
        object_code = "";
        cout<<loc<<"\t"<<symbol<<"\t"<<opcode<<"\t"<<operand<<endl;
        //Add the comment line to intermediate file and go to read label
        if(loc[0] == '.')
        {
            outputFile<<line<<endl;
            continue;
        }
        //Write to intermediate file and go to read label
        if(opcode == "START" || opcode == "END")
        {
            outputFile<<line<<endl;
           continue;
        }
        //check whether opcode is prsent in optab
        else if (optab_map.find(opcode) == optab_map.end())
        {
            cout<<"error: opcode not valid at line "<<lineno;
            break;
        }
        //Write the line to file
        outputFile<<loc<<"\t"<<symbol<<"\t"<<opcode<<"\t"<<operand;;

        //get the iter pointing at opcode
        iter = optab_map.find(opcode);
        if(iter->second != "*")
        {
            //check whether operand is there
            if(operand.size() != 0)
            {
                //copy opcode to object code
                object_code =  iter->second;
                ///if symbol is present in symbol table, concatenate symbol value to obj code
                if((iter = symtab_map.find(operand)) != symtab_map.end())
                {
                    object_code = object_code +iter->second;                    
                }
                
                //Check whether index reg is used
                //npos signifies end of string
                else if(operand.find(",X") != string::npos)  
                {
                    int x;
                    //remove ',X' from operand and get actual operand
                    int len = operand.size()-2;
                    string temp = operand.substr(0,len);
                   
                   //check whether it's present in symtab
                    if((iter = symtab_map.find(temp)) != symtab_map.end())
                    {
                        stringstream ss(iter->second);
                        //stores decimal int of string in X
                        //hex converts hex to decimal and vicde versa
                        ss>>hex>>x;
                        //OR the int to set index reg bit to '1'
                        x = x ^ 0x8000;  //32768;
                        //Convert x to hex string and concatenate to obj code
                        stringstream s2;
                        s2<<hex<<x;
                        object_code += s2.str();
                    }
                }
                else
                {
                    cout<<"\nError: invalid operand at line "<<lineno;
                    break;
                }
            }
            cout<<"\naddr "<<object_code;
        }
        else
        {
            if (opcode == "BYTE")
            {
                stringstream ss;
                if(operand[0] == 'C')
                {
                    //remove {c,','} from operand
                    string sub = operand.substr(2);
                    //size-1 to remove the last '
                    //convert ascii value of each char to hex and store it
                    for(int i = 0; i < sub.size()-1; i++)
                        ss<<hex<<(int)sub[i];
                    object_code += ss.str();
                }
                else if(operand[0] == 'X')
                {
                    string sub = operand.substr(2);
                    //append the hex values directly to obj code
                    for(int i = 0; i < sub.size()-1; i++)
                        object_code += sub[i];   
                }
                else
                {
                    cout<<"\nerror ";
                    break;
                }  
            }
            else if(opcode == "WORD")
            {
                //convert the string to decimal num and the hex string
                stringstream ss(operand);
                int value;
                ss>>value;
                stringstream s2;
                //object code length should be 6
                s2<<setfill('0')<<setw(6)<<hex<<value;
                object_code = s2.str();
            }
            cout<<"\nobject code "<<object_code;
        }
        outputFile<<"\t"<<object_code<<endl; 
    }
}
