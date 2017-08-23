import networkx as nx
import matplotlib.pyplot as plt
from pscan_algo import to_csr_graph
from itertools import chain


def vis_input(graph, min_cn, similar_deg_lst, min_pts, graph_name):
    """
    :type graph: nx.Graph
    """

    # draw background graph
    pos = nx.circular_layout(graph)
    nx.draw_networkx_nodes(graph, with_labels=True, pos=pos, font_size=12, node_size=380, alpha=1.0, width=4,
                           node_color='black')
    nx.draw_networkx(graph, with_labels=True, pos=pos, font_size=12, node_size=350, alpha=1.0, width=4,
                     edge_color='grey', node_color='white')

    # parse edge list and its property
    src_u_lst = list(chain.from_iterable(
        map(lambda pair: [pair[0] for _ in xrange(pair[1])], zip(range(len(deg_lst)), deg_lst))))
    edge_with_property_lst = filter(lambda pair: pair[0][0] < pair[0][1], zip(zip(src_u_lst, dst_v_lst), min_cn))
    print edge_with_property_lst
    nx.draw_networkx_edge_labels(graph, pos=pos, edge_labels=dict(edge_with_property_lst))

    blue_edges = map(lambda pair: pair[0], filter(lambda pair: pair[1] == -2, edge_with_property_lst))
    nx.draw_networkx_edges(graph, pos=pos, edgelist=blue_edges, edge_color='b', width=4)
    red_edges = map(lambda pair: pair[0], filter(lambda pair: pair[1] == -1, edge_with_property_lst))
    nx.draw_networkx_edges(graph, pos=pos, edgelist=red_edges, edge_color='r', width=4)

    alpha_lst = map(lambda similar_deg: min(float(similar_deg) / min_pts, 1), similar_deg_lst)
    print alpha_lst
    for idx, alpha in enumerate(alpha_lst):
        nx.draw_networkx_nodes(graph, with_labels=True, pos=pos, font_size=12, node_size=350, alpha=alpha,
                               nodelist=[idx], node_color='r')

    plt.axis('off')
    plt.savefig('./' + graph_name + '.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


if __name__ == '__main__':
    graph = nx.read_edgelist('demo_input_graph.txt', nodetype=int)

    offset_lst, dst_v_lst, deg_lst = to_csr_graph(graph)
    print 'csr representation:\noffset_lst=', offset_lst, '\ndst_v_lst=', dst_v_lst, '\ndeg_lst=', deg_lst, '\n'

    # demo input graph
    min_cn = [0 for _ in xrange(len(dst_v_lst))]
    similar_degree_lst = [0 for _ in xrange(len(deg_lst))]
    vis_input(graph, min_cn=min_cn, similar_deg_lst=similar_degree_lst, min_pts=3, graph_name='demo_input_graph')

    # after 1. pruning
    min_cn = [-1, 0, 3, 3, 3, 0, 3, 3, 0, 0, 3, 0, 0, 0, 3, 0, 3, 0, 3, 3, 4, 0, 3, 3, 0, 0, 3, 0, 0, 0, -2, -2, 0, 0]
    vis_input(graph, min_cn=min_cn, similar_deg_lst=similar_degree_lst, min_pts=3, graph_name='after_pruning_graph')

    # after 2.1 check core
    min_cn = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1,
              -1, -1, -1, -1, -2, -2, -2, -2]
    vis_input(graph, min_cn=min_cn, similar_deg_lst=similar_degree_lst, min_pts=3,
              graph_name='after_check_core_1st_bsp_graph')

    # after 2.2 check core
    similar_degree_lst = [1, 4, 3, 3, 3, 0, 3, 3, 3, 3, 0, 0]
    min_cn = [-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -2, -2, -1, -1, -1, -1, -1, -1, -1, -1,
              -1, -1, -1, -1, -2, -2, -2, -2]
    vis_input(graph, min_cn=min_cn, similar_deg_lst=similar_degree_lst, min_pts=3,
              graph_name='after_check_core_2nd_bsp_graph')
