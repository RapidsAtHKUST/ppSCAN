/*
forKNL (AVX-512)
*/

#include "scanxp.h"

int main(int argc,char *argv[])
{
  
  //Graph_input
  int cluster_num = 0;
  int hub_num = 0;
  int out_num = 0;
  
  NUMT = atoi(argv[2]);
  GRAPH g(argv[1]);//file_input
  UnionFind uf(g.nodemax);
  cout << "graph_name"<< argv[1] << endl;
  
  #pragma omp parallel for num_threads(NUMT) schedule(dynamic ,100000)
  for(int i = 0;i<g.nodemax;i++){
    g.core_count[i] = 0;
    g.label[i] = UNCLASSIFIED;
    g.core[i] = false;
    for(int n=g.node[i];n<g.node[i+1];n++){
      g.edgef[n]=i;
      g.common_node[n] = 2;
    }
  }

  double init2 = omp_get_wtime();
  
  //step1 core_detection
  cout << "STEP1" << endl;
  double s1 = omp_get_wtime();
  core_detection(&g);
  double e1 = omp_get_wtime();

//step2 cluster_construction
  /*
    This step constructs cluster using Union-Find tree.
    Each threads detects that core and adjacent nodes constructs cluster.
    Clusters are expand by merging other cluster.
  */
  cout << "STEP2" << endl;
  double s2 = omp_get_wtime();
  #pragma omp parallel for num_threads(NUMT) schedule(dynamic ,2000)
  for(int i=0;i<g.nodemax;i++)
    {
      
      int rx,ry,index;
      if(g.core[i] == true){
	g.label[i] = CMEMBER;
	for(int n = g.node[i];n<g.node[i+1];n++){
	  if(g.similarity[n] != true)continue;
	  g.label[g.edge[n]] = CMEMBER;

	  
	  do{
	    rx = uf.root(i);
	    ry = uf.root(g.edge[n]);
	    if(rx < ry){
	      index = rx;
	      rx = ry;
	      ry = index;
	    }
	    
	    if(rx == ry)break;
	    
	    
	  }while(!__sync_bool_compare_and_swap(&(uf.data[rx]),rx,ry));
	}
	
      }
    }
  // */

 
   
    double e2 = omp_get_wtime();
    cout << "STEP3" << endl;

    double s3 = omp_get_wtime();
    
    //STEP3 Hub Outlier detection
    #pragma omp parallel for num_threads(NUMT) schedule(dynamic ,1000)
    for(int i = 0;i<g.nodemax;i++){
    
      if(g.label[i] == CMEMBER)continue;
      
      if(hub_check_uf(&g,&uf,i) == true){
	g.label[i] = HUB;
      }
    } 

    
    double e3 = omp_get_wtime();


    //Result output
    set<int> c;
    for(int i = 0;i<g.nodemax;i++){
      if(g.label[i] == CMEMBER){
	c.insert(uf.root(i));
	
      }
    }

    cluster_num = c.size();
    
    #pragma omp parallel for num_threads(NUMT) reduction(+:hub_num,out_num)
    for(int i = 0;i<g.nodemax;i++){
      if(g.label[i] == HUB){
	hub_num++;
      } else if(g.label[i] == UNCLASSIFIED){
	out_num++;
      }
    }

    /*
    ofstream ofresult("result.txt",ios::app);
    for(int i = 0; i< g.nodemax; i++){
      if(g.label[i] == CMEMBER){
	ofresult << i << "\t" << uf.root(i) << endl;
      }else{
	ofresult << i << endl;
      }
    }
    // */

    
    
    
      cout << "Detect_cluster: " << cluster_num << endl;
      cout << "Hub: " << hub_num << endl;
      cout << "Outlier: " << out_num << endl;
      cout << "STEP1_core_detection:" << (double)(e1 - s1)<< endl;
      cout << "STEP2_cluster_construction:" << (double)(e2 - s2)<< endl;
      cout << "STEP3_hub_and_outlier_detection:" << (double)(e3 - s3) << endl;
      cout << "Number_of_threads" << NUMT << endl;
      cout << g.nodemax << endl;
      cout << g.edgemax << endl;
      
      
      
}

/*
Step1 'core_detection' detects all core nodes in graph.
Firstly, this method calclates set intersection of all edges.
Next, this method calclates stractural similarity based on result of set intersection.
Finaly, this method determines whether all nodes are core or not.
*/
inline void core_detection(GRAPH *g)
{

__m512i st = _mm512_set_epi32(3,3,3,3,2,2,2,2,1,1,1,1,0,0,0,0);
 __m512i ssecountplus = _mm512_set1_epi32(1);
 
#pragma omp parallel for num_threads(NUMT) schedule(dynamic ,6000)
 for(int i=0;i<(*g).edgemax;i++){
 
   int cnv[PARA] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   int to,jo;
  __m512i ssecnv = _mm512_set1_epi32(0);
    int j,t,j2,t2;

  if((*g).node[(*g).edgef[i]+1] - (*g).node[(*g).edgef[i]] < (*g).node[(*g).edge[i]+1] - (*g).node[(*g).edge[i]]){
    j=(*g).node[(*g).edgef[i]];
    t=(*g).node[(*g).edge[i]];
    j2=(*g).node[(*g).edgef[i]+1];
    t2 =(*g).node[(*g).edge[i]+1];
  }else{
    t=(*g).node[(*g).edgef[i]];
    j=(*g).node[(*g).edge[i]];
    t2=(*g).node[(*g).edgef[i]+1];
    j2 =(*g).node[(*g).edge[i]+1];
  }
  // /*
  int size1;
  size1 = (t2-t)/(j2-j);
  
  if(size1 > 2){
   
    __m512i jnode = _mm512_set1_epi32((*g).edge[j]);
    __m512i tnode = _mm512_loadu_si512((__m512i*)((*g).edge + t));
    
    int vsize = ((t2 - t) / PARA)*PARA + t;
    
    while(j<j2 && t<vsize){
      
      __mmask16 mask  = _mm512_cmpeq_epi32_mask(jnode, tnode);
      ssecnv = _mm512_mask_add_epi32(ssecnv,mask,ssecnv,ssecountplus);
      
      if((*g).edge[j] > (*g).edge[t+15]){
	t+=PARA;
	tnode = _mm512_loadu_si512((__m512i*)((*g).edge + t));  
      } else {
	j++;
	jnode = _mm512_set1_epi32((*g).edge[j]);    
      }  
    }
    
    _mm512_store_si512((__m512i*)cnv,ssecnv);
    
    for(int cnvplus=0;cnvplus < PARA;cnvplus++){
      (*g).common_node[i] += cnv[cnvplus];
    }
    
    
    t = vsize;
    
  }else{
    int jsize = ((j2 - j) / 4)*4 + j;
    int tsize = ((t2 - t) / 4)*4 + t;
    
    __m512i jnode,tnode;
    
    jnode = _mm512_loadu_si512((__m512i*)((*g).edge + j));
    tnode = _mm512_loadu_si512((__m512i*)((*g).edge + t));
    
    __mmask16 mask;
    while(j<jsize && t<tsize){
      
      __m512i jnodeA = _mm512_permutevar_epi32(st,jnode);
      __m512i tnodeA = _mm512_permute4f128_epi32(tnode,0b00000000);
      __mmask16 mask  = _mm512_cmpeq_epi32_mask(jnodeA, tnodeA);
      ssecnv = _mm512_mask_add_epi32(ssecnv,mask,ssecnv,ssecountplus);
      
      if((*g).edge[j+3] == (*g).edge[t+3]){
	j+=4;
	t+=4;
	jnode = _mm512_loadu_si512((__m512i*)((*g).edge + j));
	tnode = _mm512_loadu_si512((__m512i*)((*g).edge + t));
      }else if((*g).edge[j+3] > (*g).edge[t+3]){
	t+=4;
	
	tnode = _mm512_loadu_si512((__m512i*)((*g).edge + t));
	tnodeA = _mm512_permute4f128_epi32(tnode,0b00000000);
      }else{
	j+=4;
	jnode = _mm512_loadu_si512((__m512i*)((*g).edge + j));
      }
      
    }
    
    _mm512_store_si512((__m512i*)cnv,ssecnv);
    
    for(int cnvplus=0;cnvplus < PARA;cnvplus++){
      (*g).common_node[i] += cnv[cnvplus];
    }
    
    if(j >= jsize){
      j = jsize;
    }else{
      t = tsize;
    }
    
  }
 // */
  while(j<j2 && t<t2){
    
    if((*g).edge[j] == (*g).edge[t]){
      (*g).common_node[i]++;
      j++;
      t++;
    }else if((*g).edge[j] > (*g).edge[t]){
      t++;
      
    }else{
      j++;
      
    }
  }
  
  
  
}
  

  
  #pragma omp parallel for num_threads(NUMT) 
  for(int i=0;i<(*g).edgemax;i++)
    {

      (*g).ss[i] = (*g).common_node[i]/sqrt((((*g).node[(*g).edgef[i]+1]-(*g).node[(*g).edgef[i]])+1)*(((*g).node[((*g).edge[i]+1)]-(*g).node[(*g).edge[i]])+1));
      }


  #pragma omp parallel for num_threads(NUMT)
  for(int i=0;i<(*g).nodemax;i++)
    {
      for(int n=(*g).node[i];n<(*g).node[i+1];n++){
	if((*g).ss[n]>=EPSILON){
	  (*g).core_count[i]++;
	  (*g).similarity[n] = true;
	}else{
	  (*g).similarity[n] = false;
	}
      }
      
    }

 
#pragma omp parallel for num_threads(NUMT)
  for(int i=0;i<(*g).nodemax;i++)
    {
      if((*g).core_count[i]>=MYU){
	(*g).core[i] = true;
      }else{
	(*g).core[i] = false;
      }
      
    }
}

inline bool hub_check_uf(GRAPH *g,UnionFind *uf,int a)
{
  int counter = 0;
  set<int> c;
  bool q;
  
  for(int i=(*g).node[a];i<(*g).node[a+1];i++){
    
    if((*g).label[(*g).edge[i]] != CMEMBER)continue;
    c.insert((*uf).root((*g).edge[i]));
  }
  
  if(c.size() >= 2){
    q = true;
  }else{
    q = false;
  }

  return q;
}
