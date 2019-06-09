#include <iostream>
#include <map>
#include <vector>
#include <set>
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


struct NeiNode{
    int id;
    NeiNode * next;
   // NeiNeiNode * firstNeiNei;
    int valid = 1;
    int sup;
    double PrSup = 0;
    int tuss = 0;
    double Pr = 0.1;
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
    private:
        int edgeNum;
        int nodeNum;
        double gamma = 0.1;
        GraphNode *nodeList;
        map<int,int> findId;
        //map<pair<int,int>,edge> findInfo; 
        set<edge,cmp> edgeSet;
        int find(int id);
        int cnt = 0;
        vector<pair<int,int> > * bin;
        void setTuss(int stId,int edId,int k);
        void remEdge(int stId,int edId);
        void PrRemEdge(int stId,int edId);
        double computePrSup(int st,int ed,double p);
        void deleteEdge(int st,int ed);
        void outputSet();
};

/*
struct NeiNeiNode
{
    int id;
    int sup;
    NeiNeiNode *next;
};
*/

