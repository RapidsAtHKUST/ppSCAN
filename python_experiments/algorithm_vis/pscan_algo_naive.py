import math
import networkx as nx
import copy
from itertools import chain

from disjoint_set import DisjointSet


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

    def print_min_cnt_lst(self, comment):
        print '{}, min_cn_lst:'.format(comment), list(zip(self.el_lst, self.min_cn_lst))

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

        self.src_lst = [0] * len(dst_v_lst)
        for u in range(0, len(offset_lst) - 1):
            for i in range(offset_lst[u], offset_lst[u + 1]):
                self.src_lst[i] = u
        self.el_lst = list(zip(self.src_lst, self.dst_v_lst))

        # disjoint set
        self.disjoint_set = DisjointSet(self.n)

        # non-core clustering
        self.cluster_dict = [self.n] * self.n
        self.non_core_cluster = []

        # 1. statistics for prune
        self.prune0 = 0  # definitely not reachable
        self.prune1 = 0  # definitely reachable

        # 2.1 statistics for check core 1st bsp: set intersection with early stop
        self.intersect = 0
        self.cmp0 = 0
        self.cmp1 = 0
        self.cmp_equ = 0

        # 2.2 statistics for check core 2nd bsp: binary search
        self.binary_search_call = 0

        # 3 statistics for disjoint set

        self.result_lines = []

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
        self.intersect += 1
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

    def binary_search(self, array, offset_beg, offset_end, val):
        self.binary_search_call += 1
        mid = (offset_beg + offset_end) / 2
        if array[mid] == val:
            return mid
        return self.binary_search(array, offset_beg, mid, val) if val < array[mid] \
            else self.binary_search(array, mid + 1, offset_end, val)

    # 2nd: check core 2nd bsp computation
    def check_core_2nd_bsp(self, u):
        for edge_idx in xrange(self.offset_lst[u], self.offset_lst[u + 1]):
            v = self.dst_v_lst[edge_idx]
            if u > v:
                self.min_cn_lst[edge_idx] = self.min_cn_lst[
                    self.binary_search(self.dst_v_lst, self.offset_lst[v], self.offset_lst[v + 1], u)]
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
                line = ' '.join(map(str, ['c', i, self.cluster_dict[self.disjoint_set.find_root(i)]]))
                self.result_lines.append(line)
                print line

        for cluster_id, non_core_v in sorted(set(self.non_core_cluster)):
            line = ' '.join(map(str, ['n', non_core_v, cluster_id]))
            self.result_lines.append(line)
            print line

    def run_algorithm(self):
        print ', '.join(['not_direct_reachable = -2', 'direct_reachable = -1', 'not_sure = 0',
                         '>0 means min_cn to satisfy direct reachable']), '\n'

        # 1st: prune
        self.prune()
        self.print_min_cnt_lst('1. after prune')

        # 2nd: check core
        for i in xrange(self.n):
            self.check_core_1st_bsp(i)
        candidates = []
        self.print_min_cnt_lst('2.1 after check core 1st bsp, min_cn_lst :')

        for i in xrange(self.n):
            # print 'check core', i, 'neighbors:', self.dst_v_lst[self.offset_lst[i]:self.offset_lst[i + 1]]
            self.check_core_2nd_bsp(i)
            if self.is_definite_core_vertex(i):
                candidates.append(i)
        self.print_min_cnt_lst('2.2 after check core 2nd bsp')
        print '2.2 after check core 2nd bsp, similar_degree_lst:', self.similar_degree_lst
        print '2.2 after check core 2nd bsp, cores:', candidates

        # 3rd: cluster cores
        for candidate in candidates:
            self.cluster_core(candidate)
        print '3. after cluster core, disjoint set - parent dict:', dict(zip(range(self.n), self.disjoint_set.parent))

        # 4th: cluster non-core
        self.cluster_non_core()
        print '4. after cluster non-core mark cluster id, cluster(represented by root vertex), min ele id:', dict(
            filter(lambda pair: pair[1] != self.n, zip(range(self.n), self.cluster_dict)))

        # finally, output result
        print '\nfinal result in format:', ' '.join(
            ['core/non-core', 'vertex id', 'cluster id(min core vertex id in this cluster)'])
        self.result_lines.append('c/n vertex_id cluster_id')
        print 'c/n vertex_id cluster_id'
        self.output_result()


if __name__ == '__main__':
    graph = nx.read_edgelist('demo_input_graph.txt', nodetype=int)
    offset_lst, dst_v_lst, deg_lst = to_csr_graph(graph)

    print 'csr representation:\noffset_lst=', offset_lst, '\ndst_v_lst=', dst_v_lst, '\ndeg_lst=', deg_lst, '\n'

    pscan_algo = PScan(offset_lst, dst_v_lst, deg_lst, eps=0.6, min_pts=3)
    pscan_algo.run_algorithm()
