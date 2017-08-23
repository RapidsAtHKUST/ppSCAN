import networkx as nx
import matplotlib.pyplot as plt
from pscan_algo import to_csr_graph


def vis_input(graph):
    """
    :type graph: nx.Graph
    """
    pos = nx.circular_layout(graph)
    nx.draw_networkx_nodes(graph, with_labels=True, pos=pos, font_size=16, node_size=700, alpha=1.0, width=4,
                           node_color='black')
    nx.draw_networkx(graph, with_labels=True, pos=pos, font_size=16, node_size=600, alpha=1.0, width=4,
                     edge_color='grey', node_color='white')

    plt.axis('off')
    plt.savefig('./demo_input_graph.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


if __name__ == '__main__':
    graph = nx.read_edgelist('demo_input_graph.txt', nodetype=int)
    vis_input(graph)

    offset_lst, dst_v_lst, deg_lst = to_csr_graph(graph)
    print 'csr representation:\noffset_lst=', offset_lst, '\ndst_v_lst=', dst_v_lst, '\ndeg_lst=', deg_lst, '\n'
