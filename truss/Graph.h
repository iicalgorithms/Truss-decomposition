#include <iostream>
#include <map>
#include <vector>
#include <set>
#include <cmath>
using namespace std;

struct triNode
{
    int st,mid,ed;
    double PrSt,PrEd;
    triNode(int s,int m,int e,double ps,double pe){
        st = s;mid = m;ed = e;
        PrSt = ps; PrEd = pe;
    }
};

struct boundData
{
	int up_bound;
	int low_bound;
};

struct NeiNode{
    int id;
    NeiNode * next;
   // NeiNeiNode * firstNeiNei;
    int valid = 1;
    int sup;
    double PrSup = 0;
    int tuss = 0;
    double Pr = 0.1;
	//int eeMap = 0;
    vector<triNode> tris;
    NeiNode(){Pr = 0.5;}
};

struct GraphNode
{
    NeiNode * firstNei;
    int id; 
    int degree = 0; 
};

struct edge
{
    int st,ed;
    double PrSup;
    edge(int s,int e,double p ){st = s;ed= e;PrSup = p;}
    edge(){}
};
struct cmp{  
    bool operator ()(const edge a , const edge b) {
        if(a.PrSup!=b.PrSup)
            return a.PrSup < b.PrSup;
        else if(a.st!=b.st)
            return a.st < b.st;
        return a.ed<b.ed;
    }
};  

class Graph{
    public:
        Graph();
        void buildGraph(int edge_num,int node_num);
        int getEdgeNum(){return edgeNum;}
        int getNodeNum(){return nodeNum;}
        void initSup();
        void addEdge(int stId,int edId);
        void addEdge(int stId,int edId,double pr);
        void output();
        void egdeBinSort();
        void greed();
        void distribute();
        void writeFile(char *filename);
        void initPrSup();
        void PrGreed();
        void PrDistribute();
		void dynamicInsert(int stId,int edId);
		void dynamicDelete(int stId,int edId);
        void supInitDelete(int stId,int edId);
		void cover();//用sup的值来覆盖truss
		int changedEdgeNum = 0;
		int changedNodeNum = 0;
		void outputDynamicInfo(int computeType){
			cout<<"Compute mode:"<<computeType<<endl;
            cout<<"Total Affected nodes number:"<<changeNodes.size()<<endl;
            cout<<"The depth of broadcast:"<< maxDepth <<endl;
		}
		
    private:
        int edgeNum;
        int nodeNum;
        double gamma = 0.1;
        GraphNode *nodeList;
        map<int,int> findId;
        //map<pair<int,int>,edge> findInfo; 
        set<edge,cmp> edgeSet;
        int Find(int id);
        int cnt = 0;
        vector<pair<int,int> > * bin;
		map<pair<int,int>,int> changeEdge;//动态插入时边改变的数
		set<pair<int,int> > cntEdge;//需要改变的边
		set<int > changeNodes;//涉及到的点
		map<pair<int,int>,int> visit;//动态插入时边改变的数
        int maxDepth = -1;
		
        void setTuss(int stId,int edId,int k);
        void remEdge(int stId,int edId);
        void PrRemEdge(int stId,int edId);
        double computePrSup(int st,int ed,double p);
        void deleteEdge(int st,int ed);
        void outputSet();
		int computeSup(int st,int ed);
		boundData bound(int st,int ed);
		void upAdjust(int st,int ed);//动态添边  记录递归深度
		void upAdjust_2(int st,int ed,int deep);//动态添边扩散
		void downAdjust(int st,int ed);//动态删边
		int getTuss(int stId,int edId);
		int getSup(int stId,int edId);
		bool inCntEdge(int st,int ed);
};

/*
struct NeiNeiNode
{
    int id;
    int sup;
    NeiNeiNode *next;
};
*/

