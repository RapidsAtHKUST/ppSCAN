#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <string>
#include <cmath>
#include <queue>
#include "windows.h"
#include "time.h"
#include "fibheap.h"

using namespace std;

class Node;
class Graph;
class Cluster;

bool compare(long,vector<Node*> &);

class Edge
{          //每条边对应一个Edge对象
     public:
         Edge( long src,  long dest, double w);
         long getSrc() ;
         long  getDest()  ;
         float getWeight() ;
         void setSim(float f);
         float getSim();
         float CCS;                  //核联通相似度
         float CS;                  //结构相似度
         float weight;             // 边权
         long n1,n2;
};

class cmp
{
    public:
        bool operator()(  Edge*  n1,  Edge* n2)
        {
            return n1->CCS < n2->CCS;
        }
 };
class cmp1
{
         public:
        bool operator()(  Edge*  n1,  Edge* n2)
        {
            return n1->CS < n2->CS;
        }
};
class Node
{
    public:
        Node(long no);
        Node();
        long getSn();
        void mapIns(long n,Edge *r);       //建立节点的邻接节点与边的映射
        map <long, float> distance;        //floyd算法计算距离所用
        void adjNodesIns(long n);          //给本节点邻接边容器中插入元素
        set <long> adjNodes;               //方便计算两个节点相似度以及两个超点合并时的并交运算
        map <long, Edge *> adjEdges;       //邻接节点号，邻接边
        Cluster* link;                     //指向本节点属于的聚类
    private:
        long sn;   //原始节点号
};

class Graph
{
    friend class network_info;
    public:
        bool type;
        float TS;
        void DSSim();
        list <Cluster*> alonelist;                  //存储聚类时的孤立节点 ，随着聚类过程逐步减少
        list <Cluster*> resultlist;               //存储聚类，随着聚类过程不断扩充
        float mergeQs(vector<Edge*> &etemp,float Qs,float &cQs,
                     vector<long> &result,float &epsilon,int mu,FibHeap &hp,float &time1,float &time2);    //聚类合并第二阶段
        void mergeQs(vector<Edge*> &etemp,float &cQs,int mu,long &k,FibHeap &hp,float &time1);                     //聚类合并第一阶段
        vector <Node *> nodeIndex;               //图节点的随机访问存储结构，用vector映射到每个节点对象
        vector <long> dependNode;
        Graph (char* f);
        long verNum();                           //返回图中节点数目
        long edgeNum();                          //返回图中边数目
        Graph();
        bool readGraph();                        //从文件中读入图
        void print();                            //输出读入的图信息
        void coreSim(int,FibHeap&);                       //计算核相似度、核联通相似度
        Edge* returnEdge(long n1, long n2);      //返回以n1、n2为始、终节点的边
        void JaccardSim();                       //相似度计算，并对Q进行赋初值
        void CosSim();                           //相似度计算，并对Q进行赋初值
        void MinSim();
        list <Edge *> edgelist;                  //边的顺序访问存储结构，用双向链表将边指针串起来，方便对边的相似度计算和删除
    private:
        char* filename;                            //图文件名

};

class Cluster
{
     public:
         Cluster(long sn);
         Cluster();
         set<long> nodeSet;                       //存储本聚类中的节点编号
         float IS,DS;
         float Qs;                              //存储本剧类的Qs值，所有聚类Qs相加为总的Qs值
};
