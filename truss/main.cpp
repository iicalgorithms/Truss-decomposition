#include "Graph.h"
#include <bits/stdc++.h>
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
#define random(x) rand()%x;
using namespace std;

Graph G;
int node_num ,edge_numm ;
int m;//求解方法
int changeNum;//抽样数目
int graphmodle = -1;
int addOrSub=-1;//加还是减

vector<int > rdm; //用于随机抽样,进行shuffle随机排列
vector<int> v;//保存抽样出的数据的下标
vector<int > stId;//要插入/删除边的起点
vector<int > edId;//要插入/删除边的终点

int Pow(int k){
	int res = 1;
	while(k--) res=res*10;
	return res;
}

int main(int argc,char * argv[]){
	m = argv[2][0] - '0';
    ifstream in(argv[1]);
    if(!in.is_open()) cout<<"fail to open file!\n"<<endl;
    string temp;
    if(argv[2][0] == '2'|| argv[2][0] == '3'){//概率图
        getline(in,temp);
        istringstream strs(temp);
        int node_num;
        int node;
        strs>>node_num>>node;
        G.buildGraph(0,node_num);
    }
	int cnt = 3;
	if(argv[cnt][0] == '1' || argv[cnt][0] == '2'){
		graphmodle = argv[cnt++][0]-'0';
	}
	if(argv[cnt][0] == '1' || argv[cnt][0] == '2'){
		addOrSub = argv[cnt++][0]-'0';
	} 
	
	while(getline(in,temp)){
        istringstream str(temp);
        if(temp[0] == '#'){
            if(temp[2]=='N'&&temp[3]=='o'&&temp[4]=='d'){
                string t1,t2,t3;
                str>>t1>>t2>>node_num>>t3>>edge_numm;
                //cout<<temp<<endl;
                //cout<<t1<<" "<<t2<<" "<<node_num<<" "<<t3<<" "<<edge_numm<<"  ss"<<endl;
                G.buildGraph(edge_numm,node_num);
            }else continue;
        }else break;
    }
	
	if(argv[cnt][0] != 'r' ){
		changeNum = argv[cnt++][0] - '0';
		changeNum = Pow(changeNum);
		srand((int)time(0));
		//srand(100);
		//changeNum *= (rand()%10+1);
		if(changeNum > edge_numm) changeNum = edge_numm;
		for(int i=0;i<edge_numm;i++){
			rdm.push_back(i);
		}
		random_shuffle(rdm.begin(),rdm.end());
		for(int i=0;i<changeNum;i++){
			v.push_back(rdm[i]);
		}
	}
	cout<<"addorsub:"<<addOrSub<<endl;
	if(!v.empty()) sort(v.begin(),v.end());
	int rNum = 0;
	int pNum = 0;
    do{
        istringstream str(temp);
        if(temp[0] != '#'){
			int con = 0;
			if( !v.empty()&&v[pNum] == rNum && pNum<changeNum && addOrSub>=0){	
				pNum++;
				con = 1;
			}		
			int st,ed;
            if(argv[2][0] == '2'|| argv[2][0] == '3'){//如果是概率边的情况
                double pr;
                str>>st>>ed>>pr;
				if(!con) G.addEdge(st,ed,pr);
            }else{
                str>>st>>ed;
                if(!con) {
					G.addEdge(st,ed);
				}
            }
			if(con){
				stId.push_back(min(st,ed));
				edId.push_back(max(st,ed));				
			}

			rNum ++;
        }
    }while(getline(in,temp));

	switch(m){
		case 0:
		G.initSup();
		G.cover();
        G.greed();
		break;

		case 1:
		G.initSup();
		G.cover();
        G.distribute();
		break;

		case 2:
		G.initPrSup();//概率图中 sup和cover暂时没有分开
        G.PrGreed();
		break;

		case 3:
		G.initPrSup();
        G.PrGreed();
		break;

		case 4:
		G.initPrSup();
        G.PrDistribute();
		break;

		default:
		cout<<"Wrong modle."<<endl;
		break;
	}
	//动态图添边，删边之前，计算Truss
		//G.output();
    for(int i=0;i<changeNum;i++)
    {
		if(addOrSub==1)
			G.dynamicInsert(stId[i],edId[i]);
		else if(addOrSub==2)
			G.dynamicDelete(stId[i],edId[i]);
    }
	
	if(addOrSub>=0){
		G.outputDynamicInfo(addOrSub);
        G.distribute();
	}

    if(argv[cnt][0] == 'r'){
        G.writeFile("output.txt");
    }
}