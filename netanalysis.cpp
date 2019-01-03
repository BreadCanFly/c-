#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
vector<string> v1; //存储文件逐行读取的内容。
vector<vector<string>> v2; //存储每一行内容 分割的内容。
vector<vector<string>> v3;//存储同一个编号的TCP连接
int startCount=0;//count TCP连接个数
 int countlines(){
    ifstream ReadFile;
     int n=0;
    string tmp;
    ReadFile.open("C:\\Users\\zp\\Desktop\\Project\\trace\\441trace.txt",ios::in);//ios::in 表示以只读的方式读取文件
    if(ReadFile.fail())//文件打开失败:返回0 
    {
        return 0;
    }
    else//文件存在
    {
        while(getline(ReadFile,tmp,'\n'))
        {
            n++;
        }
        ReadFile.close();
        return n;
    }
}
//分割字符串
vector<string> split(const string& str, const string& pattern)
{
        vector<string> ret;
        if(pattern.empty()) 
            return ret;  
        size_t start=0,index=str.find_first_of(pattern,0);
        while(index!=str.npos)
        {
            if(start!=index)
                ret.push_back(str.substr(start,index-start));
            start=index+1;
            index=str.find_first_of(pattern,start);
        }
        if(!str.substr(start).empty())
            ret.push_back(str.substr(start));
        return ret;
}

void ReadDataFromFileLBLIntoCharArray()//逐行，以插char[]方式存储
{
    ifstream fin("C:\\Users\\zp\\Desktop\\Project\\trace\\example1.txt"); 
    const int LINE_LENGTH = 200; 
    char str[LINE_LENGTH];  
    while( fin.getline(str,LINE_LENGTH) )
    {    
        cout << "Read from file: " << str << endl;
    }
}
void ReadDataFromFileLBLIntoString()//逐行读取以string方式存储//以vector对象的方式存储每行
{
    ifstream fin("C:\\Users\\zp\\Desktop\\Project\\trace\\441trace.txt");  
    string str;
    int countR=0;
    int countF=0;
    int countS=0;
    int countCF=0;//只要含有F表明TCP连接就已经结束，但不一定是完整的TCP连接。
    
    string startNum;//连接分类的标志位
    string ipport1;//判别端口
    string ipport2;
    string ipport3;
   
    while( getline(fin,str) )
    {      
        v1.push_back(str+" "+"0");
       
    }
     
    for(int i=0;i<v1.size();i++){ 

        v2.push_back(split(v1[i]," "));   
      // cout<<v1[i]<<endl;
        
    }
    
    for(int j=0;j<v2.size();j++)
    {
        string strTemp;
        string str1;
        
        str1=*(v2[j].begin()+1);
        strTemp=*(v2[j].end()-2);
        startNum=*(v2[j].end()-1);
        //cout<<v2[ j][4]<<endl;
        //cout<<str1<<endl;
        //cout<<*(v2[j].end()-1)<<" ";
       
        //进行TCP连接的整理分类   
        if(strTemp=="S") 
        {
            countS++;
            startCount++;
            startNum=to_string(startCount);
            *(v2[j].end()-1)=startNum;
        }
         else
        {
           int flag = 0;
           ipport1=v2[j][1]+v2[j][6]+v2[j][3]+v2[j][7];
           for(int i=j-1;i>=0;i--)
           {
                ipport2=v2[i][1]+v2[i][6]+v2[i][3]+v2[i][7];
                ipport3=v2[i][3]+v2[i][7]+v2[i][1]+v2[i][6];
                if (ipport1 == ipport2 || ipport1 == ipport3)
                {
                    *(v2[j].end()-1)=*(v2[i].end()-1);
                    flag=1;
                    if(*(v2[i].end()-2)=="S")
                    {
                        if(*(v2[j].end()-2)=="F")
                          countF++;
                        if(*(v2[j].end()-2)=="R")
                            countR++;
                            break;
                    }
                }
           }
          if(flag==0)
          {
               startCount++;
            startNum=to_string(startCount);
            *(v2[j].end()-1)=startNum;
          }
          // cout<<*(v2[j].end()-1)<<endl;
        }
       
        if(strTemp.find("F",0)!=string::npos)
            countCF++;
        for (vector<string>::const_iterator p = v2[j].begin(); p != v2[j].end(); p++)
        {
            //cout<<*(v2[j].end()-1);
            //cout<<*p[v2[j].length()-1]<<" ";
            //cout<<*p;
        }
        //cout<<endl;
       //cout<<v2[j][11]<<endl;
    }
    //cout<<countCF<<endl;
    
    cout<<"complete TCP connections :"<<countF<<endl;
    cout<<"reset TCP connections :"<<countR<<endl;
    cout<<"TCP connections were still open :"<<countS-countF-countR<<endl;
       //分割str，查找关闭或重置。 
}
//完整TCP持续连接时间分析
void TimeAlysis(){


    //连接时间大小判断
    double min=10000000.0;
    double tempmin=0.0;
    double tempmax=0.0;
    double max=0.0;
    double sum=0.0;
    double timeTemp1=0.0;
    double timeTemp2=0.0;
    double timeTemp=0.0;
    //string str2;
    //max=stod(str2);
    //min=stod(str2);
   
    //string strP;
    //PackMax=stoi(strP);
    //PackMin=stoi(strP);

    //计算TCP连接时间大小
    for(int i=1;i<=startCount;i++)
    {
        timeTemp1=0.0;
        timeTemp2=0.0;
        timeTemp=0.0;
        string strStTemp;
        strStTemp=to_string(i);
        int num=0;
        v3.clear();
        while(num<v2.size())
        {
            if(v2[num][15]==strStTemp)
            {
                v3.push_back(v2[num]);
                if(v2[num][14]=="F" )
                         break;
                
                   
            }
           
            num++;
        }
        if(v3[0][14]=="S" && v3[v3.size()-1][14]=="F")
        {
                    timeTemp1=stod(v3[0][0]);
                    timeTemp2=stod(v3[v3.size()-1][0]);
                    timeTemp=timeTemp2-timeTemp1;
                    sum+=timeTemp;
                    if(timeTemp>max)
                        max=timeTemp;
                    if(timeTemp<min)
                        min=timeTemp;
        }
       
        /*for(int j=0;j<v3.size();j++)
        {
            for (vector<string>::const_iterator p = v3[j].begin(); p != v3[j].end(); p++)
            {
            //cout<<*(v2[j].end()-1);
            //cout<<*p[v2[j].length()-1]<<" ";
            cout<<*p;
             }
             cout<<endl;
        }*/     
    }
    cout<<"maximum time:"<<max<<endl;
    cout<<"minimum time:"<<min<<endl;
    cout<<"meam time:"<<sum/startCount<<endl;
     
}
//数据包数量判断
void PackAlysis(){


    int PackMin=2147483647;
    int PacketTemp=0;
    int PackMax=0;
    int PackSum=0;
    int PackMean=0;
    //计算数据包数量大小
    //v3.clear();
    for(int i=1;i<=startCount;i++)
    {
        PacketTemp=0;
        string strStTemp;
        strStTemp=to_string(i);
        int num=0;
        v3.clear();
        while(num<v2.size())
        {
            if(v2[num][15]==strStTemp)
            {
                v3.push_back(v2[num]);
                if(v2[num][14]=="F")
                         break;
            }
            num++;
           
        }
         if(v3[0][14]=="S" && v3[v3.size()-1][14]=="F")
        {
            PacketTemp=v3.size()-1;
            PackSum+=PacketTemp;
            if(PacketTemp>PackMax)
                PackMax=PacketTemp;
            if(PacketTemp<PackMin)
                PackMin=PacketTemp;
        }
    }

    cout<<"maximum packet size:"<<PackMax<<endl;
    cout<<"minimum packet size:"<<PackMin<<endl;
    cout<<"meam packet size:"<<(double)PackSum/startCount<<endl;
}
//数据大小分析
void DataAlysis(){


    //数据大小判断
    long long dataTemp=0;
    long long dataMin=2147483647;
    long long dataMax=0;
    long long dataSum=0;
    for(int i=1; i<=startCount;i++)
    {
        dataTemp=0;
        string strStTemp;
        strStTemp=to_string(i);
        int num=0;
        v3.clear();
        while(num<v2.size())
        {
            
         if(v2[num][15]==strStTemp)
            {
                v3.push_back(v2[num]);
                dataTemp+=((stoll(v2[num][10]))-(stoll(v2[num][8])));
                if(v2[num][14]=="F")
                         break;
            }
            num++;
        }
         if(v3[0][14]=="S" && v3[v3.size()-1][14]=="F")
        {
            dataSum+=dataTemp;
            if(dataTemp>dataMax)
                dataMax=dataTemp;
            if(dataTemp<dataMin)
                dataMin=dataTemp;
        }
    }
    
   
    cout<<"maximum data size:"<<dataMax<<endl;
    cout<<"minimum data size:"<<dataMin<<endl;
    cout<<"meam data size:"<<(double)dataSum/startCount<<endl;
}
//特定数据包大小查找
void DownlodeCheck(){


    //查询特定数据包大小的连接
    long long downlodeData=0;
    string serverIp="";
    int datasize=0;
    int dataflag=0;
    for(int i=1; i<=startCount;i++)
    {
        downlodeData=0;
        string strStTemp;
        strStTemp=to_string(i);
        int num=0;
        int flag=0;
        v3.clear();
        while(num<v2.size())
        {
            
         if(v2[num][15]==strStTemp && flag==0)
            {
                serverIp=v2[num][3];
                flag=1;
            }
         if(v2[num][15]==strStTemp)
            {
                v3.push_back(v2[num]);
                //dataTemp+=((stoll(v2[num][10]))-(stoll(v2[num][8])));
                if(v2[num][1]==serverIp)
                    downlodeData+=((stoll(v2[num][10]))-(stoll(v2[num][8])));
                if(v2[num][14]=="F")
                         break;
            }
            num++;
        }
         if(v3[0][14]=="S" && v3[v3.size()-1][14]=="F")
        {
            if((downlodeData/1024.0)>(downlodeData/1024))
                datasize=(downlodeData/1024)+1;
            else
                datasize=downlodeData/1024;
           //   cout<<datasize<<" ";
            if(datasize==33)
            {
                dataflag=1;
                break;
            }
        }
    }
    //cout<<v3[0][6];
    if(dataflag==1)
    {
        cout<<"The source port number is:"<<v3[0][6]<<endl;
        cout<<"The IP address of the Web server is:"<<v3[0][3]<<endl;
    }
    else
        cout<<"Not find 33KB TCP connection"<<endl;

    //
}


int main(){
    //cout<<countlines();     
   // ReadDataFromFileLBLIntoCharArray();
    ReadDataFromFileLBLIntoString();
    TimeAlysis();
    PackAlysis();
    DataAlysis();
    DownlodeCheck();
    return 0;
}