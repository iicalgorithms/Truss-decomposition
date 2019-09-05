#include "Graph.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <map>
#define random(x) rand()%x;
using namespace std;

Graph G;
int node_num ,edge_num ;//边数，点数

string temp;//文件输出临时存储字符串

//在进行随机插入or删除的时候，需要进行抽样
int changeNum;//抽样数目
map<int,int > rdm; //用于随机抽样,map去重
vector<int > shhuffle_rdm;//用于随机抽样,进行shuffle随机排列
vector<int> v;//保存抽样出的数据的下标
vector<int > stId;//要插入/删除边的起点
vector<int > edId;//要插入/删除边的终点

char * filename ;//文件路径
int method = -1;//求解方法 0-贪心 1-分布式
int graphType = -1;//图类型：0-static graph  1-temporal graph
int computeType = -1;//0-不插入不删除 1-插入：逐条初始化并分布式计算 2-插入：批次插入初始化并分布式计算 3-插入：批次插入，初始化为Sup+2并分布式计算 4-删除：批次删除并分布式计算 5-删除：初始化min{sup+2，truss(e)}并分布式计算
int number = -1; //插入/删除 数量级
char write;  //'w'-将truss值写入"output.txt"文件  

int Pow(int k){
	int res = 1;
	while(k--) res=res*10;
	return res;
}

string getName(char *filename){
    int len = strlen(filename);
    int st=-1,ed = -1;
    int fg1 = 0,fg2 = 0;
    for(int i = len-1;i>=0;i--){
        if(filename[i]=='.'&&!fg1) {
            fg1 = 1;
            ed = i-1;
        }
        if((filename[i]=='/'||filename[i]=='\\')&&!fg2) {
            fg2 = 1;
            st = i+1;
        }
    }
    if(st == -1) st = 0;
    string res = "";
    for(int i = st;i<=ed;i++) res += filename[i];
    return res;
}

int main(int argc,char * argv[]){
	//strcat(filename,argv[1]);
	filename = argv[1];
	method = argv[2][0] - '0';
	graphType = argv[3][0] - '0';
	computeType = argv[4][0] - '0';
	number = argv[5][0] - '0';
	write = argv[6][0];

    ifstream in(filename);
    if(!in.is_open()) cout<<"fail to open file!\n"<<endl;

    if(argv[2][0] == '2'|| argv[2][0] == '3'){//概率图
        getline(in,temp);
        istringstream strs(temp);
        int node_num;
        int node;
        strs>>node_num>>node;
        G.buildGraph(0,node_num);
    }
	
	//读入文件头，确定点数和边数，建图
	while(getline(in,temp)){
        istringstream str(temp);
        if(temp[0] == '#'){
            if(temp[2]=='N'&&temp[3]=='o'&&temp[4]=='d'){
                string t1,t2,t3;
                str>>t1>>t2>>node_num>>t3>>edge_num;
                //cout<<temp<<endl;
                //cout<<t1<<" "<<t2<<" "<<node_num<<" "<<t3<<" "<<edge_num<<"  ss"<<endl;
                G.buildGraph(edge_num,node_num);
            }else continue;
        }else break;
    }

	if(graphType!=2){
		changeNum = number;
		changeNum = Pow(changeNum);
		if(graphType == 0 ){
			for(int i=0;i<edge_num;i++) shhuffle_rdm.push_back(i);
			random_shuffle(shhuffle_rdm.begin(),shhuffle_rdm.end());
			for(int i=0;i<changeNum;i++) v.push_back(shhuffle_rdm[i]);
			if(!v.empty()) sort(v.begin(),v.end());
		}else if(graphType == 1){
			for(int i=0;i<changeNum;i++){
				v.push_back(edge_num-changeNum+i);
			}
		}
	}
	//for (int i = 0; i < v.size(); i++) 		cout<<v[i]<<endl;

	int rNum = 0;
	int pNum = 0;
    do{
        istringstream str(temp);
        if(temp[0] != '#'){
			int needInsert = 0;
			if(computeType<=3 && computeType>=1 && !v.empty() && v[pNum] == rNum && pNum<changeNum ){	
				pNum++;
				needInsert = 1;
			}		
			int st,ed;
            if(argv[2][0] == '2'|| argv[2][0] == '3'){//如果是概率边的情况
                double pr;
                str>>st>>ed>>pr;
				if(!needInsert) G.addEdge(st,ed,pr);
            }else{
                str>>st>>ed;
                if(!needInsert) {
					G.addEdge(st,ed);
				}
            }
			if(needInsert||computeType==4||computeType==5){//如果不需要插入或者删除
				stId.push_back(min(st,ed));
				edId.push_back(max(st,ed));				
			}
			rNum ++;
			//cout<<rNum<<endl;
        }
    }while(getline(in,temp));

	if(method == 0){
		G.initSup();
		G.cover();
        G.greed();
	}else if(method == 1){
		cout<<"start"<<endl;
		G.initSup();
		G.cover();
        G.distribute();
	}else cout<<"Wrong modle."<<endl;

	if(graphType == 2){
		G.initSuperSup();
		G.initConstrainSup();
	}

	G.startCntSteps = 1;
	switch (computeType)
	{
	case 0:
		break;
	case 1:
		for(int i=0;i<changeNum;i++){
			G.dynamicInsert(stId[i],edId[i]);
			G.distribute();
		}
		break;
	case 2:
		for(int i=0;i<changeNum;i++) G.dynamicInsert(stId[i],edId[i]);
		G.distribute();
		break;
	case 3:
		for(int i=0;i<changeNum;i++) G.addEdge(stId[i],edId[i],0);
		G.initSup();
		G.cover();
		G.distribute();
		break;
	case 4:
		for(int i=0;i<changeNum;i++){	
			G.dynamicDelete(stId[i],edId[i]);
			G.distribute();
		}
		break;
	case 5:
		for(int i=0;i<changeNum;i++){
			G.supInitDelete(stId[i],edId[i]);
		}
		G.distribute();
		break;
	default:
		if(graphType!=2)	
			cout<<"Wrong modle!"<<endl;
		break;
	}
	if(computeType!=0 &&graphType!=2 ) G.outputDynamicInfo(computeType);
    
    if(write == 'w'){
		string str = getName(filename);
		str =  argv[5][0] + str;
		//cout<<str<<endl;
        G.writeFile((char *)str.c_str());
    }
}