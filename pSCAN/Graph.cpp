#include "Utility.h"
#include "Graph.h"

Graph::Graph(const char *_dir) {
	dir = string(_dir);

	n = m = 0;

	eps_a2 = eps_b2 = miu = 0;

	pstart = NULL;
	edges = NULL;
	reverse = NULL;
	min_cn = NULL;

	cid = NULL;

	degree = NULL;
	effective_degree = NULL;
	similar_degree = NULL;

	pa = NULL;
	rank = NULL;
}

Graph::~Graph() {
	if(pstart != NULL) {
		delete[] pstart;
		pstart = NULL;
	}
	if(edges != NULL) {
		delete[] edges;
		edges = NULL;
	}
	if(reverse != NULL) {
		delete[] reverse;
		reverse = NULL;
	}
	if(min_cn != NULL) {
		delete[] min_cn;
		min_cn = NULL;
	}
	if(cid != NULL) {
		delete[] cid;
		cid = NULL;
	}
	if(degree != NULL) {
		delete[] degree;
		degree = NULL;
	}
	if(effective_degree != NULL) {
		delete[] effective_degree;
		effective_degree = NULL;
	}
	if(similar_degree != NULL) {
		delete[] similar_degree;
		similar_degree = NULL;
	}
	if(pa != NULL) {
		delete[] pa;
		pa = NULL;
	}
	if(rank != NULL) {
		delete[] rank;
		rank = NULL;
	}
}

void Graph::read_graph() {
	FILE *f = open_file((dir + string("/b_degree.bin")).c_str(), "rb");

	int tt;
	fread(&tt, sizeof(int), 1, f);
	if(tt != (int)sizeof(int)) {
		printf("sizeof int is different: edge.bin(%d), machine(%d)\n", tt, (int)sizeof(int));
		return ;
	}
	fread(&n, sizeof(int), 1, f);
	fread(&m, sizeof(int), 1, f);

	// printf("\tn = %u; m = %u\n", n, m/2);

	degree = new int[n];
	fread(degree, sizeof(int), n, f);

#ifdef _DEBUG_
	long long sum = 0;
	for(ui i = 0;i < n;i ++) sum += degree[i];
	if(sum != m) printf("WA input graph\n");
#endif

	fclose(f);

	f = open_file((dir + string("/b_adj.bin")).c_str(), "rb");

	if(pstart == NULL) pstart = new ui[n+1];
	if(edges == NULL) edges = new int[m];
	if(reverse == NULL) reverse = new ui[m];
	if(min_cn == NULL) min_cn = new int[m];
	memset(min_cn, 0, sizeof(int)*m);

	int *buf = new int[n];

	pstart[0] = 0;
	for(ui i = 0;i < n;i ++) {
		//printf("%d %d\n", i, degree[i]);
		if(degree[i] > 0) fread(buf, sizeof(int), degree[i], f);

		for(ui j = 0;j < degree[i];j ++) edges[pstart[i] + j] = buf[j];

		pstart[i+1] = pstart[i] + degree[i];

		++ degree[i];
	}

	delete[] buf;

	fclose(f);

	for(ui i = 0;i < n;i ++) {
		for(ui j = pstart[i];j < pstart[i+1];j ++) {
			if(edges[j] == i) {
				printf("Self loop\n");
				//exit(1);
			}
			if(j > pstart[i]&&edges[j] <= edges[j-1]) {
				printf("Edges not sorted in increasing id order!\nThe program may not run properly!\n");
				//exit(1);
			}
		}
	}
}

ui Graph::binary_search(const int *array, ui b, ui e, int val) {
#ifdef _DEBUG_
	if(e < b) printf("??? WA1 in binary_search\n");
#endif
	-- e;
	if(array[e] < val) return e+1;
	while(b < e) {
		ui mid = b + (e-b)/2;
		if(array[mid] >= val) e = mid;
		else b = mid+1;
	}
#ifdef _DEBUG_
	if(array[e] < val) printf("??? WA2 in binary_search\n");
#endif
	return e;
}

void Graph::cluster_noncore_vertices(int eps_a2, int eps_b2, int mu) {
	if(cid == NULL) cid = new int[n];
	for(ui i = 0;i < n;i ++) cid[i] = n;

	for(ui i = 0;i < n;i ++) if(similar_degree[i] >= mu) {
		int x = find_root(i);
		if(i < cid[x]) cid[x] = i;
	}

	noncore_cluster.clear();
	noncore_cluster.reserve(n);
	for(ui i = 0;i < n;i ++) if(similar_degree[i] >= mu) {
		for(ui j = pstart[i];j < pstart[i+1];j ++) if(similar_degree[edges[j]] < mu) {
			if(min_cn[j] >= 0) {
				min_cn[j] = similar_check_OP(i, j, eps_a2, eps_b2);
				if(reverse[reverse[j]] != j) printf("WA cluster_noncore\n");
				min_cn[reverse[j]] = min_cn[j];
				if(min_cn[j] == -1) {
					++ similar_degree[i];
					++ similar_degree[edges[j]];
				}
			}

			if(min_cn[j] == -1) noncore_cluster.pb(mp(cid[pa[i]], edges[j]));
		}
	}
}

void Graph::output(const char *eps_s, const char *miu) {
	printf("\t*** Start write result into disk!\n");

	string out_name = dir + "/result-" + string(eps_s) + "-" + string(miu) + ".txt";
	FILE *fout = open_file(out_name.c_str(), "w");

	fprintf(fout, "c/n vertex_id cluster_id\n");

	int mu = atoi(miu);
	for(ui i = 0;i < n;i ++) if(similar_degree[i] >= mu) {
		fprintf(fout, "c %d %d\n", i, cid[pa[i]]);
	}

	sort(noncore_cluster.begin(), noncore_cluster.end());
	noncore_cluster.erase(unique(noncore_cluster.begin(), noncore_cluster.end()), noncore_cluster.end());
	for(ui i = 0;i < noncore_cluster.size();i ++) {
		fprintf(fout, "n %d %d\n", noncore_cluster[i].second, noncore_cluster[i].first);
	}

	fclose(fout);
}

void Graph::pSCAN(const char *eps_s, int _miu) {
	get_eps(eps_s);
	miu = _miu;

	if(similar_degree == NULL) similar_degree = new int[n];
	memset(similar_degree, 0, sizeof(int)*n);

	if(effective_degree == NULL) effective_degree = new int[n];
	for(ui i = 0;i < n;i ++) effective_degree[i] = degree[i]-1;

	if(pa == NULL) pa = new int[n];
	if(rank == NULL) rank = new int[n];
	for(ui i = 0;i < n;i ++) {
		pa[i] = i;
		rank[i] = 0;
	}

#ifdef _LINUX_
	struct timeval start;
	gettimeofday(&start, NULL);
#else
	int start = clock();
#endif

	ui *edge_buf = new ui[n];
	int *cores = new int[n];
	int cores_n = 0;

	prune_and_cross_link(eps_a2, eps_b2, miu, cores, cores_n);
	//printf("\t*** Finished prune and cross link!\n");

#ifdef _LINUX_
	struct timeval end1;
	gettimeofday(&end1, NULL);

	long long mtime1, seconds1, useconds1;
	seconds1 = end1.tv_sec - start.tv_sec;
	useconds1 = end1.tv_usec - start.tv_usec;
	mtime1 = seconds1*1000000 + useconds1;
#else
	int end1 = clock();
#endif

	int *bin_head = new int[n];
	int *bin_next = new int[n];
	for(ui i = 0;i < n;i ++) bin_head[i] = -1;

	int max_ed = 0;
	for(ui i = 0;i < n;i ++) if(effective_degree[i] >= miu) {
		int ed = effective_degree[i];
		if(ed > max_ed) max_ed = ed;
		bin_next[i] = bin_head[ed];
		bin_head[ed] = i;
	}

	while(true) {
		int u = -1;
		if(cores_n) u = cores[-- cores_n];
		else {
			while(max_ed >= miu&&u == -1) {
				for(int x = bin_head[max_ed];x != -1;) {
					int tmp = bin_next[x];
					int ed = effective_degree[x];
					if(ed == max_ed) {
						u = x;
						bin_head[max_ed] = bin_next[x];
						break;
					}
					else if(ed >= miu) {
						bin_next[x] = bin_head[ed];
						bin_head[ed] = x;
					}
					x = tmp;
				}
				if(u == -1) {
					bin_head[max_ed] = -1;
					-- max_ed;
				}
			}
		}
		if(u == -1) break;

		int edge_buf_n = 0;
		for(ui j = pstart[u];j < pstart[u+1];j ++) {
			if(min_cn[j] == -2) continue;

			if(similar_degree[u] < miu||find_root(u) != find_root(edges[j])) edge_buf[edge_buf_n ++] = j;
		}

		int i = 0;
		while(similar_degree[u] < miu&&effective_degree[u] >= miu&&i < edge_buf_n) {
			ui idx = edge_buf[i];
			if(min_cn[idx] != -1) {
#ifdef _DEBUG_
				if(min_cn[idx] == 0) printf("WA min_cn!\n");
#endif
				int v = edges[idx];

				min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

				if(min_cn[idx] == -1) ++ similar_degree[u];
				else -- effective_degree[u];

				if(effective_degree[v] >= 0) {
					if(min_cn[idx] == -1) {
						++ similar_degree[v];

						if(similar_degree[v] == miu) cores[cores_n ++] = v;
					}
					else -- effective_degree[v];
				}
			}

			++ i;
		}

		effective_degree[u] = -1;

		if(similar_degree[u] < miu) continue;

		for(int j = 0;j < edge_buf_n;j ++) {
			ui idx = edge_buf[j];
			if(min_cn[idx] == -1&&similar_degree[edges[idx]] >= miu) my_union(u, edges[idx]);
		}

		while(i < edge_buf_n) {
			ui idx = edge_buf[i];
			int v = edges[idx];
			if(min_cn[idx] < 0||similar_degree[v] < miu||find_root(u) == find_root(v)) {
				++ i;
				continue;
			}

			min_cn[idx] = min_cn[reverse[idx]] = similar_check_OP(u, idx, eps_a2, eps_b2);

			if(effective_degree[v] >= 0) {
				if(min_cn[idx] == -1) {
					++ similar_degree[v];

					if(similar_degree[v] == miu) cores[cores_n ++] = v;
				}
				else -- effective_degree[v];
			}

			if(min_cn[idx] == -1) my_union(u,v);

			++ i;
		}
		//printf(")\n");
	}
	//printf("\t*** Finished clustering core vertices!\n");

	delete[] edge_buf; edge_buf = NULL;
	delete[] cores; cores = NULL;
	delete[] bin_head; bin_head = NULL;
	delete[] bin_next; bin_next = NULL;

#ifdef _LINUX_
	struct timeval end;
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - end1.tv_sec;
	useconds = end.tv_usec - end1.tv_usec;
	mtime = seconds*1000000 + useconds;

	//printf("Prune time: %lld\nRefine time: %lld\n", mtime1, mtime);
#else
	int end = clock();

	printf("Prune time: %d\nSort time: %d\nRefine time: %d\n", end1-start,end2-2end1,end-end2);
#endif

	cluster_noncore_vertices(eps_a2, eps_b2, miu);
}

int Graph::check_common_neighbor(int u, int v, int c) {
	int cn = 2;

	if(degree[u] > degree[v]) swap(u,v);

	int du = degree[u]+1, dv = degree[v]+1;
	ui i = pstart[u], j = pstart[v];
	while(i < pstart[u+1]&&j < pstart[v+1]&&cn < c&&du >= c&&dv >= c) {
		if(edges[i] < edges[j]) {
			-- du;
			++ i;
		}
		else if(edges[i] > edges[j]) {
			-- dv;
			++ j;
		}
		else {
			++ cn;
			++ i;
			++ j;
		}
	}

	if(cn >= c) return -1;
	return -2;
}

int Graph::similar_check_OP(int u, ui idx, int eps_a2, int eps_b2) {
	int v = edges[idx];

#ifdef _DEBUG_
	if(min_cn[idx] == -1||min_cn[idx] == -2) printf("??? WA in similar_check\n");
#endif

	if(min_cn[idx] == 0) {
		int du = degree[u], dv = degree[v];
		int c = compute_common_neighbor_lowerbound(du,dv,eps_a2,eps_b2);
	
#ifdef _DEBUG_
		if(du < c||dv < c) return -2;
#endif

		if(c <= 2) return -1;

		min_cn[idx] = min_cn[reverse[idx]] = c;
	}

	return check_common_neighbor(u, v, min_cn[idx]);
}

int Graph::compute_common_neighbor_lowerbound(int du,int dv,int eps_a2,int eps_b2) {
	int c = (int)(sqrtl((((long double)du)*((long double)dv)*eps_a2)/eps_b2));

#ifdef _DEBUG_
	if(((long long)du)*dv*eps_a2 < 0||((long long)c)*c*eps_b2 < 0) printf("??? Overflow in similar_check\n");
#endif

	if(((long long)c)*((long long)c)*eps_b2 < ((long long)du)*((long long)dv)*eps_a2) ++ c;

#ifdef _DEBUG_
	if(((long long)c)*((long long)c)*eps_b2 < ((long long)du)*((long long)dv)*eps_a2) printf("??? Wrong common neigbor computation in similar_check\n");
#endif
	return c;
}

void Graph::prune_and_cross_link(int eps_a2, int eps_b2, int miu, int *cores, int &cores_e) {
	for(ui i = 0;i < n;i ++) { //must be iterating from 0 to n-1
		for(ui j = pstart[i];j < pstart[i+1];j ++) {
			if(edges[j] < i) {
				if(min_cn[j] == 0) min_cn[j] = -2;
				continue; //this edge has already been checked
			}

			int v = edges[j];
			int a = degree[i], b = degree[v];
			if(a > b) swap(a, b);

			if(((long long)a)*eps_b2 < ((long long)b)*eps_a2) {
				min_cn[j] = -2;

				-- effective_degree[i];
				-- effective_degree[v];
			}
			else {
				int c = compute_common_neighbor_lowerbound(a, b, eps_a2, eps_b2);

#ifdef _DEBUG_
				if(a < c||b < c) printf("!!! HHH\n");
#endif

				if(c <= 2) {
					min_cn[j] = -1;

					++ similar_degree[i];
					++ similar_degree[v];

					if(similar_degree[i] == miu) cores[cores_e ++] = i;
					if(similar_degree[v] == miu) cores[cores_e ++] = v;
				}
				else min_cn[j] = c;
			}

			if(min_cn[j] != -2) {
			//else {
				ui r_id = binary_search(edges, pstart[v], pstart[v+1], i);
				reverse[j] = r_id;
				reverse[r_id] = j;

				min_cn[r_id] = min_cn[j];
			}
		}
	}
}

int Graph::find_root(int u) {
	int x = u;
	while(pa[x] != x) x = pa[x];

	while(pa[u] != x) {
		int tmp = pa[u];
		pa[u] = x;
		u = tmp;
	}

	return x;
}

void Graph::my_union(int u, int v) {
	int ru = find_root(u);
	int rv = find_root(v);

	if(ru == rv) return ;

	if(rank[ru] < rank[rv]) pa[ru] = rv;
	else if(rank[ru] > rank[rv]) pa[rv] = ru;
	else {
		pa[rv] = ru;
		++ rank[ru];
	}
}

void Graph::get_eps(const char *eps_s) {
	int i = 0, eps_a = 0, eps_b = 1;
	while(eps_s[i] != '\0'&&eps_s[i] != '.') {
		eps_a = eps_a*10 + (eps_s[i]-'0');
		++ i;
	}

	if(eps_s[i] == '.') {
		++ i;
		while(eps_s[i] != '\0') {
			eps_a = eps_a*10 + (eps_s[i]-'0');
			eps_b *= 10;
			++ i;
		}
	}

	if(eps_a > eps_b||eps_b > 100||eps_a <= 0) {
		printf("??? Wrong eps format: %d/%d, %s\n", eps_a, eps_b, eps_s);
		exit(1);
	}

	eps_a2 = eps_a * eps_a;
	eps_b2 = eps_b * eps_b;
}
