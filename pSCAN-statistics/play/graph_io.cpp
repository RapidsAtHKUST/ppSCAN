
#include <cstdio>
#include "../Utility.h"

//
// Created by yche on 8/2/17.
//

int *degree;
int n;
int m;

ui *pstart;
int *edges;
ui *reverse_;
int *min_cn;

using namespace std;

void read_graph(string dir) {
    FILE *f = open_file((dir + string("/b_degree.bin")).c_str(), "rb");

    int tt;
    fread(&tt, sizeof(int), 1, f);
    if (tt != (int) sizeof(int)) {
        printf("sizeof int is different: edge.bin(%d), machine(%d)\n", tt, (int) sizeof(int));
        return;
    }
    fread(&n, sizeof(int), 1, f);
    fread(&m, sizeof(int), 1, f);

    degree = new int[n];
    fread(degree, sizeof(int), n, f);

#ifdef _DEBUG_
    long long sum = 0;
    for (ui i = 0; i < n; i++) sum += degree[i];
    if (sum != m) printf("WA input graph\n");
#endif

    fclose(f);

    f = open_file((dir + string("/b_adj.bin")).c_str(), "rb");

    pstart = new ui[n + 1];
    edges = new int[m];
    reverse_ = new ui[m];
    min_cn = new int[m];
    memset(min_cn, 0, sizeof(int) * m);

    int *buf = new int[n];

    pstart[0] = 0;
    for (ui i = 0; i < n; i++) {
        //printf("%d %d\n", i, degree[i]);
        if (degree[i] > 0) fread(buf, sizeof(int), degree[i], f);

        for (ui j = 0; j < degree[i]; j++) edges[pstart[i] + j] = buf[j];

        pstart[i + 1] = pstart[i] + degree[i];

        ++degree[i];
    }

    delete[] buf;

    fclose(f);

    for (ui i = 0; i < n; i++) {
        for (ui j = pstart[i]; j < pstart[i + 1]; j++) {
            if (edges[j] == i) {
                cout << "Self loop\n";
                //exit(1);
            }
            if (j > pstart[i] && edges[j] <= edges[j - 1]) {
                cout << "Edges not sorted in increasing id order!\nThe program may not run properly!\n";
                //exit(1);
            }
        }
    }
}

int main(int argc, char **argv) {
    read_graph(argv[1]);
}