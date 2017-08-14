#include "graph.h"
ofstream outfile4("4.txt", ofstream::trunc);
Edge:: Edge(long src, long dest, double w)
{
    n1 = src;
    n2 = dest;
    weight = w;
    CCS = 0;
    CS = 0;
}

long Edge::getSrc()
{
    return n1;
}

long Edge::getDest()
{
    return n2;
}

float Edge::getWeight()
{
    return weight;
}

bool operator <(Edge& e1, Edge& e2)
{
    return e1.CCS < e2.CCS;
}

Node::Node(long no)
{
    sn = no;
    link = NULL;
}

Node:: Node()
{
    sn = 0;
    link = NULL;
}

long Node::getSn()
{
    return sn;
}
void Node::mapIns(long n, Edge* r)
{
    adjEdges.insert(pair<long, Edge*>(n, r));
}

void Node::adjNodesIns(long n)
{
    adjNodes.insert(n);
}

Graph:: Graph(char* f)
{
    filename = f;
    TS = 0;
}

long Graph::verNum()
{
    if (nodeIndex[0] == NULL)
    {
        return nodeIndex.size() - 1;
    }
    else
    {
        return nodeIndex.size();
    }
}

long Graph::edgeNum()
{
    return edgelist.size();
}

Edge* Graph::returnEdge(long n1, long n2)
{
    map<long, Edge*>::iterator it;
    for (it = nodeIndex[n1]->adjEdges.begin();
            it != nodeIndex[n1]->adjEdges.end();
            ++it)
    {
        if (it->first == n2)
        {
            return it->second;
        }
    }
    return NULL;
}

void Graph::CosSim()
{
    Node *p1,*p2;
    long int n1,n2;
    float w1, w2;
    float numerator = 0;
    float denominator;
    list<Edge*>::iterator it;
    set<long>::iterator sit;
    map<long, Edge*>::iterator mit;
    set<long> interSet;
    float t1;
    float t2;
    for (it = edgelist.begin(); it != edgelist.end(); ++it)
    {
        t1 = 1;
        t2 = 1;
        numerator = 2 * ((*it)->getWeight());
        n1 = (*it)->getSrc();
        n2 = (*it)->getDest();
        p1 = nodeIndex[n1];
        p2 = nodeIndex[n2];

        for (mit = p1->adjEdges.begin();
                mit != p1->adjEdges.end();
                ++mit)
        {
            t1 += pow((mit->second)->getWeight(),2);
        }

        for (mit = p2->adjEdges.begin();
                mit != p2->adjEdges.end();
                ++mit)
        {
            t2 += pow((mit->second)->getWeight(),2);
        }
        denominator = sqrt(t1 * t2);

        set_intersection(p1->adjNodes.begin(),p1->adjNodes.end(),
                         p2->adjNodes.begin(),p2->adjNodes.end(),
                         inserter(interSet, interSet.begin()));

        for (sit = interSet.begin(); sit != interSet.end(); ++sit)
        {
            w1 = (p1->adjEdges[*sit])->getWeight();
            w2 = (p2->adjEdges[*sit])->getWeight();
            numerator += w1 * w2;
        }
        interSet.clear();

        (*it)->CS = numerator / denominator;
    }
}

void Graph::JaccardSim()
{
    int i = 0;
    unsigned long n1,n2;
    double w;
    float w1, w2;
    double numerator = 0;
    double denominator;
    list<Edge*>::iterator it;
    set<long>::iterator it1, it2 ;
    map<long, Edge*>::iterator mit;
    set<long> interSet, differSet1, differSet2;
    float t1, t2;
    for (it = edgelist.begin(); it != edgelist.end(); ++it)
    {
        numerator = 2 * ((*it)->getWeight());
        t1 = (*it)->getWeight();
        t2 = (*it)->getWeight();
        n1 = (*it)->getSrc();
        n2 = (*it)->getDest();
        w = (*it)->getWeight();

        set_difference(nodeIndex[n1]->adjNodes.begin(),
                       nodeIndex[n1]->adjNodes.end(),
                       nodeIndex[n2]->adjNodes.begin(),
                       nodeIndex[n2]->adjNodes.end(),
                       inserter(differSet1, differSet1.begin()));

        set_difference(nodeIndex[n2]->adjNodes.begin(),
                       nodeIndex[n2]->adjNodes.end(),
                       nodeIndex[n1]->adjNodes.begin(),
                       nodeIndex[n1]->adjNodes.end(),
                       inserter(differSet2, differSet2.begin()));

        set_intersection(nodeIndex[n1]->adjNodes.begin(),
                         nodeIndex[n1]->adjNodes.end(),
                         nodeIndex[n2]->adjNodes.begin(),
                         nodeIndex[n2]->adjNodes.end(),
                         inserter(interSet, interSet.begin()));

        for (it1 = interSet.begin(); it1 != interSet.end(); ++it1)
        {
            w1 = (nodeIndex[n1]->adjEdges[*it1])->getWeight();
            w2 = (nodeIndex[n2]->adjEdges[*it1])->getWeight();
            numerator += w1 * w2;
        }
        denominator = numerator - 2 * (*it)->getWeight();

        for (it1 = differSet1.begin(); it1 != differSet1.end(); ++it1)
        {
            denominator += nodeIndex[(*it1)]->adjEdges[n1]->getWeight();
        }

        for (it1 = differSet2.begin(); it1 != differSet2.end(); ++it1)
        {
            denominator += nodeIndex[(*it1)]->adjEdges[n2]->getWeight();
        }
        interSet.clear();
        differSet1.clear();
        differSet2.clear();

        (*it)->CS = numerator / denominator;
    }
}
void Graph::MinSim()
{
    Node *p1,*p2;
    long int n1,n2;
    float w1, w2;
    float numerator = 0;
    float denominator;
    list<Edge*>::iterator it;
    set<long>::iterator sit;
    map<long, Edge*>::iterator mit;
    set<long> interSet;
    float t1;
    float t2;
    for (it = edgelist.begin(); it != edgelist.end(); ++it)
    {
        t1 = 1;
        t2 = 1;
        numerator = 2 * ((*it)->getWeight());
        n1 = (*it)->getSrc();
        n2 = (*it)->getDest();
        p1 = nodeIndex[n1];
        p2 = nodeIndex[n2];

        for (mit = p1->adjEdges.begin();
                mit != p1->adjEdges.end();
                ++mit)
        {
            t1 += (mit->second)->getWeight();
        }

        for (mit = p2->adjEdges.begin();
                mit != p2->adjEdges.end();
                ++mit)
        {
            t2 += (mit->second)->getWeight();
        }


        denominator = (t1>t2)?t1:t2;

        set_intersection(p1->adjNodes.begin(),p1->adjNodes.end(),
                         p2->adjNodes.begin(),p2->adjNodes.end(),
                         inserter(interSet, interSet.begin()));

        for (sit = interSet.begin(); sit != interSet.end(); ++sit)
        {
            w1 = (p1->adjEdges[*sit])->getWeight();
            w2 = (p2->adjEdges[*sit])->getWeight();
            numerator += w1 * w2;
        }
        interSet.clear();

        (*it)->CS = numerator / denominator;
    }

}
void Graph::DSSim()
{
    long max = nodeIndex.size();
    long i;
    vector<Node*>::iterator vit;
    map<long, Edge*>::iterator mit;
    if(type == 0)
    {
        i = 0;
    }
    else
    {
        i = 1;
    }
    for (; i < max; i++)
    {
        if(nodeIndex[i] != NULL)
        {
            for (mit = nodeIndex[i]->adjEdges.begin();
                    mit != nodeIndex[i]->adjEdges.end();
                    ++mit)
            {
                nodeIndex[i]->link->DS += (mit->second)->CCS;
            }
        }
    }
}

void Graph::coreSim(int mu, FibHeap &hp)
{
    int u = mu - 1;
    int max;
    int temp;
    long i;
    float CS[nodeIndex.size()];
    Edge* p;
    float a, b, c, min;
    map<long, Edge*>::iterator mit;
    list<Edge*>::iterator lit;
    priority_queue<Edge*, vector<Edge*>, cmp1> pq;


    for (i = type; i < nodeIndex.size(); ++i)
    {
        if(nodeIndex[i] != NULL)
        {
            while (!pq.empty())
            {
                pq.pop();
            }

            for (mit = nodeIndex[i]->adjEdges.begin();
                    mit != nodeIndex[i]->adjEdges.end();++mit)
            {
                p = mit->second;
                pq.push(p);
            }

            if (u == 0)
            {
                CS[i] = 1;
            }

            if (u > nodeIndex[i]->adjEdges.size())
            {
                CS[i] = 0;
            }
            else
            {
                temp = u;
                while (temp > 1)
                {
                    pq.pop();
                    temp--;
                }
                CS[i] = pq.top()->CS;
            }
        }
    }

    float RS1,RS2;
    bool* inHeap = new bool[nodeIndex.size()+1];
    FibHeapNode  *newNode;
    vector<FibHeapNode*> fibNodeVec;
    fibNodeVec.resize(nodeIndex.size() + 1);
    long sn;

    for(int i =0; i<nodeIndex.size()+1; i++)
    {
        inHeap[i] = 0;
    }

    for (lit = edgelist.begin();lit != edgelist.end();lit++)
    {
        a = CS[(*lit)->getSrc()];
        b = CS[(*lit)->getDest()];
        c = (*lit)->CS;

        if(a < c)
            RS1 = a;
        else
            RS1 = c;

        if(b < c)
            RS2 = b;
        else
            RS2 = c;

        if(RS1 < RS2)
        {
            sn = (*lit)->getSrc();
            newNode = new FibHeapNode();
            newNode->SetKeyValue(1 - RS2);
            newNode->SetIndexValue(sn);

            if(inHeap[sn] == 0)
            {
                hp.Insert(newNode);
                inHeap[sn] = 1;
                fibNodeVec[sn] = newNode;

                if(sn >= dependNode.size())
                    dependNode.resize(sn+1);

                dependNode[sn] = (*lit)->getDest();
            }
            else
            {
                if(inHeap[sn] == 1 && 1 - RS2 < fibNodeVec[sn]->GetKeyValue())
                {
                    hp.DecreaseKey(fibNodeVec[sn], 1 - RS2);
                    dependNode[sn] =   (*lit)->getDest();
                }
            }
        }
        else
        {
            if(RS1>RS2)
            {
                sn = (*lit)->getDest();
                newNode = new FibHeapNode();
                newNode->SetKeyValue(1 - RS1);
                newNode->SetIndexValue(sn);

                if(inHeap[sn] == 0)
                {
                    hp.Insert(newNode);
                    fibNodeVec[sn] = newNode;
                    inHeap[sn] = 1;
                    if(sn >= dependNode.size())
                    {
                        dependNode.resize(sn+1);
                    }
                    dependNode[sn] = (*lit)->getSrc();
                }
                else
                {
                    if(inHeap[sn] == 1 && 1 - RS1 < fibNodeVec[sn]->GetKeyValue())
                    {
                        hp.DecreaseKey(fibNodeVec[sn], 1 - RS1);
                        dependNode[sn] =  (*lit)->getSrc();
                    }
                }
            }
        }

        if (a > b)
        {
            if (b > c)
            {
                min = c;
            }
            else
            {
                min = b;
            }
        }
        else
        {
            if (a > c)
            {
                min = c;
            }
            else
            {
                min = a;
            }
        }
        (*lit)->CCS = min;
        TS += min;
    }
    TS *= 2;
}

bool Graph::readGraph()
{
    long c0, c1;
    float c2 = 1;
    ifstream o2(filename);
    if (!o2.is_open())
    {
        cout << "error:can not open file:" << filename << endl;
        system("pause");
        exit(0);
    }

    int j = 0;
    char buf[100];
    ifstream o1(filename);

    do
    {
        o1.get(buf[j]);
        if (buf[j] == '\t')
        {
            j++;
        }
    }
    while (buf[j] != '\n');
    o1.close();
    do
    {
        if (j == 1)
        {
            o2 >> c0;
            o2 >> c1;
        }
        else
        {
            o2 >> c0;
            o2 >> c1;
            o2 >> c2;
        }
        if (c0 > c1)
        {
            long temp = c0;
            c0 = c1;
            c1 = temp;
        }

        set<long>::iterator sit;
        bool flag2, flag3;
        bool flag = 1;
        map<long, Edge*>::iterator mit;

        if (c0 < nodeIndex.size() && nodeIndex[c0] != NULL)
        {
            for (mit = nodeIndex[c0]->adjEdges.begin(); mit != nodeIndex[c0]->adjEdges.end(); ++mit)
            {
                if (mit->first == c1)
                {
                    flag = 0;
                    break;
                }
            }
        }

        if (flag)
        {
            Edge* s = new Edge(c0, c1, c2);
            edgelist.push_back(s);

            flag2 = compare(c0, nodeIndex);

            if (flag2)
            {
                Cluster* q = new Cluster(c0);
                nodeIndex[c0]->link = q;
                alonelist.push_back(q);
            }
            nodeIndex[c0]->mapIns(c1, s);
            flag3 = compare(c1, nodeIndex);

            if (flag3)
            {
                Cluster* q = new Cluster(c1);
                nodeIndex[c1]->link = q;
                alonelist.push_back(q);
            }

            nodeIndex[c1]->mapIns(c0, s);
            nodeIndex[c0]->adjNodesIns(c1);
            nodeIndex[c1]->adjNodesIns(c0);
        }
    }
    while (!o2.eof());
    o2.close();
    if(nodeIndex[0] != NULL)
        type = 0;
    else
        type = 1;
    clock_t time2 = clock();
    return 1;
}

bool compare(long  n, vector<Node*>& p)
{
    if(n+1>p.size())
    {
        p.resize(n+1);
    }
    if(p[n] != NULL)
    {
        return 0;
    }
    else
    {
        p[n] = new Node(n);
        return 1;
    }
}
Cluster::Cluster(long sn)
{
    nodeSet.insert(sn);
    IS = DS = Qs = 0;
}

void
Graph::mergeQs(vector<Edge*>& etemp, float& cQs, int mu,long &k,FibHeap &hp,float &time1)
{
    float epsilon = etemp[0]->CCS;
    Cluster *p1,*p2;
    long size1,size2;
    float deltaQ = 0;
    float DS1, DS2;
    float Qs1,Qs2;
    int i = 0;
    int count = 0;
    long sn1, sn2;
    float iQs = 0;

    set<long>::iterator it, sit;
    list<Cluster*>::iterator  cit;

    set<long> interSet;
    double US, Qs;

    vector <Edge*> stemp;

    for (i = 0; i < etemp.size(); ++i)
    {
        US = Qs = 0;
        sn1 = etemp[i]->getSrc();
        sn2 = etemp[i]->getDest();

        if(nodeIndex[sn1]->link->nodeSet.size() >nodeIndex[sn2]->link->nodeSet.size())
        {
            long  t = sn1;
            sn1 = sn2;
            sn2 = t;
        }
        p1 = nodeIndex[sn1]->link;
        p2 = nodeIndex[sn2]->link;
        size1 = p1->nodeSet.size();
        size2 = p2->nodeSet.size();
        count = 0;

        if(p1 != p2)
        {
            clock_t t1 = clock();
            for (sit =p1->nodeSet.begin(); sit != p1->nodeSet.end(); ++sit)
            {
                set_intersection(nodeIndex[*sit]->adjNodes.begin(),nodeIndex[*sit]->adjNodes.end(),
                                 p2->nodeSet.begin(),p2->nodeSet.end(),inserter(interSet, interSet.begin()));

                for (it = interSet.begin(); it != interSet.end(); ++it)
                {
                    US += nodeIndex[*sit]->adjEdges[*it]->CCS;
                }

                interSet.clear();
            }

            clock_t t2 = clock();

            if(p2->nodeSet.size() == 1)
            {
                resultlist.push_back(p2) ;
            }

            for (it = p1->nodeSet.begin(); it != p1->nodeSet.end(); ++it)
            {
                p2->nodeSet.insert(*it);
                if (*it != sn1)
                {
                    nodeIndex[*it]->link = p2;
                }
            }

            DS1 = p1->DS;
            DS2 = p2->DS;
            p2->DS =p1->DS +p2->DS;

            if(size2 == 1)
            {
                Qs1 = -(DS1/TS)*(DS1/TS) ;
                Qs2 = -(DS2/TS)*(DS2/TS) ;
                cit = alonelist.begin();
                while(cit != alonelist.end())
                {
                    if(count != 2)
                    {
                        if ((*cit) ==p1 || (*cit) == p2)
                        {
                            cit = alonelist.erase(cit);
                            count++;
                        }
                        else
                            cit++;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            if(size2 > 1 && size1 == 1)
            {
                Qs1 = -(DS1/TS)*(DS1/TS) ;
                Qs2 = 0;
                cit = alonelist.begin();
                while(cit != alonelist.end())
                {
                    if ((*cit) == p1 )
                    {
                        alonelist.erase(cit);
                        break;
                    }
                    else
                    {
                        cit++;
                    }
                }
            }

            if(size2 > 1 && size1 > 1)
            {
                Qs1 = Qs2 = 0;
                cit = resultlist.begin();
                while(cit != resultlist.end())
                {
                    if ((*cit) == p1 )
                    {
                        resultlist.erase(cit);
                        break;
                    }
                    else
                    {
                        cit++;
                    }
                }
            }
            iQs +=Qs1+Qs2;
            delete p1;
            nodeIndex[sn1]->link = p2;
            deltaQ += 2 * US / TS - 2 * DS1 * DS2 / (TS * TS);
        }//if
    }//end of etemp cycle

    cQs = cQs + deltaQ + iQs;
    FibHeapNode  *outNode;
    vector<FibHeapNode*> vfn;
    long u,v;
    if(mu>2)
    {
        vector<FibHeapNode*>::iterator fit;
        while(hp.GetNumNodes() != 0&&1-hp.Minimum()->GetKeyValue() >= epsilon)
        {
            u = hp.Minimum()->GetIndexValue();
            v = dependNode[u];
            if(nodeIndex[u]->link->nodeSet.size() == 1 && nodeIndex[v]->link->nodeSet.size() != 1)
            {
                hp.ExtractMin();
                stemp.push_back(nodeIndex[u]->adjEdges[v]);
                k++;
            }
            else
            {
                vfn.push_back(hp.ExtractMin());
            }
        }

        for(fit = vfn.begin(); fit !=vfn.end(); ++fit )
        {
            hp.Insert(*fit);
        }
        vfn.clear();

        if(!stemp.empty())
        {
            mergeQs(stemp,cQs,-1,k,hp,time1);
        }
        stemp.clear();
    }
    if(mu != -1)
    {
        outfile4<<epsilon<<"    "<<cQs<<endl;
    }
}

float Graph::mergeQs(vector<Edge*>& etemp,float Qs1,float& cQs,
                     vector<long>& result,float& epsilon,int mu,FibHeap &hp,
                     float &time1,float &time2)
{
    clock_t t1 = clock();
    float eps = etemp[0]->CCS;

    Cluster *p1,*p2;
    long k = 0;
    float deltaQ = 0;
    float DS1, DS2;
    float iQs1,iQs2;
    float iQs = 0;
    int i = 0;
    int count = 0;
    long sn1, sn2;
    set<long>::iterator it, sit;
    list<Cluster*>::iterator  cit,lit;
    set<long> interSet;
    vector <Edge*> stemp;
    long t;
    double US, Qs;
    long size1,size2;

    for (i = 0; i < etemp.size(); ++i)
    {
        US = Qs = 0;
        count = 0;
        sn1 = etemp[i]->getSrc();
        sn2 = etemp[i]->getDest();
        if(nodeIndex[sn1]->link->nodeSet.size() > nodeIndex[sn2]->link->nodeSet.size())
        {
            t = sn1;
            sn1 = sn2;
            sn2 = t;
        }
        p1 = nodeIndex[sn1]->link;
        p2 = nodeIndex[sn2]->link;
        size1 = p1->nodeSet.size();
        size2 = p2->nodeSet.size();

        if(p2->nodeSet.size() == 1)
        {
            resultlist.push_back( p2) ;
        }

        if (p1 !=p2)
        {
            for (sit = p1->nodeSet.begin(); sit != p1->nodeSet.end(); ++sit)
            {
                set_intersection(nodeIndex[*sit]->adjNodes.begin(),
                                 nodeIndex[*sit]->adjNodes.end(),
                                 p2->nodeSet.begin(),p2->nodeSet.end(),
                                 inserter(interSet, interSet.begin()));

                for (it = interSet.begin(); it != interSet.end(); ++it)
                {
                    US += nodeIndex[*sit]->adjEdges[*it]->CCS;
                }
                interSet.clear();
            }

            for (it = p1->nodeSet.begin() ;
                    it != p1->nodeSet.end();
                    ++it)
            {
                p2->nodeSet.insert(*it);

                if (*it != sn1)
                {
                    nodeIndex[*it]->link = p2;
                }
            }

            DS1 =p1->DS;
            DS2 =p2->DS;
            p2->DS = DS1+DS2;

            if(size2 == 1)
            {
                iQs1 = -(DS1/TS)*(DS1/TS) ;
                iQs2 = -(DS2/TS)*(DS2/TS) ;
                cit = alonelist.begin();
                while(cit != alonelist.end()&&count < 2)
                {
                    if ((*cit) == p1 || (*cit) == p2)
                    {
                        cit = alonelist.erase(cit);
                        count++;
                    }
                    else
                    {
                        cit++;
                    }
                }
            }

            if(size2 > 1 && size1 == 1)
            {
                iQs1 = -(DS1/TS)*(DS1/TS) ;
                iQs2 = 0;
                cit = alonelist.begin();
                while(cit != alonelist.end())
                {
                    if ((*cit) == p1 )
                    {
                        alonelist.erase(cit);
                        break;
                    }
                    else
                    {
                        cit++;
                    }
                }
            }

            if(size2 > 1 && size1 > 1)
            {
                iQs1 = iQs2 = 0;
                cit = resultlist.begin();
                while(cit != resultlist.end())
                {
                    if ((*cit) == p1 )
                    {
                        resultlist.erase(cit);
                        break;
                    }
                    else
                    {
                        cit++;
                    }
                }
            }
            iQs += iQs1+iQs2;
            delete p1;
            nodeIndex[sn1]->link = p2;
            deltaQ += 2 * US / TS - 2 * DS1 * DS2 / (TS * TS);
        }
    }

    cQs = cQs + deltaQ + iQs;

    if(mu != -1)
    {
        outfile4<<eps<<"    "<<cQs<<endl;
    }

    clock_t t2 = clock();
    time2 += t2-t1;
    clock_t t3 = clock();

    if(mu>2)
    {
        vector<FibHeapNode*> vfn;
        long u,v;

        while(hp.GetNumNodes() > 0&&1-hp.Minimum()->GetKeyValue() >= eps)
        {
            u = hp.Minimum()->GetIndexValue();
            v = dependNode[u];

            if(nodeIndex[u]->link->nodeSet.size() == 1 && nodeIndex[v]->link->nodeSet.size() != 1)
            {
                hp.ExtractMin();
                stemp.push_back(nodeIndex[u]->adjEdges[v]);
            }
            else
            {
                vfn.push_back(hp.ExtractMin());
            }
        }

        vector<FibHeapNode*>::iterator it = vfn.begin();
        for(; it !=vfn.end(); ++it )
        {
            hp.Insert(*it);
        }
        vfn.clear();

        if(!stemp.empty())
        {
            mergeQs(stemp,cQs,-1,k,hp,time2);
        }
        stemp.clear();
    }

    if (cQs > Qs1)
    {
        epsilon = etemp[0]->CCS;

        result.clear();
        result.resize(verNum()+type);

        long i = 1;
        for (cit = resultlist.begin(); cit != resultlist.end(); ++cit)
        {
            for(it = (*cit)->nodeSet.begin(); it!=(*cit)->nodeSet.end(); it++)
            {
                result[*it] =  i;
            }
            i++;
        }

        for (cit = alonelist.begin(); cit != alonelist.end(); ++cit)
        {
            it =  (*cit)->nodeSet.begin();
            result[*it] = 0;
        }
        clock_t t4 = clock();
        time1+= t4-t3;

        return cQs;
    }
    else
    {
        return Qs1;
    }

}

Cluster::Cluster()
{
}
