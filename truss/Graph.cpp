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

int Graph::Find(int id){
    map<int,int>::iterator iter;
    iter = findId.find(id);
    if(iter != findId.end())
        return iter->second;
    else
        return -1;
}

void Graph::addEdge(int stId,int edId){
    int findSt = Find(stId),findEd = Find(edId);//map  将标号映射成下表
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
    NeiNode *p = nodeList[findSt].firstNei;//无重边
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
	
	changeEdge[make_pair(min(findSt,findEd),max(findSt,findEd))] = 0;
	visit[make_pair(min(findSt,findEd),max(findSt,findEd))] = 0;
}

void Graph::addEdge(int stId,int edId,double pr){
    edgeNum ++;
	if(stId == edId){//无自环
        return;
    }
    if(stId == edId){
        return;
    }
    int findSt = Find(stId),findEd = Find(edId);
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
	changeEdge[make_pair(min(findSt,findEd),max(findSt,findEd))] = 0;
	visit[make_pair(min(findSt,findEd),max(findSt,findEd))] = 0;
}

void Graph::initSup(){
    for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->sup = computeSup(k,nei->id);
            }
			edge e(k,nei->id,0);
            edgeSet.insert(e);
            nei = nei->next;
        }
    }
}

int Graph::computeSup(int st,int ed){
	int res = 0;
	NeiNode *i = nodeList[st].firstNei;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) res++;
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	return res;
}

void Graph::cover(){
	for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->tuss = nei->sup+2;
                if(k<nei->id)
                  bin[nei->sup ].push_back(make_pair(k,nei->id));
            }
            nei = nei->next;
        }
    }
}

//将边的truss设置为k
void Graph::setTuss(int stId,int edId,int k){
    NeiNode *p = nodeList[stId].firstNei;
    while (p)
    {
        if(p->id == edId){
            p->tuss = k;
			break;
        }
        p = p->next;
    }
    p = nodeList[edId].firstNei;
    while (p)
    {
        if(p->id == stId){
            p->tuss = k;
			break;
        }
        p = p->next;
    }
	cout<<"Set edge ("<<stId<<","<<edId<<") Truss in:"<<k<<endl;
}

int Graph::getTuss(int stId,int edId){
	NeiNode *p = nodeList[stId].firstNei;
	int res;
    while (p)
    {
        if(p->id == edId){
            res = p->tuss;
			break;
        }
        p = p->next;
    }
	p = nodeList[edId].firstNei;
    while (p)
    {
        if(p->id == stId){
            res = max(res,p->tuss);
			break;
        }
        p = p->next;
    }
}


int Graph::getSup(int stId,int edId){
	NeiNode *p = nodeList[stId].firstNei;
	int res;
    while (p)
    {
        if(p->id == edId){
            res = p->sup;
			break;
        }
        p = p->next;
    }
	p = nodeList[edId].firstNei;
    while (p)
    {
        if(p->id == stId){
            res = max(res,p->sup);
			break;
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
    outFile<<"# Nodes:"<<nodeNum<<" Edges:"<<edgeNum<<endl;
	outFile<<"Node 1\tNode 2\tTruss"<<endl;
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


//初始化Pr SuP  每条边的sup值用dp计算  并且将sup覆盖truss
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

bool Graph::inCntEdge(int st,int ed){
	int stId = min(st,ed);
	int edId = max(st,ed);
	int it = visit[make_pair(min(st,ed),max(st,ed))];;
	if(it == 0 ) return false ;
	return true;
}

void Graph::upAdjust_2(int st,int ed){
	NeiNode *i = nodeList[st].firstNei;
	int t = i->tuss;
	visit[make_pair(min(st,ed),max(st,ed))] = 1;;
	cout<<"fff"<< cntEdge.size()<<endl;
	cout<<"ADD@2:"<<min(st,ed)<<" "<<max(st,ed)<<"  "<<endl;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						if(i->tuss == t&&j->tuss>t && !inCntEdge(st,i->id) ){
							setTuss(st,i->id,1+getTuss(st,i->id));
							//changeEdge[make_pair(min(st,i->id),max(st,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							upAdjust_2(min(st,i->id),max(st,i->id));
							changeNodes.insert(st);
							changeNodes.insert(i->id);
							cntTotalChangedEdge++;
						}
						if(j->tuss == t&&i->tuss>t && !inCntEdge(ed,j->id) ){
							setTuss(ed,j->id,1+getTuss(ed,j->id));
							//changeEdge[make_pair(min(ed,i->id),max(ed,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							upAdjust_2(min(ed,i->id),max(ed,i->id));
							changeNodes.insert(ed);
							changeNodes.insert(j->id);
							cntTotalChangedEdge++;
						}
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	cout<<"add2 return"<<endl;
}

void Graph::upAdjust(int st, int ed){
	NeiNode *i = nodeList[st].firstNei;
	int t = i->tuss;
	visit[make_pair(min(st,ed),max(st,ed))] = 1;;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						//cout<<i->tuss<<" "<<j->tuss<<" "<<t<<endl;
						if(i->tuss < t){
							setTuss(st,i->id,1+getTuss(st,i->id));
							//changeEdge[make_pair(min(st,i->id),max(st,i->id))] += 1;
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							cout<<"ADD:"<<min(st,i->id)<<" "<<max(st,i->id)<<"  "<<changeEdge[make_pair(min(st,i->id),max(st,i->id))]<<endl;
							upAdjust_2(min(st,i->id),max(st,i->id));
							changeNodes.insert(t);
							cntTotalChangedEdge++;
							changeNodes.insert(i->id);
						}
						if(j->tuss < t){
							setTuss(ed,j->id,1+getTuss(ed,j->id));
							//changeEdge[make_pair(min(ed,i->id),max(ed,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							cout<<"ADD:"<<min(ed,i->id)<<" "<<max(ed,i->id)<<"  "<<changeEdge[make_pair(min(ed,i->id),max(ed,i->id))]<<endl;
							upAdjust_2(min(ed,i->id),max(ed,i->id));
							changeNodes.insert(t);
							cntTotalChangedEdge++;
							changeNodes.insert(j->id);
						}
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	cout<<"add return"<<endl;
	
}

void Graph::downAdjust_2(int st,int ed){
	NeiNode *i = nodeList[st].firstNei;
	int t = i->tuss;
	visit[make_pair(min(st,ed),max(st,ed))] = 1;;
	//cout<<"fff"<< cntEdge.size()<<endl;
	cout<<"ADD@2:"<<min(st,ed)<<" "<<max(st,ed)<<"  "<<changeEdge[make_pair(min(st,ed),max(st,ed))]<<endl;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						if(i->tuss == t&&j->tuss>t && !inCntEdge(st,i->id) ){
							//changeEdge[make_pair(min(st,i->id),max(st,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							upAdjust_2(min(st,i->id),max(st,i->id));
							changeNodes.insert(t);
							changeNodes.insert(i->id);
						}
						if(j->tuss == t&&i->tuss>t && !inCntEdge(ed,j->id) ){
							//changeEdge[make_pair(min(ed,i->id),max(ed,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							upAdjust_2(min(ed,i->id),max(ed,i->id));
							changeNodes.insert(t);
							changeNodes.insert(j->id);
						}
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
}

void Graph::downAdjust(int st, int ed){
	NeiNode *i = nodeList[st].firstNei;
	int t = i->tuss;
	visit[make_pair(min(st,ed),max(st,ed))] = 1;
	cout<<"fk"<<endl;
	cout<<st<<" "<<ed<<endl;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					//cout<<i->id<<" "<<j->id<<endl;
					if(j->id == i->id) {
						cout<<i->id<<" "<<j->id<<endl;
						//cout<<i->tuss<<" "<<j->tuss<<" "<<t<<endl;
						if(i->tuss <= t){
							//changeEdge[make_pair(min(st,i->id),max(st,i->id))] += 1;
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							cout<<"ADD:"<<min(st,i->id)<<" "<<max(st,i->id)<<"  "<<changeEdge[make_pair(min(st,i->id),max(st,i->id))]<<endl;
							downAdjust_2(min(st,i->id),max(st,i->id));
							changeNodes.insert(t);
							changeNodes.insert(i->id);
						}
						if(j->tuss <= t){
							//changeEdge[make_pair(min(ed,i->id),max(ed,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							cout<<"ADD:"<<min(ed,i->id)<<" "<<max(ed,i->id)<<"  "<<changeEdge[make_pair(min(ed,i->id),max(ed,i->id))]<<endl;
							downAdjust_2(min(ed,i->id),max(ed,i->id));
							changeNodes.insert(t);
							changeNodes.insert(j->id);
						}
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	
}

void Graph::dynamicInsert(int stId,int edId){
	cout<<"Insert:"<<stId<<" "<<edId<<"<<<<<<<<<<<<<<<<<<<<<<"<<endl;
	int s = stId;
	int e = edId;
	addEdge(stId,edId);
	stId = Find(stId);
	edId = Find(edId);
	int sp = computeSup(stId,edId);
	nodeList[stId].firstNei->sup = sp;
	nodeList[edId].firstNei->sup = sp;
	setTuss( stId,edId,sp+2);
	boundData bd = bound(stId,edId);
	cout<<"Dot:"<<s<<" "<<e<<" ->bound:"<<bd.up_bound<<" sup+2:" <<sp+2<<endl;

	setTuss( stId,edId,min(sp+2,bd.up_bound));

	upAdjust( stId, edId);
	cout<<"Adjust finished"<<endl;
	
	set<pair<int,int> >::iterator it;
	for(it =cntEdge.begin();it!=cntEdge.end();it++){
		visit[*it] = 0;
	}
	cntEdge.clear();
	cout<<"Insert finish:"<<s<<" "<<e<<"<<<<<<<<<<<<<<<<<<<<<<"<<endl;
	/*
	set<pair<int,int> >::iterator it;
	for(it =cntEdge.begin();it!=cntEdge.end();it++){
		//cout<<(*it).first<<" "<<(*it).second<<"  "<<changeEdge[*it]<<" "<<getTuss((*it).first,(*it).second)<<" "<<getSup((*it).first,(*it).second)+2<<endl;
		int a = getSup((*it).first,(*it).second)+2;
		int b = getTuss((*it).first,(*it).second)+changeEdge[*it];
		int newTuss = min(a,b);
		cntTotalChangedEdge += changeEdge[*it];
		//cout<<a<<" "<<b<<endl;
		//cout<<(*it).first<<" "<<(*it).second<<" "<< newTuss<<" "<<changeEdge[*it]<<" "<<getTuss((*it).first,(*it).second)<<" "<<getSup((*it).first,(*it).second)+2<<endl;
		setTuss((*it).first, (*it).second,newTuss);
		changeEdge[*it] = 0;
		visit[*it] = 0;
	}
	cntEdge.clear();
	*/
}

void Graph::dynamicDelete(int stId,int edId){
	addEdge(stId,edId);
	stId = Find(stId);
	edId = Find(edId);
	//boundData bd = bound(stId,edId);
	remEdge(stId,edId);
	downAdjust( stId, edId);
	set<pair<int,int> >::iterator it;
	for(it =cntEdge.begin();it!=cntEdge.end();it++){
		int a = computeSup((*it).first,(*it).second)+2;
		int b = getTuss((*it).first,(*it).second);
		int newTuss = min(a,b);
		cout<<(*it).first<<" "<<(*it).second<<" "<< newTuss<<" "<<a<<" "<<b<<endl;
		setTuss((*it).first, (*it).second,newTuss);
		cntTotalChangedEdge++;
		visit[*it] = 0;
	}
	cntEdge.clear();

}

boundData Graph::bound(int st,int ed){
	vector<int > v;
	boundData res;
	res.up_bound = 9999999;
	res.low_bound = 9999999;
	NeiNode *i = nodeList[st].firstNei;
	int maxTuss = -1;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						int a = getTuss(i->id,st);
						int b = getTuss(j->id,ed);
						int d = min(a,b);
						v.push_back(d);
						maxTuss = max(maxTuss,d);
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	int up = 0;
	int low = 0;
	while(maxTuss>0){
		int cnt = 0;
		int cnt2 = 0;
		for(int q = 0;q<v.size();q++){
	 //       cout<<newTuss[q]<<" ";
			if(v[q]>=maxTuss-1)
				cnt++;
			if(v[q]>=maxTuss)
				cnt2++;
		}
		if(cnt>=maxTuss-2 && up == 0) {
			res.up_bound = maxTuss;
			up = 1;
		}
		if(cnt2>=maxTuss-2 && low == 0) {
			res.low_bound = maxTuss;
			low = 1;
		}
		if(up&&low) break;
		maxTuss --;
	}
	return res;
	
}










