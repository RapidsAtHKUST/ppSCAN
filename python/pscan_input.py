from BinaryStream import BinaryStream

import numpy as np
import networkx as nx


# (1,2) (2,1) count twice in num_edges statistics
def get_v_e_deg():
    with open('/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/data/b_degree.bin') as ifs:
        stream = BinaryStream(ifs)
        int_size = stream.readInt32()
        assert int_size == 4
        num_vertices = stream.readInt32()
        num_edges = stream.readInt32()
        deg_arr = np.empty((num_vertices,), dtype=int)
        print 'deg arr shape:', deg_arr.shape

        for i in xrange(num_vertices):
            deg_arr[i] = stream.readInt32()
            assert deg_arr[i] > 0

        assert deg_arr.sum() == num_edges
    return num_vertices, num_edges, deg_arr


def get_spare_adj_matrix():
    with open('/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/data/b_adj.bin') as ifs:
        stream = BinaryStream(ifs)
        adj_arr = np.empty((n,), dtype=list)
        for i in xrange(n):
            lst = []
            for j in xrange(deg_arr[i]):
                lst.append(stream.readInt32())
            adj_arr[i] = lst
    return adj_arr


if __name__ == '__main__':
    n, m, deg_arr = get_v_e_deg()
    print 'v_num:', n, 'e_num:', m / 2, 'avg deg:', m / 2.0 / n
    print get_spare_adj_matrix()
