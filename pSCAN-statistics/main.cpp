#include "Utility.h"
#include "Graph.h"

void usage() {
	printf("Usage: [1]exe [2]graph-dir [3]similarity-threshold [4]density-threshold [5 optional]output\n");
}

int main(int argc, char *argv[]) {
	if(argc < 4) {
		usage() ;
		return 0;
	}

#ifdef _DEBUG_
	printf("**** Graph Clustering (Debug): %s, %s, %s *** ", argv[1], argv[2], argv[3]);
#else
	printf("**** Graph Clustering (Release): %s, %s, %s *** ", argv[1], argv[2], argv[3]);
#endif

	printf("\n");

#ifdef _LINUX_
	struct timeval start, end1, end;
	gettimeofday(&start, NULL);
#else
	int start, end1, end;
	start = clock();
#endif

	Graph *graph = new Graph(argv[1]);
	graph->read_graph();
	printf("\t*** Finished loading graph!\n");

#ifdef _LINUX_
	gettimeofday(&end1, NULL);

	long long mtime1, seconds1, useconds1;
	seconds1 = end1.tv_sec - start.tv_sec;
	useconds1 = end1.tv_usec - start.tv_usec;
	mtime1 = seconds1*1000000 + useconds1;
#else
	end1 = clock();
#endif

	graph->pSCAN(argv[2], atoi(argv[3]));

#ifdef _LINUX_
	gettimeofday(&end, NULL);

	long long mtime, seconds, useconds;
	seconds = end.tv_sec - start.tv_sec;
	useconds = end.tv_usec - start.tv_usec;
	mtime = seconds*1000000 + useconds;

	printf("Total time without IO: %lld\n", mtime-mtime1);
#endif

	if(argc >= 5&&strcmp(argv[4], "output") == 0) graph->output(argv[2], argv[3]);

	return 0;
}

