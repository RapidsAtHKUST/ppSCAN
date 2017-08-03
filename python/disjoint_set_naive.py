class LinkedListNode:
    def __init__(self, val, list_head_ptr):
        self.value_ = val
        self.next_ = None
        self.list_head_ = list_head_ptr


class LinkedList:
    def __init__(self, x):
        self.head_node_ = LinkedListNode(x, self)
        self.tail_node = self.head_node_
        self.node_size = 1

    # heuristic: weighted union, i.e, choose biggest as the head
    @staticmethod
    def link(left_list, right_list):
        """
        :type left_list: LinkedList
        :type right_list: LinkedList
        """

        def get_sorted_pair():
            if left_list.node_size > right_list.node_size:
                return left_list, right_list
            else:
                return right_list, left_list

        def concat_two_lists():
            # update next pointer
            cur_node_ref = small_list.head_node_
            large_list.tail_node.next_ = small_list.head_node_

            while cur_node_ref is not None:
                # update first pointer
                cur_node_ref.list_head_ = large_list
                # update link
                cur_node_ref = cur_node_ref.next_

            # update last pointer
            large_list.tail_node = small_list.tail_node

        large_list, small_list = get_sorted_pair()
        large_list.node_size += small_list.node_size
        concat_two_lists()
        return large_list

    def to_sorted_list(self):
        val_list = []
        iter_node = self.head_node_
        while iter_node is not None:
            val_list.append(iter_node.value_)
            iter_node = iter_node.next_
        return sorted(val_list)

    def __str__(self):
        return str(self.to_sorted_list())


# attention: users need to keep the disjoint property
# self.node_dict: keep the mapping from ele to node
class NaiveDisjointSet:
    def __init__(self):
        self.set_list = []
        self.node_dict = {}

    def make_set(self, x):
        new_list = LinkedList(x)
        self.set_list.append(new_list)
        self.node_dict[x] = new_list.head_node_

    def union(self, x, y):
        x_list = self.node_dict[x].list_head_
        y_list = self.node_dict[y].list_head_
        union_list = LinkedList.link(x_list, y_list)
        if union_list is not x_list:
            self.set_list.remove(x_list)

        else:
            self.set_list.remove(y_list)

    def find_set(self, x):
        """
        :rtype: LinkedList
        """
        return self.node_dict[x].list_head_ if x in self.node_dict else None

    def __str__(self):
        str_list = map(lambda ele: ele.to_sorted_list(), self.set_list)
        return str(str_list)


def connected_component(vertex_collection, edge_list):
    naive_disjoint_set = NaiveDisjointSet()
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
