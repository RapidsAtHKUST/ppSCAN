#include "fibheap.h"
#include "network_info.h"
#include <sstream>
ofstream outfile1("1.txt", ofstream::trunc);
ofstream outfile2("2.txt", ofstream::trunc);

void primF(Graph&, int, priority_queue<Edge*, vector<Edge*>, cmp>&);
void clustering(int,Graph&,priority_queue<Edge*, vector<Edge*>, cmp>&,
                vector<long>&,network_info&,float,
                float,FibHeap&,const float,int);
void savecurrentState(Graph &G,const float eps);

main(int argc, char* argv[])
{
    ofstream os1;
    os1.open("5.txt",ofstream::trunc);
    os1.close();
    clock_t begin = clock();    //程序开始 计时
    string method = "";         //所采用的相似度计算方法  cosine 或jaccard
    int mu = 2;
    char name1[30] = {'\0'};
    char* name = {"net10000_0.2.dat"};       //输入文件名
    char temp[20] = {'\0'};
    float b = 0;                //参数b 范围0~1
    float e = 1;                //参数b 范围0~1
    float p = 1;                //参数p 范围0~1
    int i = 1;
    int j = 0;
    int k = 0;
    int s = 1;                  //相似度计算法 1 cosine  2 jaccard
    int v = 1;                  //聚类结果评价方法 1 Qs 2 SWC

    network_info* info = new network_info();

    ifstream infile("config.txt");
    if (!infile.is_open())
    {
        cout << "error:can not open config file\n";
        system("pause");
        exit(0);
    }

    char order[100] = {'\0'};      //接收用户输入的命令
    while (!infile.eof())
    {
        infile.get(order[j]);
        j++;
    }

    i = 0;
    while (i < j)
    {
        if (order[i] == '-' && order[i + 1] == 'f')
        {
            i = i + 2;
            while (order[i] == ' ')
            {
                i++;
            }

            while (order[i] != '\n' && order[i] != '\0')
            {
                name1[k] = order[i];
                k++;
                i++;
            }
            k = 0;
            name = name1;

            while (order[i] == '\n')
            {
                i++;
            }
        }

        if (order[i] == '-' && order[i + 1] == 'u')
        {
            i = i + 2;
            while (order[i] == ' ')
            {
                i++;
            }

            if (order[i] != '\n' && order[i] != '\0')                     //这里限制了u只能是一位数 注意
            {
                mu = order[i] - '0';
                i++;
            }

            while (order[i] == '\n')
            {
                i++;
            }
        }
        if (order[i] == '-' && order[i + 1] == 's')
        {
            i = i + 2;
            while (order[i] == ' ')
            {
                i++;
            }

            if (order[i] != '\n' && order[i] != '\0')
            {
                s = order[i] - '0';
                i++;
            }

            while (order[i] == '\n')
            {
                i++;
            }
        }

        if (order[i] == '-' && order[i + 1] == 'b')
        {
            i = i + 2;
            while (order[i] == ' ')
            {
                i++;
            }

            while (order[i] != '\n' && order[i] != '\0')
            {
                temp[k] = order[i];
                i++;
                k++;
            }
            b = atof(temp);

            for (k = 0; k < 20; k++)
            {
                temp[k] = '\0';
            }
            k = 0;

            while (order[i] == '\n')
            {
                i++;
            }
        }

        if (order[i] == '-' && order[i + 1] == 'e')
        {
            i = i + 2;
            while (order[i] == ' ')
            {
                i++;
            }

            while (order[i] != '\n' && order[i] != '\0')
            {
                temp[k] = order[i];
                i++;
                k++;
            }
            e = atof(temp);

            for (k = 0; k < 20; k++)
            {
                temp[k] = '\0';
            }
            k = 0;

            while (order[i] == '\n')
            {
                i++;
            }
        }

        if (order[i] == '-' && order[i + 1] == 'p')
        {
            i = i + 2;
            while (order[i] == ' ')
            {
                i++;
            }

            while (order[i] != '\n' && order[i] != '\0')
            {
                temp[k] = order[i];
                i++;
                k++;
            }
            p = atof(temp);

            for (k = 0; k < 20; k++)
            {
                temp[k] = '\0';
            }
            k = 0;

            while (order[i] == '\n')
            {
                i++;
            }
        }

        if (order[i] == '-' && order[i + 1] == 'v')
        {
            i = i + 2;

            while (order[i] == ' ')
            {
                i++;
            }

            if (order[i] != '\n' && order[i] != '\0')
            {
                v = order[i] - '0';
                i++;
            }

            if(order[i] == '\0')
            {
                i++;
            }

        }
    }
    i = 1;
    while (i < argc)
    {
        if (i < argc && strcmp(argv[i], "-f") == 0)
        {
            name = argv[i + 1];
            i += 2;
        }
        else
        {
            cout << "error:invalid argument\n";
            system("pause");
            exit(0);
        }

        if (i < argc && strcmp(argv[i], "-u") == 0)
        {
            mu = atoi(argv[i + 1]);
            i += 2;
        }

        if (i < argc && strcmp(argv[i], "-s") == 0)
        {
            s = atoi(argv[i + 1]);
            i += 2;
        }

        if (i < argc && strcmp(argv[i], "-b") == 0)
        {
            b = atof(argv[i + 1]);
            i += 2;
        }

        if (i < argc && strcmp(argv[i], "-e") == 0)
        {
            e = atof(argv[i + 1]);
            i += 2;
        }

        if (i < argc && strcmp(argv[i], "-p") == 0)
        {
            p = atof(argv[i + 1]);
            i += 2;
        }

        if (i < argc && strcmp(argv[i], "-v") == 0)
        {
            v = atoi(argv[i + 1]);
            i += 2;
        }
    }

    if (s == 1)
    {
        method = "cosine";
    }
    else
    {
        method = "jaccard";
    }
    info->method = method;

    Graph* g = new Graph(name);

    clock_t begin1 = clock();
    g->readGraph();
    cout << "read end" << endl;

    clock_t endofread = clock();
    cout<<endofread - begin1<<endl;
    if (s == 1)
    {
        g->CosSim();
    }                   //计算余弦相似度
    else
    {
        if(s == 2)
        {
            g->JaccardSim();
        }
        else if(s == 3)
        {
            g->MinSim();
        }
    }
    cout << "sim end" << endl;
    clock_t simt = clock();
    cout<<simt-endofread<<endl;
    FibHeap hp;
    g->coreSim(mu,hp);                //计算核联通相似度
    cout << "core end" << endl;

    g->DSSim();                    //计算DS
    cout<<"DS end"<<endl;

    priority_queue<Edge*, vector<Edge*>, cmp> priq, priq1;
    primF(*g, mu, priq);
    cout << "prim end" << endl;
    clock_t endofprim = clock();
    cout<<endofprim-simt<<endl;
    vector<long> result,singlenode;            //result 存储最大Qs时的聚类 singlenode存储最大Qs时的孤立节点
    result.resize(g->verNum()+g->type) ;
    clustering(mu, *g, priq, result,*info, b, e,hp,p,v);
    clock_t endofclu = clock();
    cout<<"clu end"<<endl;
    cout<<endofclu-endofprim<<endl;
    info->info_out(g);

    system("pause");
}

void primF(Graph& G, int mu, priority_queue<Edge*, vector<Edge*>, cmp>& priq)                          // Find a minimum spanning tree of G using Prim's algorithm and return its in T.
{
    FibHeap hp;
    long nodenum = G.nodeIndex.size();
    long max;
    FibHeapNode* newNode, * outNode;
    vector<FibHeapNode*> fibNodeVec;
    fibNodeVec.resize(nodenum);
    Edge* e;
    long int u, v, sn;
    Edge* cheap[nodenum];               //表示堆中节点对应的边
    bool* inHeap = new bool[nodenum]; // track vertices in heap
    bool* inMST = new bool[nodenum];


    for (u = G.type; u <nodenum; u++)
    {
        inHeap[u] = 0;
        inMST[u] = 0;
    }

    map<long, Edge*>::iterator mit;
    mit = G.nodeIndex[G.type]->adjEdges.begin();
    inMST[G.type] = 1;

    do
    {
        newNode = new FibHeapNode();
        newNode->SetKeyValue(1 - mit->second->CCS);
        sn = (mit->second)->getSrc();
        if (G.nodeIndex[G.type]->getSn() == sn)
        {
            sn = (mit->second)->getDest();
        }

        newNode->SetIndexValue(sn);
        fibNodeVec[sn] = newNode;
        hp.Insert(newNode);
        inHeap[sn] = 1;
        cheap[sn] = mit->second;
        mit++;
    }
    while (mit != G.nodeIndex[G.type]->adjEdges.end());

    while (hp.GetNumNodes() > 0)
    {
        outNode = hp.ExtractMin();
        sn = outNode->GetIndexValue();
        inHeap[sn] = 0;
        inMST[sn] = 1;
        priq.push(cheap[sn]);

        for (mit = G.nodeIndex[sn]->adjEdges.begin();
                mit != G.nodeIndex[sn]->adjEdges.end();
                ++mit)
        {
            v = mit->second->getSrc();
            if (v == sn)
            {
                v = mit->second->getDest();
            }

            if (inHeap[v] == 1 && 1 - mit->second->CCS < fibNodeVec[v]->GetKeyValue())
            {
                hp.DecreaseKey(fibNodeVec[v], 1 - mit->second->CCS);
                cheap[v] = mit->second;
            }
            else
            {
                if (inHeap[v] == 0 && !inMST[v])
                {
                    newNode = new FibHeapNode();
                    newNode->SetKeyValue(1 - mit->second->CCS);
                    newNode->SetIndexValue(v);
                    fibNodeVec[v] = newNode;
                    hp.Insert(fibNodeVec[v]);
                    cheap[v] = mit->second;
                    inHeap[v] = 1;

                }
            }//else
        }//for
    }
}

void clustering(int mu,Graph& G,priority_queue<Edge*, vector<Edge*>, cmp>& priq,
                vector<long>& result,network_info& info,float b,float e,
                FibHeap &hp,const float p,int v)
{
    clock_t beginofclu = clock();
    vector<Cluster>::iterator rit;
    list<Cluster*> ::iterator lit;
    set<long>::iterator sit, sit1;
    vector<long>::iterator vlit;
    long j = 1, n1, n2;
    long k = 0;
    vector<Edge*> etemp;                         //存储同一趟聚类的边，它们具有相同eps
    vector<long> vec, vec2, vec3;
    vec.resize(G.nodeIndex.size());
    float delta = 0;
    float cQs = 0;                             //本趟聚类Qs
    float lQs = 0;                             //上一趟聚类所得Qs ，用于计算下降幅度
    float Qs = 0;                              //最大Qs
    const float readNum = b * (G.verNum() - 1); //第一阶段聚类读入的边数 此阶段不计算Qs，不判断其增大或减小，只进行聚类
    float epsilon = 0;
    float time1 = 0;                         //统计时间
    float time2 = 0;                         //统计时间
    float t3 = 0;                            //统计时间

        while (!priq.empty() && k < readNum)
        {
            etemp.push_back(priq.top());
            priq.pop();
            k++;
            while (!priq.empty() && etemp[0]->CCS == priq.top()->CCS)     //将CCS相同的边一次性加入，同时进行聚类
            {
                etemp.push_back(priq.top());
                priq.pop();
                k++;
            }

            G.mergeQs(etemp, cQs, mu,k,hp,time1);                           //聚类合并
            etemp.clear();
        }//while
        Qs = cQs;


    long i =1;
    for (lit = G.resultlist.begin(); lit != G.resultlist.end(); ++lit)
    {
        for(sit = (*lit)->nodeSet.begin(); sit != (*lit)->nodeSet.end(); sit++)
        {
            result[*sit] = i;
        }
        i++;
    }
    for (lit = G.alonelist.begin(); lit != G.alonelist.end(); ++lit)
    {
        sit =  (*lit)->nodeSet.begin();
        result[*sit] = 0;
    }

    clock_t mid = clock();

        while (!priq.empty())
        {

            etemp.push_back(priq.top());
            priq.pop();

            while (!priq.empty() && etemp[0]->CCS == priq.top()->CCS)
            {
                etemp.push_back(priq.top());
                priq.pop();
            }

            clock_t tt = clock();
            if(etemp[0]->CCS<p)
                break;
            Qs = G.mergeQs(etemp, Qs, cQs,result,epsilon, mu,hp,time1,time2);
            savecurrentState(G,etemp[0]->CCS);
            clock_t tf = clock();
            t3 += tf-tt;
            if (cQs < (1 - e) * lQs)
            {
                break;
            }
            else
            {
                lQs = cQs;
            }
            etemp.clear();
        }//while


    clock_t last = clock();
    vector<Cluster> cvec;
    cvec.resize(1);
    bool flag = 0;

    for(i = G.type; i<result.size(); i++)
    {
        if(cvec.size()<result[i]+1)
        {
            cvec.resize(result[i]+1);

        }

        cvec[result[i]].nodeSet.insert(i);

        if(result[i] == result.size() - 1)
        {
            flag = 1;
        }
    }
    i = 0;

    info.comm_number = cvec.size()-1;
    info.Qs = Qs;
    clock_t end = clock();

    info.begin_time = beginofclu;
    info.end_time = end;
    info.eps = epsilon;
    j = 1;

    long maxsize = 0, minsize = G.nodeIndex.size();
    double average = 0;

    for (rit = cvec.begin()+1; rit != cvec.end(); ++rit)
    {
        outfile2 << "cluster " << j << "  number:[" << rit->nodeSet.size()
        << "]" << endl;
        for (sit = rit->nodeSet.begin(); sit != rit->nodeSet.end(); ++sit)
        {
            outfile1 << *sit << "  " << j << endl;
            outfile2 << *sit << " ";
            vec[*sit] = j;
        }
        outfile2 << endl;

        if (rit->nodeSet.size() > maxsize)
        {
            maxsize = rit->nodeSet.size();
        }
        if (rit->nodeSet.size() < minsize)
        {
            minsize = rit->nodeSet.size();
        }
        average += rit->nodeSet.size();
        outfile2 << endl;
        j++;
    }

    average /= cvec.size()-1;
    info.min_comm_size = minsize;
    info.max_comm_size = maxsize;
    info.ave_comm_size = average;

    set<long> iset;

    if(cvec[0].nodeSet.size()!= 0)
    {
        for (sit = cvec[0].nodeSet.begin(); sit != cvec[0].nodeSet.end(); ++sit)
        {
            for (sit1 = G.nodeIndex[*sit]->adjNodes.begin();
                    sit1 != G.nodeIndex[*sit]->adjNodes.end();
                    ++sit1)
            {
                if (result[*sit1] != 0)
                {
                    iset.insert(vec[*sit1]);
                }
            }

            if (iset.size() < 2)
            {
                vec3.push_back(*sit);            //离群点
            }
            else
            {
                vec2.push_back(*sit);               //中心点
            }
            iset.clear();
        }
    }

    info.hubs_number = vec2.size();
    info.out_number = vec3.size();
    outfile2 << "hubs: ";
    for(vlit = vec2.begin(); vlit!= vec2.end(); ++vlit)
    {
        outfile2<<*vlit<<"  ";
    }
    outfile2 << "\n\noutliers: ";
    for(vlit = vec3.begin(); vlit!= vec3.end(); ++vlit)
    {
        outfile2<<*vlit<<"  ";
    }
    clock_t endofclu = clock();
}
void savecurrentState(Graph &G,const float eps)
{
    ofstream os;
    os.open("5.txt",ofstream::app);
    os<<"eps:"<<eps<<endl;
    list <Cluster*> ::iterator it = G.resultlist.begin();
    set<long>::iterator sit;
    int i =1;
    for(;it!=G.resultlist.end();++it)
    {
        os<<"cluster ["<<i<<"]"<<endl;
        for(sit = (*it)->nodeSet.begin();sit != (*it)->nodeSet.end();++sit)
        {
            os<<*sit<<"  ";
        }
        i++;
        os<<endl;
    }
    os<<"------------------------------------------------------------"<<endl;
}
