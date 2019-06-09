#include "Graph.h"
#include <fstream>
#include <iomanip>

Graph::Graph(){}

void Graph::buildGraph(int edge_num,int node_num){
    edgeNum = edge_num;
    nodeNum = node_num;
    nodeList = new GraphNode[node_num]();
    bin = new vector<pair<int,int> >[node_num-1](); 
}

int Graph::find(int id){
    map<int,int>::iterator iter;
    iter = findId.find(id);
    if(iter != findId.end())
        return iter->second;
    else
        return -1;
}

void Graph::addEdge(int stId,int edId){
    if(stId == edId){
        return;
    }
    int findSt = find(stId),findEd = find(edId);
    if(findSt<0){
        findSt = cnt;
        nodeList[cnt].id = stId;
        findId[stId] = cnt++;
    }
    if(findEd<0){
        findEd = cnt;
        nodeList[cnt].id = edId;
        findId[edId] = cnt++;
    }
    NeiNode *p = nodeList[findSt].firstNei;
    while(p){
        if(findEd == p->id && p->valid) return;
        p = p->next;
    }

    NeiNode *e = new NeiNode;
    e->id = findEd;
    e->next = nodeList[findSt].firstNei;
    e->sup = 0;
    nodeList[findSt].firstNei = e;
    nodeList[findSt].degree ++;

    e = new NeiNode;
    e->id = findSt;
    e->next = nodeList[findEd].firstNei;
    e->sup = 0;
    nodeList[findEd].firstNei = e;
    nodeList[findEd].degree ++;
}

void Graph::addEdge(int stId,int edId,double pr){
    edgeNum ++;
    if(stId == edId){
        return;
    }
    int findSt = find(stId),findEd = find(edId);
    if(findSt<0){
        findSt = cnt;
        nodeList[cnt].id = stId;
        findId[stId] = cnt++;
    }
    if(findEd<0){
        findEd = cnt;
        nodeList[cnt].id = edId;
        findId[edId] = cnt++;
    }
    NeiNode *p = nodeList[findSt].firstNei;
    while(p){
        if(findEd == p->id && p->valid) return;
        p = p->next;
    }

    NeiNode *e = new NeiNode;
    e->id = findEd;
    e->next = nodeList[findSt].firstNei;
    e->sup = 0;
    e->Pr = pr;
    nodeList[findSt].firstNei = e;
    nodeList[findSt].degree ++;

    e = new NeiNode;
    e->id = findSt;
    e->next = nodeList[findEd].firstNei;
    e->sup = 0;
    e->Pr = pr;
    nodeList[findEd].firstNei = e;
    nodeList[findEd].degree ++;
}

void Graph::initSup(){
    for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->sup = 0;
                NeiNode *i = nodeList[k].firstNei;
                while(i){
                    if(i->valid && i->id != nei->id){
                        NeiNode *j = nodeList[nei->id].firstNei;
                        while(j){
                            if(j->valid && j->id!= k){
                                if(j->id == i->id) nei->sup++;
                            }
                            j = j->next;
                        }
                    }
                    i = i->next;
                }
                nei->tuss = nei->sup+2;
                if(k<nei->id)
                  bin[nei->sup ].push_back(make_pair(k,nei->id));
            }
            nei = nei->next;
        }
    }
    
}

void Graph::setTuss(int stId,int edId,int k){
    NeiNode *p = nodeList[stId].firstNei;
    while (p)
    {
        if(p->id == edId){
            p->tuss = k;
        }
        p = p->next;
    }
    p = nodeList[edId].firstNei;
    while (p)
    {
        if(p->id == stId){
            p->tuss = k;
        }
        p = p->next;
    }
}


void Graph::remEdge(int stId,int edId){
    int st = min(stId,edId);
    int ed = max(stId,edId);
    NeiNode *i = nodeList[st].firstNei;
    while (i)
    {
        if(i->valid && i->id!= ed)
        {
            NeiNode *j = nodeList[ed].firstNei;
            while(j)
            {
                if(j->valid && j->id!= st)
                {
                    if(j->id == i->id){
                        i->sup --;
                      //  cout<<"sup of "<<nodeList[st].id<<" "<<nodeList[i->id].id<<" down"<<endl;
                        NeiNode *ip = nodeList[i->id].firstNei;
                        while(ip){
                            if(ip->id == st) {
                                ip->sup--;
                              //  cout<<"sup of "<<nodeList[i->id].id<<" "<<nodeList[ip->id].id<<" down"<<endl;
                                for(int k=0;k<bin[i->sup+1 ].size();k++){
                                    if (bin[i->sup+1][k].first == min(i->id,st) && bin[i->sup+1][k].second == max(i->id,st))
                                    {
                                      //  cout<<"erase "<<i->sup+1<<" in "<<nodeList[bin[i->sup+1][k].first].id <<" "<<nodeList[bin[i->sup+1][k].second].id<<endl;
                                        bin[i->sup+1].erase(bin[i->sup+1].begin()+k);
                                        bin[i->sup].push_back(make_pair(min(i->id,st),max(i->id,st)));
                                        break;
                                    }
                                    
                                }
                                break;
                            }
                            ip = ip->next;
                        }
                        j->sup --;
                        //cout<<"sup of "<<nodeList[ed].id<<" "<<nodeList[j->id].id<<" down"<<endl;
                        ip = nodeList[j->id].firstNei;
                        while(ip){
                            if(ip->id == ed) {
                                ip->sup--;
                               // cout<<"sup of "<<nodeList[j->id].id<<" "<<nodeList[ip->id].id<<" down"<<endl;
                                for(int k=0;k<bin[j->sup+1 ].size();k++){
                                    if (bin[j->sup+1][k].first == min(j->id,ed) && bin[j->sup+1][k].second == max(j->id,ed) )
                                    {
                                       // cout<<"erase "<<j->sup+1<<" in "<<nodeList[bin[j->sup+1][k].first].id <<" "<<nodeList[bin[j->sup+1][k].second].id<<endl;
                                        bin[j->sup+1].erase(bin[j->sup+1].begin()+k);
                                        bin[j->sup].push_back(make_pair(min(j->id,ed),max(j->id,ed)));
                                        break;
                                    }
                                    
                                }
                                break;
                            }
                            ip = ip->next;
                        }
                        break;
                    }
                }else if(j->id == st) j->valid = 0;
                j = j->next;
            }
        }else if(i->id == ed) i->valid = 0;
        i = i -> next;
    }
    
}

void Graph::greed(){
    int k = 2;
    while(1)
    {
        int minSup = 0;
        while(bin[minSup].size()==0) {
            minSup++;
            if(minSup > nodeNum-2) return;
        }
        
        //cout<<"minSup:"<<minSup<<endl;
        while(minSup>k-2) k++;
        int stId = bin[minSup][0].first;
        int edId = bin[minSup][0].second;
        bin[minSup].erase(bin[minSup].begin()+0);
        //cout<<nodeList[stId].id<<" "<<nodeList[edId].id<<endl;
        setTuss(stId,edId,k);
        remEdge(stId,edId);
    }
}

void Graph::distribute(){
    int maxIter = 99999999;
    int cnt_iter = 0;
    while(maxIter--){
        int changeEdgeNum = 0;
        int changeNodeNum = 0;
        for(int k=0;k<nodeNum;k++){
            NeiNode *nei = nodeList[k].firstNei;
            int oldChangeEdgeNum = changeEdgeNum;
          //  cout<<"Node:"<<k<<endl;
            while(nei){
                if(nei->valid){
                    vector<int> newTuss;
                    int maxTuss = -1;
                    NeiNode *i = nodeList[k].firstNei;
                    while (i)
                    {
                        if(i->valid&&i->id!=nei->id){
                            NeiNode *j = nodeList[nei->id].firstNei;
                            while (j)
                            {
                                if(j->valid&&j->id!=k){
                                    if(j->id == i->id){
                                        newTuss.push_back(min(i->tuss,j->tuss));
                                        maxTuss = max(maxTuss,min(i->tuss,j->tuss));
                                    }
                                }
                                j = j->next;
                            }
                            
                        }
                        i = i->next;
                    }
                   // cout<<"nei(k-2 >=k):"<<nei->id<<endl;
                    while(maxTuss>1){
                        int cnt = 0;
                        for(int q = 0;q<newTuss.size();q++){
                     //       cout<<newTuss[q]<<" ";
                            if(newTuss[q]>=maxTuss)
                                cnt++;
                        }
                        if(cnt>=maxTuss-2) break;
                        maxTuss --;
                    }
                 //   cout<<endl;
                    if(maxTuss>1&&maxTuss<nei->tuss){
                        nei->tuss = maxTuss;
                        changeEdgeNum++;
                    }
                }
                nei = nei->next;
            }
            if(changeEdgeNum-oldChangeEdgeNum>0)changeNodeNum++;
        }
        //cout<<"第"<<++cnt_iter<<"轮： 更新点数-"<<changeNodeNum<<"   更新边数-"<<changeEdgeNum<<endl;
        cout<<"Step "<<setw(4)<<++cnt_iter<<":  Changed Point Num = "<<setw(4)<<changeNodeNum<<"  Changed Edge Num:"<<setw(4)<<changeEdgeNum<<endl;
        if(changeEdgeNum == 0){
            break;
        }
    }
    
}

void Graph::output(){
    cout<<"Node:"<<nodeNum<<" Edge:"<<edgeNum<<endl;
    for (int i = 0; i < nodeNum; i++)
    {
        cout<<"Point:"<<nodeList[i].id<<" Degree:"<<nodeList[i].degree<<"  Arc:"<<endl;
        NeiNode  *nei = nodeList[i].firstNei;
        while (nei)
        {
            cout<<"Id:"<<nodeList[nei->id].id<<"  Valid:"<<nei->valid<<" Sup:"<<nei->sup<<" Tuss:"<<nei->tuss<<endl; 
            nei = nei->next;
        }
    }

    cout<<"Bin:"<<endl;
    for (int i = 0; i < nodeNum-1; i++)
    {
        cout<<"i:"<<i<<"    >>>>>>  ";
        for (int j = 0;j<bin[i].size();j++)
        {
            cout<<"<"<<nodeList[bin[i][j].first].id<<","<<nodeList[bin[i][j].second].id<<">"<<" ";
        } 
        cout<<endl;
    }
    
    
}

void Graph::writeFile(char * filename){
    ofstream outFile(filename);
    outFile<<"Node 1\tNode 2\tTruss"<<endl;
    outFile<<"# Nodes:"<<nodeNum<<" Edges:"<<edgeNum<<endl;
    for (int i = 0; i < nodeNum; i++)
    {
        NeiNode  *nei = nodeList[i].firstNei;
        while (nei)
        {
            outFile<<setw(12)<<left<<nodeList[i].id<<"\t"<<setw(12)<<left<<nodeList[nei->id].id<<"\t"<<setw(12)<<left<<nei->tuss<<endl;
            nei = nei->next;
        }
    }
}


//初始化Pr SuP  每条边的sup值用dp计算
void Graph::initPrSup(){
   
    for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->PrSup = computePrSup(k,nei->id,nei->Pr);
                nei->tuss = nei->PrSup + 2;
                if(k<nei->id){
                    edge e(k,nei->id,nei->PrSup);
                    edgeSet.insert(e);
                    //cout<<k<<" "<<nei->id<<" "<<nei->PrSup<<endl;
                }
                //nei->tuss = nei->PrSup+2;
            }
            nei = nei->next;
        }
    }
}

//dp及计算sup dp[i][j] = dp[i-1][j-1]p(st,j)p(ed,j) +  (1-p(st,j)p(ed,j))dp[i-1][j-1]
//前i个三角形里面有j个是存在的
double Graph::computePrSup(int st,int ed,double p){
        NeiNode *i = nodeList[st].firstNei;
       
       // cout<<"<"<<st<<","<<ed<<">:";
       int ek = 0;
       vector<double> pr;
        while(i){
            if(i->valid && i->id != ed){
                NeiNode *j = nodeList[ed].firstNei;
                while(j){
                    if(j->valid && j->id!= st){
                        if(j->id == i->id){
                            pr.push_back(i->Pr*j->Pr);
                            ek ++;
                        }
                    }
                    j = j->next;
                }
            }
            i = i->next;
        }
       // cout<<" = "<<PrSup<<endl;
        double e[ek+1];
        double f[ek+1][ek+1]; 
        f[0][0] = 1;
        // for(int k =1;k<=ek+1;k++) f[0][k] = 0;
        for(int l = 0;l<=ek;l++){
            for(int k = l+1;k<=ek;k++){
                f[k][l] = 0;
            }
        }
        for(int l=1;l<=ek;l++){
            f[0][l] = (1-pr[l-1])*f[0][l-1];
        }
        for(int k = 1;k<=ek;k++){
            for(int l=1;l<=ek;l++){
                f[k][l] = pr[l-1]*f[k-1][l-1] + (1-pr[l-1])*f[k][l-1]; 
            }
        }
        e[0] = 1;
        for(int t=1;t<=ek;t++){
            e[t] = e[t-1]-f[t][ek];//\sum pr(sup>=t) sup大于等于t的所有情况的概率 
            //cout<<"t:"<<t<<" e[t]:"<<e[t]<<" e[t-1]:"<<e[t-1]<<" p:"<<p<<" e[t]*p:"<<e[t]*p<<" f[t][ek]:"<<f[t][ek]<<" gamma:"<<gamma<<endl;
            if(e[t]*p < gamma) return t-1;
        }

        return ek;
}

void Graph::PrGreed(){
    int k = 2;
    while(1)
    {
        if(edgeSet.size() == 0) break;
        double minSup = (*edgeSet.begin()).PrSup;
        
        //cout<<"minSup:"<<minSup<<endl;
        while(minSup>k-2) k++;
        int stId = (*edgeSet.begin()).st;
        int edId = (*edgeSet.begin()).ed;
        edgeSet.erase(*edgeSet.begin());
        //cout<<nodeList[stId].id<<" - "<<nodeList[edId].id<<endl;
        setTuss(stId,edId,k);
        PrRemEdge(stId,edId);
    }
}

void Graph::PrRemEdge(int stId,int edId){
    int st = min(stId,edId);
    int ed = max(stId,edId);
    deleteEdge(st,ed);
    NeiNode *i = nodeList[st].firstNei;
    while (i)
    {
        if(i->valid && i->id!= ed)
        {
            NeiNode *j = nodeList[ed].firstNei;
            while(j)
            {
                if(j->valid && j->id!= st)
                {
                    if(j->id == i->id){
                        edge temp1(min(i->id,st),max(i->id,st),i->PrSup);
                        edgeSet.erase(temp1);
                        i->PrSup = computePrSup(st,i->id,i->Pr);
                        edge temp2(min(i->id,st),max(i->id,st),i->PrSup);
                        edgeSet.insert(temp2);
                        
                        edge temp3(min(j->id,ed),max(j->id,ed),j->PrSup);
                        edgeSet.erase(temp3);
                        j->PrSup = computePrSup(ed,j->id,j->Pr);
                        edge temp4(min(j->id,ed),max(j->id,ed),j->PrSup);
                        edgeSet.insert(temp4);
                        //cout<<"sup of "<<nodeList[st].id<<" "<<nodeList[i->id].id<<" down to (i,j):"<<i->PrSup<<" "<<j->PrSup<<endl;
                        break;
                    }
                }
                j = j->next;
            }
        }
        i = i -> next;
    }
    
}

void Graph::deleteEdge(int st,int ed){
    NeiNode *i = nodeList[st].firstNei;
    while (i)
    {
        if(i->valid && i->id== ed)
        {
            i->valid = 0;
        }
        i = i -> next;
    }
    NeiNode *j = nodeList[ed].firstNei;
    while(j)
    {
        if(j->valid && j->id== st)
        {
            j->valid = 0;
        }
        j = j->next;
    }
        
}
void Graph::outputSet(){
    set<edge>::iterator iter = edgeSet.begin();
    cout<<"Set size:"<<edgeSet.size()<<endl;
    while (iter!=edgeSet.end())
    {
        cout<<(*iter).st<<" "<<(*iter).ed<<endl;
        iter++;
    }
}

void Graph::PrDistribute(){
    int maxIter = 99999999;
    int cnt_iter = 0;
    while(maxIter--){
        int changeEdgeNum = 0;
        int changeNodeNum = 0;
        for(int k=0;k<nodeNum;k++){
            NeiNode *nei = nodeList[k].firstNei;
          //  cout<<"Node:"<<k<<endl;
            int oldChangeEdgeNum = changeEdgeNum;
            while(nei){
                if(nei->valid){
                    vector<int> newTuss;
                    int maxTuss = -1;
                    NeiNode *i = nodeList[k].firstNei;
                    while (i)
                    {
                        if(i->valid&&i->id!=nei->id){
                            NeiNode *j = nodeList[nei->id].firstNei;
                            while (j)
                            {
                                if(j->valid&&j->id!=k){
                                    if(j->id == i->id){ //形成了一个三角形
                                        newTuss.push_back(min(i->tuss,j->tuss));//放入三角形临边较小的truss
                                        maxTuss = max(maxTuss,min(i->tuss,j->tuss));//记录数组中最大的maxTruss
                                    }
                                }
                                j = j->next;
                            }
                            
                        }
                        i = i->next;
                    }
                   // cout<<"nei(k-2 >=k):"<<nei->id<<endl;
                    while(maxTuss>1){
                        int cnt = 0;
                        for(int q = 0;q<newTuss.size();q++){
                     //       cout<<newTuss[q]<<" ";
                            if(newTuss[q]>=maxTuss)
                                cnt++;
                        }
                        if(cnt>=maxTuss-2) break;
                        maxTuss --;
                    }
                 //   cout<<endl;
                    if(maxTuss>1&&maxTuss<nei->tuss){
                        nei->tuss = maxTuss;
                        changeEdgeNum++;
                    }
                }
                nei = nei->next;
            }
            if(changeEdgeNum-oldChangeEdgeNum>0)changeNodeNum++;
        }
        //cout<<"第"<<++cnt_iter<<"轮： 更新点数-"<<changeNodeNum<<"   更新边数-"<<changeEdgeNum<<endl;
        cout<<"Step "<<setw(4)<<++cnt_iter<<":  Changed Point Num = "<<setw(4)<<changeNodeNum<<"  Changed Edge Num:"<<setw(4)<<changeEdgeNum<<endl;
        if(changeEdgeNum == 0){
            break;
        }
    }
    
}