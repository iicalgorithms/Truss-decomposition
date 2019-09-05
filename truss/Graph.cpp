#include "Graph.h"
#include <fstream>
#include <iomanip>
#include <string.h>
#include <stack>
#include <stdlib.h>

Graph::Graph(){}

void Graph::buildGraph(int edge_num,int node_num){
    edgeNum = edge_num;
    nodeNum = node_num;
    nodeList = new GraphNode[node_num+10]();
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
    //cout<<stId<<" "<<edId<<endl;
    int findSt = Find(stId);//map  将标号映射成下表
    if(findSt<0){
        findSt = cnt;
        nodeList[cnt].id = stId;
        findId[stId] = cnt++;
    }
    int findEd = Find(edId);
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
    cout<<"Init support start"<<endl;
    for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->sup = computeSup(k,nei->id);
                //cout<<nei->sup<<endl;
            }
            nei = nei->next;
        }
    }
    cout<<"Init support complish"<<endl;
}

void Graph::initSuperSup(){
    cout<<"Init SS start"<<endl;
    for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->sup = computeSuperSup(k,nei->id,nei->tuss);
                //cout<<nei->sup<<endl;
            }
            nei = nei->next;
        }
    }
    cout<<"Init SS complish"<<endl;
}
void Graph::initConstrainSup(){
    cout<<"Init CS start"<<endl;
    for (int k = 0; k < nodeNum; k++){
        NeiNode *nei = nodeList[k].firstNei;
        while(nei){
            if(nei->valid){
                nei->sup = computeConstrainSup(k,nei->id,nei->tuss);
                //cout<<nei->sup<<endl;
            }
            nei = nei->next;
        }
    }
    cout<<"Init CS complish"<<endl;
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
int Graph::computeSuperSup(int st,int ed,int truss){
    int res = 0;
	NeiNode *i = nodeList[st].firstNei;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id && j->tuss>= truss && i->tuss >= truss) res++;
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	return res;
}

int Graph::computeConstrainSup(int st,int ed,int truss){
    int res = 0;
	NeiNode *i = nodeList[st].firstNei;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
                        int minn = min(i->tuss,j->tuss);
                        if(minn > truss)
                            res++;
                        else if(minn = truss){
                            int flg  = 1;
                            if(i->tuss == minn){
                                if(i->SS <= truss-2) flg = 0;
                            }
                            if(j->tuss == minn){
                                if(j->SS <= truss-2) flg = 0;
                            }
                            if(flg) res ++;
                        }
                    }
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
	//cout<<"Set edge ("<<stId<<","<<edId<<") Truss in:"<<k<<endl;
}

int Graph::getEdgeMess(int stId,int edId,int model){
	NeiNode *p = nodeList[stId].firstNei;
	int res;
    while (p)
    {
        if(p->id == edId){
            switch(model)
            {
            case 1:
                res = p->tuss;
                break;
            case 2:
                res = p->sup;
                break;
            case 3:
                res = p->SS;
                break;
            case 4:
                res = p->CS;
                break;
            default:
                break;
            }
			break;
        }
        p = p->next;
    }
    return res;
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
		edgeNum--;
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
        //output();
    }
}

void Graph::distribute(){
    cout<<"Start DIS_Algrithm"<<endl;
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
        if(startCntSteps){
            totalMsgNumber += changeNodeNum;
            totalSteps += 1;
        }  
        //cout<<"第"<<++cnt_iter<<"轮： 更新点数-"<<changeNodeNum<<"   更新边数-"<<changeEdgeNum<<endl;
        cout<<"Step "<<setw(4)<<++cnt_iter<<":  Changed Point Num = "<<setw(4)<<changeNodeNum<<"  Changed Edge(Message) Num:"<<setw(4)<<changeEdgeNum<<endl;
        if(changeEdgeNum == 0){
            break;
        }
    }
    cout<<endl;
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
    FILE *fp;//定义一个文件指针
    string tmp="";
    for(int i=0;i<strlen(filename);i++)
    tmp+=filename[i];
    cout<<filename<<endl;
    fp=fopen(strcat(filename,".truss"),"w");//以只写的方式打开文件，前面的参数是文件路径，后面的参数是表示只写
    fprintf(fp,"# Nodes:%d Edges:%d\n",nodeNum,edgeNum);
    fprintf(fp,"Node 1\tNode 2\tTruss\n");
    int totalNnum = 0;
    map<int ,int > InPercent;
    for (int i = 0; i < nodeNum; i++)
    {
        NeiNode  *nei = nodeList[i].firstNei;
        while (nei)
        {
            if(nodeList[i].id<nodeList[nei->id].id){
                fprintf(fp,"%d\t%d\t%d\n",nodeList[i].id,nodeList[nei->id].id,nei->tuss);
                totalNnum++;
                InPercent[nei->tuss]++;
            }
            nei = nei->next;
        }
    }
    filename = (char *)tmp.c_str();
    fp=fopen(strcat(filename,".percent"),"w");//以只写的方式打开文件，前面的参数是文件路径，后面的参数是表示只写
    fprintf(fp,"Total Affected nodes number:%d\n",changeNodes.size());
    fprintf(fp,"The depth of broadcast:%d\n",maxDepth);
    fprintf(fp,"Total Message Number:%d\n",totalMsgNumber);
    fprintf(fp,"Total steps:%d\n\n",totalSteps);
    fprintf(fp,"Truss\tPercent\n");
    map<int ,int >::iterator it;
    for (it = InPercent.begin(); it != InPercent.end(); it++)
    {
        fprintf(fp,"%d\t%f\n",it->first,(double)(((double)it->second)/totalNnum));
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
    //cout<<"st:"<<st<<" ed:"<<ed<<endl;
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

void Graph::upAdjust_2(int st,int ed,int deep){
	NeiNode *i = nodeList[st].firstNei;
	int t = i->tuss;
	visit[make_pair(min(st,ed),max(st,ed))] = 1;
    maxDepth = max(maxDepth,deep);
	//cout<<"fff"<< cntEdge.size()<<endl;
	//cout<<"ADD@2:"<<min(st,ed)<<" "<<max(st,ed)<<"  "<<endl;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						if(i->tuss == t&&j->tuss>=t && !inCntEdge(st,i->id) ){
							//setTuss(st,i->id,1+getTuss(st,i->id));
							//changeEdge[make_pair(min(st,i->id),max(st,i->id))] += 1 ;
                            //cout<<"ADD@2:"<<min(st,i->id)<<" "<<max(st,i->id)<<"  "<<endl;
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							upAdjust_2(min(st,i->id),max(st,i->id),deep+1);
							changeNodes.insert(st);
							changeNodes.insert(i->id);
						}
						if(j->tuss == t&&i->tuss>=t && !inCntEdge(ed,j->id) ){
							//setTuss(ed,j->id,1+getTuss(ed,j->id));
							//changeEdge[make_pair(min(ed,i->id),max(ed,i->id))] += 1 ;
                            //cout<<"ADD@2:"<<min(ed,i->id)<<" "<<max(ed,i->id)<<"  "<<endl;
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							upAdjust_2(min(ed,i->id),max(ed,i->id),deep+1);
							changeNodes.insert(ed);
							changeNodes.insert(j->id);
						}
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	//cout<<"add2 return"<<endl;
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
					if(j->id == i->id ) {
						//cout<<i->tuss<<" "<<j->tuss<<" "<<t<<endl;
						if(i->tuss < t&& !inCntEdge(st,i->id)){
							//setTuss(st,i->id,1+getTuss(st,i->id));
							//changeEdge[make_pair(min(st,i->id),max(st,i->id))] += 1;
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							//cout<<"ADD:"<<min(st,i->id)<<" "<<max(st,i->id)<<"  "<<changeEdge[make_pair(min(st,i->id),max(st,i->id))]<<endl;
							upAdjust_2(min(st,i->id),max(st,i->id),1);
							changeNodes.insert(t);
							changeNodes.insert(i->id);
						}
						if(j->tuss < t && !inCntEdge(ed,j->id)){
							//setTuss(ed,j->id,1+getTuss(ed,j->id));
							//changeEdge[make_pair(min(ed,i->id),max(ed,i->id))] += 1 ;
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							//cout<<"ADD:"<<min(ed,i->id)<<" "<<max(ed,i->id)<<"  "<<changeEdge[make_pair(min(ed,i->id),max(ed,i->id))]<<endl;
							upAdjust_2(min(ed,i->id),max(ed,i->id),1);
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
	//cout<<"add return"<<endl;
	
}

void Graph::dynamicInsert(int stId,int edId){
	//cout<<"Insert:"<<stId<<" "<<edId<<"<<<<<<<<<<<<<<<<<<<<<<"<<endl;
	int s = stId;
	int e = edId;
	addEdge(stId,edId,0);
	stId = Find(stId);
	edId = Find(edId);
	int sp = computeSup(stId,edId);
	nodeList[stId].firstNei->sup = sp;
	nodeList[edId].firstNei->sup = sp;
	setTuss( stId,edId,sp+2);
	boundData bd = bound(stId,edId);
	//cout<<"Dot:"<<s<<" "<<e<<" ->bound:"<<bd.up_bound<<" sup+2:" <<sp+2<<endl;

	setTuss( stId,edId,min(sp+2,bd.up_bound));
//cout<<nodeList[stId].id <<" "<<nodeList[edId].id<<" Up_bound:"<<min(sp+2,bd.up_bound)<<endl;
    upAdjust( stId, edId);
	//cout<<"Adjust finished"<<endl;
	/*
	set<pair<int,int> >::iterator it;
	for(it =cntEdge.begin();it!=cntEdge.end();it++){
		visit[*it] = 0;
	}
	cntEdge.clear();
	//cout<<"Insert finish:"<<s<<" "<<e<<"<<<<<<<<<<<<<<<<<<<<<<"<<endl;
	*/
	set<pair<int,int> >::iterator it;
	for(it =cntEdge.begin();it!=cntEdge.end();it++){
		//cout<<(*it).first<<" "<<(*it).second<<"  "<<changeEdge[*it]<<" "<<getTuss((*it).first,(*it).second)<<" "<<getSup((*it).first,(*it).second)+2<<endl;
		int a = computeSup((*it).first,(*it).second)+2;
		int b = getEdgeMess((*it).first,(*it).second,1)+1;//changeEdge[*it];
        //cout<<"xutao xitong d :"<<changeEdge[*it]<<endl;
		int newTuss = min(a,b);
		//cout<<a<<" "<<b<<endl;
//cout<<nodeList[(*it).first].id<<" "<<nodeList[(*it).second].id<<" newTruss:"<< newTuss<<"  getTruss and sup:"<<getTuss((*it).first,(*it).second)<<endl;
		setTuss((*it).first, (*it).second,newTuss);
		//changeEdge[*it] = 0;
		visit[*it] = 0;
	}
	cntEdge.clear();
	visit[make_pair(min(stId,edId),max(stId,edId))] = 0;
}

void Graph::downAdjust(int st, int ed){
	NeiNode *i = nodeList[st].firstNei;
	int t = i->tuss;
	visit[make_pair(min(st,ed),max(st,ed))] = 1;;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						if(i->tuss <= t){
							cntEdge.insert(make_pair(min(st,i->id),max(st,i->id)));
							//cout<<"ADD:"<<min(st,i->id)<<" "<<max(st,i->id)<<"  "<<changeEdge[make_pair(min(st,i->id),max(st,i->id))]<<endl;
							upAdjust_2(min(st,i->id),max(st,i->id),1);
							changeNodes.insert(t);
							changeNodes.insert(i->id);
						}
						if(j->tuss <= t){
							cntEdge.insert(make_pair(min(ed,i->id),max(ed,i->id)));
							//cout<<"ADD:"<<min(ed,i->id)<<" "<<max(ed,i->id)<<"  "<<changeEdge[make_pair(min(ed,i->id),max(ed,i->id))]<<endl;
							upAdjust_2(min(ed,i->id),max(ed,i->id),1);
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

void Graph::dynamicDelete(int stId,int edId){
	//cout<<"Start delete:"<<stId<<" "<<edId<<endl;
	//addEdge(stId,edId);
	stId = Find(stId);
	edId = Find(edId);
	//boundData bd = bound(stId,edId);
	remEdge(stId,edId);
	downAdjust( stId, edId);
	//cout<<"Adjust finished"<<endl;
	set<pair<int,int> >::iterator it;
	for(it =cntEdge.begin();it!=cntEdge.end();it++){
		int a = computeSup((*it).first,(*it).second)+2;
		int b = getEdgeMess((*it).first,(*it).second,1);
		int newTuss = min(a,b);
		//cout<<(*it).first<<" "<<(*it).second<<" "<< newTuss<<" "<<a<<" "<<b<<endl;
		setTuss((*it).first, (*it).second,newTuss);
		visit[*it] = 0;
	}
	cntEdge.clear();
	//cout<<"Delete finished"<<endl;
    visit[make_pair(min(stId,edId),max(stId,edId))] = 0;
}

boundData Graph::bound(int st,int ed){
	vector<int > v;
	boundData res;
	res.up_bound = 9999999;
	res.low_bound = 9999999;
	NeiNode *i = nodeList[st].firstNei;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						int a = i->tuss;// getTuss(i->id,st);
						int b = j->tuss;//getTuss(j->id,ed);
						int d = min(a,b);
						v.push_back(d);
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
	int up = 0;
	int low = 0;
    int maxTuss = v.size()+2;
	while(maxTuss>2){
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

void Graph::supInitDelete(int stId,int edId){
    stId = Find(stId);
	edId = Find(edId);
    int st = min(stId,edId);
    int ed = max(stId,edId);
    deleteEdge(st,ed);
}


void Graph::centerInsert(int stId,int edId){
    addEdge(stId,edId);
    stId = Find(stId);
	edId = Find(edId);
    int st = min(stId,edId);
    int ed = max(stId,edId);
    int LB = 0;
    vector<int > v;
    NeiNode *i = nodeList[st].firstNei;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						int a = i->tuss;// getTuss(i->id,st);
						int b = j->tuss;//getTuss(j->id,ed);
						int d = min(a,b);
						v.push_back(d);
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
    int maxTuss = v.size()+2;
	while(maxTuss>2){
		int cnt = 0;
		for(int q = 0;q<v.size();q++){
	 //       cout<<newTuss[q]<<" ";
			if(v[q]>=maxTuss)
				cnt++;
		}
		if(cnt>=maxTuss-2) {
			LB = maxTuss;
			break;
		}
		maxTuss --;
	}
    int UB=0;
    int myTruss = LB;

    setTuss(st,ed,LB);
    initSuperSup();
    initConstrainSup();
    int mySS = getEdgeMess(st,ed,3);
    int myCS = getEdgeMess(st,ed,4);
    if(myCS > myTruss-2) UB = LB+1;
    else UB = LB;

    set<pair<int,int> > PES;
    i = nodeList[st].firstNei;
	while(i){
		if(i->valid && i->id != ed){
			NeiNode *j = nodeList[ed].firstNei;
			while(j){
				if(j->valid && j->id!= st){
					if(j->id == i->id) {
						if(i->tuss<UB) PES.insert(make_pair(min(i->id,st),max(i->id,st)));
                        if(i->tuss<UB) PES.insert(make_pair(min(j->id,ed),max(j->id,ed)));
					}
				}
				j = j->next;
			}
		}
		i = i->next;
	}
    set<pair<int,int> >::iterator iter;
    for(iter = PES.begin();iter!=PES.end();iter++){
        Vset.clear();
        Sset.clear();
        Xset.clear();
        while(!Stack.empty()) Stack.pop();

        pair<int,int> e_root = *iter;
        int rootTruss = getEdgeMess(e_root.first,e_root.second,1);
        Sset[e_root] = getEdgeMess(e_root.first,e_root.second,4);
        Stack.push(e_root);
        Vset[e_root] = 1;
        while(!(Stack.empty())){
            pair<int,int> now_e = Stack.top();
            Stack.pop();
            if(Sset[now_e]>rootTruss-2){
            int a = now_e.first;
                int b = now_e.second;
                i = nodeList[a].firstNei;
                while(i){
                    if(i->valid && i->id != b){
                        NeiNode *j = nodeList[b].firstNei;
                        while(j){
                            if(j->valid && j->id!= a){
                                if(j->id == i->id) {
                                    if(min(i->tuss,j->tuss)>= rootTruss){
                                        pair<int,int> new_e = make_pair(min(i->id,st),max(i->id,st));
                                        if(i->tuss == rootTruss && i->SS > rootTruss-2 && Vset[new_e]==0 ){
                                            Stack.push(new_e);
                                            Vset[new_e] = 1;
                                            Sset[new_e] = Sset[new_e] + getEdgeMess(new_e.first,new_e.second,4);
                                        }
                                        new_e = make_pair(min(i->id,ed),max(i->id,ed));
                                        if(j->tuss == rootTruss && j->SS > rootTruss-2 && Vset[new_e]==0 ){
                                            Stack.push(new_e);
                                            Vset[new_e] = 1;
                                            Sset[new_e] = Sset[new_e] + getEdgeMess(new_e.first,new_e.second,4);
                                        }
                                    }
                                }
                            }
                            j = j->next;
                        }
                    }
                    i = i->next;
                }
            }else {
                if(Xset[now_e] == 0) {
                    Xset[now_e] = 1;
                    Eliminate(now_e,rootTruss);
                }
            }
        }
    }
}

void Graph::Eliminate(pair<int,int>,int){
    
}


