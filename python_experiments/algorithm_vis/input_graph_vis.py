import networkx as nx
import matplotlib.pyplot as plt
from itertools import chain


def vis_input(graph):
    """
    :type graph: nx.Graph
    """
    pos = nx.circular_layout(graph)
    nx.draw(graph, with_labels=True, pos=pos, font_size=16, node_size=700, alpha=1, width=4,
            node_color='black')
    nx.draw(graph, with_labels=True, pos=pos, font_size=16, node_size=600, alpha=1, width=4,
            edge_color='grey', node_color='white')

    plt.axis('off')
    plt.savefig('./demo_graph.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


# pre-processing
def to_csr_graph(graph):
    """
    :type graph: nx.Graph
    """
    # 1st: deg_lst
    deg_lst = range(max(graph.nodes()) + 1)
    for node in graph.nodes():
        deg_lst[node] = nx.degree(graph, node)
    deg_lst = map(lambda node: nx.degree(graph, node), graph.nodes())
    assert 2 * graph.number_of_edges() == sum(deg_lst)

    # 2nd: offset_lst
    offset_lst = [0] * (len(deg_lst) + 1)
    # prefix sum
    for i in xrange(len(deg_lst)):
        offset_lst[i + 1] = offset_lst[i] + deg_lst[i]

    # 3rd: dst_v_lst
    dst_v_lst = list(chain.from_iterable(map(lambda node: graph.neighbors(node), graph.nodes())))
    return offset_lst, dst_v_lst, deg_lst


class PScan:
    not_direct_reachable = -2
    direct_reachable = -1
    not_sure = 0

    def __init__(self, offset_lst, dst_v_lst, deg_lst, eps, min_pts):
        # parameters
        self.eps = eps
        self.min_pts = min_pts

        self.n = len(deg_lst)

        # offset and vertex properties
        self.offset_lst = offset_lst
        self.inc_degree_lst = map(lambda degree_val: degree_val + 1, deg_lst)
        self.similar_degree_lst = [0] * len(self.inc_degree_lst)

        # dst_v and edeg properties
        self.dst_v_lst = dst_v_lst
        self.min_cn_lst = [PScan.not_sure] * len(self.dst_v_lst)

        # statistics
        self.prune0 = 0  # definitely not reachable
        self.prune1 = 0  # definitely reachable

    def is_definitely_not_reachable(self, u, v):
        du = self.inc_degree_lst[u]
        dv = self.inc_degree_lst[v]
        return True if min(du, dv) < max(du, dv) * self.eps else False

    def eval_density(self, u, edge_idx):
        v = dst_v_lst[edge_idx]
        cn = 2

        du = deg_lst[u] + 1
        dv = deg_lst[v] + 1
        offset_nei_u = dst_v_lst[u]
        offset_nei_v = dst_v_lst[v]
        # min_cn_num=min_cn_num[edge_idx]
        # while offset_nei_u < dst_v_lst[u + 1] and offset_nei_v < dst_v_lst[v + 1] and cn <

    def prune(self):
        for i in xrange(self.n):
            for j in xrange(offset_lst[i], offset_lst[i + 1]):
                if i < j:
                    if self.is_definitely_not_reachable(i, j):
                        self.prune0 += 1
                        self.min_cn_lst[j] = PScan.not_direct_reachable
                    else:
                        pass
                        # self.min_cn_lst[]


if __name__ == '__main__':
    graph = nx.read_edgelist('demo_input_graph.txt', nodetype=int)
    to_csr_graph(graph)
    offset_lst, dst_v_lst, deg_lst = to_csr_graph(graph)
    print offset_lst, '\n', dst_v_lst, '\n', deg_lst
