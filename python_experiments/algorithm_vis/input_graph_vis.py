import math
import networkx as nx
import matplotlib.pyplot as plt
from itertools import chain

from disjoint_set import DisjointSet


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
    plt.savefig('./demo_input_graph.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


# pre-processing, neighbors should be sorted
def to_csr_graph(graph):
    """
    :type graph: nx.Graph
    """
    # 1st: deg_lst
    deg_lst = [0] * (max(graph.nodes()) + 1)
    for node in graph.nodes():
        deg_lst[node] = nx.degree(graph, node)
    assert 2 * graph.number_of_edges() == sum(deg_lst)

    # 2nd: offset_lst
    offset_lst = [0] * (len(deg_lst) + 1)
    # prefix sum
    for i in xrange(len(deg_lst)):
        offset_lst[i + 1] = offset_lst[i] + deg_lst[i]

    # 3rd: dst_v_lst, should be sorted
    dst_v_lst = list(chain.from_iterable(map(lambda node: sorted(graph.neighbors(node)), graph.nodes())))
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

        # disjoint set
        self.disjoint_set = DisjointSet(self.n)

        # non-core clustering
        self.cluster_dict = [self.n] * self.n
        self.non_core_cluster = []

        # statistics
        self.prune0 = 0  # definitely not reachable
        self.prune1 = 0  # definitely reachable

        self.cmp0 = 0
        self.cmp1 = 0
        self.cmp_equ = 0

    def is_definitely_not_reachable(self, u, v):
        du = self.inc_degree_lst[u]
        dv = self.inc_degree_lst[v]
        return True if min(du, dv) < max(du, dv) * (self.eps ** 2) else False

    def compute_cn_lower_bound(self, u, v):
        return int(math.ceil(math.sqrt(self.inc_degree_lst[u] * self.inc_degree_lst[v]) * self.eps))

    # 1st: prune
    def prune(self):
        for i in xrange(self.n):
            for j in xrange(offset_lst[i], offset_lst[i + 1]):
                v = self.dst_v_lst[j]
                if i < v:
                    if self.is_definitely_not_reachable(i, v):
                        self.prune0 += 1
                        self.min_cn_lst[j] = PScan.not_direct_reachable
                    else:
                        tmp = self.compute_cn_lower_bound(i, v)
                        if tmp <= 2:
                            self.prune1 += 1
                            self.min_cn_lst[j] = PScan.direct_reachable
                        else:
                            self.min_cn_lst[j] = tmp

    def eval_density(self, u, edge_idx):
        v = dst_v_lst[edge_idx]
        cn = 2

        du = self.inc_degree_lst[u] + 1
        dv = self.inc_degree_lst[v] + 1
        offset_nei_u = offset_lst[u]
        offset_nei_v = offset_lst[v]
        min_cn_num = self.min_cn_lst[edge_idx]

        while offset_nei_u < offset_lst[u + 1] and offset_nei_v < offset_lst[v + 1] \
                and cn < min_cn_num <= du and dv >= min_cn_num:
            if self.dst_v_lst[offset_nei_u] < self.dst_v_lst[offset_nei_v]:
                du -= 1
                offset_nei_u += 1
                self.cmp0 += 1
            elif self.dst_v_lst[offset_nei_u] > self.dst_v_lst[offset_nei_v]:
                dv -= 1
                offset_nei_v += 1
                self.cmp1 += 1
            else:
                cn += 1
                offset_nei_v += 1
                offset_nei_u += 1
                self.cmp_equ += 1
        return PScan.direct_reachable if cn >= min_cn_num else PScan.not_direct_reachable

    # 2nd: check core 1st bsp computation
    def check_core_1st_bsp(self, u):
        for edge_idx in xrange(self.offset_lst[u], self.offset_lst[u + 1]):
            if self.min_cn_lst[edge_idx] > 0 and u <= self.dst_v_lst[edge_idx]:
                self.min_cn_lst[edge_idx] = self.eval_density(u, edge_idx)

    @staticmethod
    def binary_search(array, offset_beg, offset_end, val):
        mid = (offset_beg + offset_end) / 2
        if array[mid] == val:
            return mid
        return PScan.binary_search(array, offset_beg, mid, val) if val < array[mid] \
            else PScan.binary_search(array, mid + 1, offset_end, val)

    # 2nd: check core 2nd bsp computation
    def check_core_2nd_bsp(self, u):
        for edge_idx in xrange(self.offset_lst[u], self.offset_lst[u + 1]):
            v = self.dst_v_lst[edge_idx]
            if u > v:
                self.min_cn_lst[edge_idx] = self.min_cn_lst[
                    PScan.binary_search(self.dst_v_lst, self.offset_lst[v], self.offset_lst[v + 1], u)]
            if self.min_cn_lst[edge_idx] == PScan.direct_reachable:
                self.similar_degree_lst[u] += 1

    def is_definite_core_vertex(self, u):
        return self.similar_degree_lst[u] >= self.min_pts

    # 3rd: cluster cores
    def cluster_core(self, u):
        for edge_idx in xrange(self.offset_lst[u], self.offset_lst[u + 1]):
            if self.min_cn_lst[edge_idx] == PScan.direct_reachable \
                    and self.is_definite_core_vertex(self.dst_v_lst[edge_idx]):
                # print 'union ', u, self.dst_v_lst[edge_idx]
                self.disjoint_set.union(u, self.dst_v_lst[edge_idx])

    def mark_cluster_min_ele_as_id(self):
        for i in xrange(self.n):
            if self.is_definite_core_vertex(i):
                x = self.disjoint_set.find_root(i)
                if i < self.cluster_dict[x]:
                    self.cluster_dict[x] = i
        # print 'disjoint set root dict:', self.cluster_dict
        pass

    # 4th: cluster non-cores
    def cluster_non_core(self):
        self.mark_cluster_min_ele_as_id()
        for i in xrange(self.n):
            if self.is_definite_core_vertex(i):
                for j in xrange(self.offset_lst[i], self.offset_lst[i + 1]):
                    if not self.is_definite_core_vertex(self.dst_v_lst[j]) \
                            and self.min_cn_lst[j] == PScan.direct_reachable:
                        self.non_core_cluster.append(
                            (self.cluster_dict[self.disjoint_set.find_root(i)], self.dst_v_lst[j]))

    def output_result(self):
        for i in xrange(self.n):
            if self.is_definite_core_vertex(i):
                print 'c ', i, ' ', self.cluster_dict[self.disjoint_set.find_root(i)]

        for cluster_id, non_core_v in sorted(set(self.non_core_cluster)):
            print 'n ', non_core_v, ' ', cluster_id

    def run_algorithm(self):
        # 1st: prune
        self.prune()
        print 'min_cn_lst after prune:\t\t', self.min_cn_lst

        # 2nd: check core
        for i in xrange(self.n):
            self.check_core_1st_bsp(i)
        candidates = []
        for i in xrange(self.n):
            # print 'check core', i, 'neighbors:', self.dst_v_lst[self.offset_lst[i]:self.offset_lst[i + 1]]
            self.check_core_2nd_bsp(i)
            if self.is_definite_core_vertex(i):
                candidates.append(i)
        print 'min_cn_lst after check core:\t\t', self.min_cn_lst

        # 3rd: cluster cores
        print 'cores:', candidates
        for candidate in candidates:
            self.cluster_core(candidate)

        # 4th: cluster non-core
        self.cluster_non_core()

        # finally, output result
        print '\n', '\t'.join(['core/non-core', 'vertex id', 'cluster id(min core vertex id in this cluster)']), '\n'
        print 'c/n vertex_id cluster_id'
        self.output_result()


if __name__ == '__main__':
    graph = nx.read_edgelist('demo_input_graph.txt', nodetype=int)
    vis_input(graph)
    to_csr_graph(graph)
    offset_lst, dst_v_lst, deg_lst = to_csr_graph(graph)
    print 'offset_lst:', offset_lst, '\ndst_v_lst:', dst_v_lst, '\ndeg_lst:', deg_lst
    PScan(offset_lst, dst_v_lst, deg_lst, eps=0.6, min_pts=3).run_algorithm()
