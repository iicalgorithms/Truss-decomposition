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

using namespace std;

char * file_1;
char * file_2;

map<pair<int,int>,int> truss_file_1;
map<pair<int,int>,int> truss_file_2;
string temp;
int node_num_1,edge_num_1;
int node_num_2,edge_num_2;

int main(int argc,char *argv[]){
    file_1 = argv[1];
    file_2 = argv[2];
    ifstream in_1(file_1);
    if(!in_1.is_open()) cout<<"fail to open file 1!\n"<<endl;
    while(getline(in_1,temp)){
        istringstream str(temp);
        if(temp[0] == '#'){
            if(temp[2]=='N'&&temp[3]=='o'&&temp[4]=='d'){
                string t1,t2,t3;
                str>>t1>>t2>>node_num_1>>t3>>edge_num_1;
                //cout<<t1<<" "<<t2<<" "<<node_num_1<<" "<<t3<<" "<<edge_num_1<<endl;
            }else continue;
        }else break;
    }
    do{
        istringstream str(temp);
        if(temp[0] == 'N' ) continue;
        if(temp[0] != '#'){
			int st,ed,tr;
            str>>st>>ed>>tr;
            pair<int,int> tempPair = make_pair(min(st,ed),max(st,ed));
            if(truss_file_1[tempPair]==0) truss_file_1[tempPair] = tr;
            else if(truss_file_1[tempPair]!=tr) cout<<"File 1:Wrong Truss at ("<<st<<","<<ed<<"), Not same as ("<<ed<<","<<st<<")."<<endl;
        }
    }while(getline(in_1,temp));
    in_1.close();

    ifstream in_2(file_2);
    if(!in_2.is_open()) cout<<"fail to open file 2!\n"<<endl;
    while(getline(in_2,temp)){
        istringstream str(temp);
        if(temp[0] == '#'){
            if(temp[2]=='N'&&temp[3]=='o'&&temp[4]=='d'){
                string t1,t2,t3;
                str>>t1>>t2>>node_num_2>>t3>>edge_num_2;
                //cout<<t1<<" "<<t2<<" "<<node_num_1<<" "<<t3<<" "<<edge_num_2<<endl;
            }else continue;
        }else break;
    }
    do{
        istringstream str(temp);
        if(temp[0] == 'N' ) continue;
        if(temp[0] != '#'){
			int st,ed,tr;
            str>>st>>ed>>tr;
            pair<int,int> tempPair = make_pair(min(st,ed),max(st,ed));
            if(truss_file_2[tempPair]==0) truss_file_2[tempPair] = tr;
            else if(truss_file_2[tempPair]!=tr) cout<<"File 2:Wrong Truss at ("<<st<<","<<ed<<"), Not same as ("<<ed<<","<<st<<")."<<endl;
        }
    }while(getline(in_2,temp));
    in_2.close();

    if(edge_num_1!=edge_num_2) cout<<"Different edge number:" <<edge_num_1 <<" "<<edge_num_2<<endl;
    if(node_num_1!=node_num_2) cout<<"Different node number:" <<node_num_1 <<" "<<node_num_2<<endl;

    map<pair<int,int>,int>::iterator it;
    for(it = truss_file_1.begin();it!=truss_file_1.end();it++){
        int ts1 = (*it).second;
        int ts2 = truss_file_2[(*it).first];
        if(ts1!=ts2) {
            cout<<"Different Truss at ("<<(*it).first.first<<","<<(*it).first.second<<"):"<<ts1<<" vs. "<<ts2<<endl;
        }
    }
    cout<<"Finished"<<endl;
}

