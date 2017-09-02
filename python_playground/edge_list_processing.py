import networkx as nx
import pandas


def read_edge_construct_graph(my_path):
    graph = nx.read_edgelist(my_path)
    assert isinstance(graph, nx.DiGraph)
    print graph.number_of_nodes()
    print graph.number_of_edges()


def read_csv(my_path):
    my_arr = pandas.read_csv(my_path)
    # not skip #
    print len(my_arr)


if __name__ == '__main__':
    # read 500M edge list file
    my_path = '/home/yche/GitRepos/ScanOptimizing/dataset/com-lj.ungraph.txt'
