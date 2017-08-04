import networkx as nx
import matplotlib.pyplot as plt
from networkx.drawing.nx_agraph import graphviz_layout


def vis_input(graph):
    pos = graphviz_layout(graph)
    nx.draw(graph, pos=pos)

    plt.axis('off')
    plt.show()


if __name__ == '__main__':
    graph = nx.read_edgelist('data/pscan_demo_edge_list.txt', nodetype=int)
    vis_input(graph)
