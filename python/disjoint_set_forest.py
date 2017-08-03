class TreeNode:
    def __init__(self, val):
        self.value_ = val
        self.parent_ = self
        self.rank_ = 0
        # only useful for the root for elements traversal
        self.children = [self]

    def append(self, another_root):
        """
        :type another_root: TreeNode
        """
        another_root.parent_ = self
        # since tree height increment by one
        if self.rank_ == another_root.rank_:
            self.rank_ += 1

        # update children
        self.children.extend(another_root.children)
        another_root.children = None

    @staticmethod
    def link(node_x, node_y):
        """
        :type node_x: TreeNode
        :type node_y: TreeNode
        """
        if node_x.rank_ > node_y.rank_:
            node_x.append(node_y)
            return node_x
        else:
            node_y.append(node_x)
            return node_y


# attention: users need to keep the disjoint property
# self.node_dict: keep the mapping from ele to node
class ForestDisjointSet:
    def __init__(self):
        self.set_list_ = set()
        self.node_dict_ = {}

    def make_set(self, x):
        new_node = TreeNode(x)
        self.set_list_.add(new_node)
        self.node_dict_[x] = new_node

    def union(self, x, y):
        node_x = self.find_set(x)
        node_y = self.find_set(y)
        # heuristic: union by rank
        union_node = TreeNode.link(node_x, node_y)
        if union_node is node_y:
            self.set_list_.remove(node_x)
        else:
            self.set_list_.remove(node_y)

    def find_set_detail(self, node_x):
        # bottom condition
        if node_x.parent_ == node_x:
            return node_x.parent_
        else:
            # heuristic: path compression
            # two pass method: 1) recursively find the root, 2) update parent in the path to root
            node_x.parent_ = self.find_set_detail(node_x.parent_)
            return node_x.parent_

    def find_set(self, x):
        """
        :rtype: TreeNode
        """
        node_x = self.node_dict_[x]
        assert isinstance(node_x, TreeNode)
        return self.find_set_detail(node_x)

    def __str__(self):
        return str(map(lambda my_set: sorted(map(lambda node: node.value_, my_set.children)), self.set_list_))


def connected_component(vertex_collection, edge_list):
    naive_disjoint_set = ForestDisjointSet()
    for vertex in vertex_collection:
        naive_disjoint_set.make_set(vertex)
    for src, dst in edge_list:
        if naive_disjoint_set.find_set(src) != naive_disjoint_set.find_set(dst):
            naive_disjoint_set.union(src, dst)
    return naive_disjoint_set


if __name__ == '__main__':
    edge_list = [(1, 2), (2, 3), (4, 5), (4, 7), (5, 7)]
    vertex_set = set()
    for src, dst in edge_list:
        vertex_set.add(src)
        vertex_set.add(dst)
    vertex_set.add(9)

    # find connected component
    print 'vertices:', vertex_set
    print 'edges:', edge_list
    print 'connected component:', connected_component(vertex_set, edge_list)
