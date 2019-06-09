#include "Graph.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <sstream>
using namespace std;

Graph G;

int main(int argc,char * argv[]){
    ifstream in(argv[1]);
    if(!in.is_open()) cout<<"fail to open file!\n"<<endl;
    string temp;
    if(argv[2][0] == '2'){
        getline(in,temp);
        istringstream strs(temp);
        int node_num;
        int node;
        strs>>node_num>>node;
        G.buildGraph(0,node_num);
    }
    while(getline(in,temp)){
        istringstream str(temp);
        if(temp[0] == '#'){
            if(temp[2]=='N'&&temp[3]=='o'&&temp[4]=='d'){
                int node_num ,edge_numm ;
                string t1,t2,t3;
                str>>t1>>t2>>node_num>>t3>>edge_numm;
                //cout<<temp<<endl;
                //cout<<t1<<" "<<t2<<" "<<node_num<<" "<<t3<<" "<<edge_numm<<"  ss"<<endl;
                G.buildGraph(edge_numm,node_num);
            }else continue;
        }else{
            if(argv[2][0] == '2'){
                int st,ed;
                double pr;
                str>>st>>ed>>pr;
                G.addEdge(st,ed,pr);
                //cout<<st<<" "<<ed<<" "<<pr<<endl;
            }else{
                int st,ed;
                str>>st>>ed;
                G.addEdge(st,ed);
            }
        }
    }
    if(!(int)(argv[2][0]-'0')){
        G.initSup();
        G.greed();
        //G.output();
    }else if(!(int)(argv[2][0]-'1')){
        G.initSup();
        G.distribute();
        //G.output();
    }else if(argv[2][0] == '2'){
        G.initPrSup();
        G.PrGreed();
    }else{
        cout<<"Wrong modle."<<endl;
    }

    if(argv[3][0] == 'r'){
        G.writeFile("output.txt");
    }
}