class LinkedListNode:
    def __init__(self, val):
        self.value_ = val
        self.first_ = self
        self.last_ = self
        self.next_ = None

    def append(self, another_list_head):
        # update next pointer
        cur_node_ref = another_list_head
        self.last_.next_ = another_list_head

        # update first pointer
        while cur_node_ref is not None:
            cur_node_ref.first_ = self

            # update last pointer
            if cur_node_ref.next_ is None:
                self.last_ = cur_node_ref

            cur_node_ref = cur_node_ref.next_


class LinkedList:
    def __init__(self, x):
        self.head_node = LinkedListNode(x)
        self.node_size = 1

    # heuristic: weighted union, i.e, choose biggest as the head
    def union(self, another_list):
        """
        :type another_list: LinkedList
        """

        def get_sorted_pair():
            if self.node_size > another_list.node_size:
                return self, another_list
            else:
                return another_list, self

        large_list, small_list = get_sorted_pair()
        large_list.node_size += small_list.node_size
        large_list.head_node.append(small_list.head_node)
        return large_list

    def is_in_set(self, x):
        iter_node = self.head_node
        while iter_node is not None:
            if iter_node.value_ == x:
                return True
            iter_node = iter_node.next_
        return False

    def to_sorted_list(self):
        val_list = []
        iter_node = self.head_node
        while iter_node is not None:
            val_list.append(iter_node.value_)
            iter_node = iter_node.next_
        return sorted(val_list)

    def __str__(self):
        return str(self.to_sorted_list())


# attention: users need to keep the disjoint property
class NaiveDisjointSet:
    def __init__(self):
        self.set_list = []

    def make_set(self, x):
        self.set_list.append(LinkedList(x))

    def union(self, x, y):
        x_list = self.find_set(x)
        y_list = self.find_set(y)
        union_list = x_list.union(y_list)
        if union_list is not x_list:
            self.set_list.remove(x_list)
        else:
            self.set_list.remove(y_list)

    def find_set(self, x):
        """
        :rtype: LinkedList
        """
        for element in self.set_list:
            assert isinstance(element, LinkedList)
            if element.is_in_set(x):
                return element
        # not found in any
        return None

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
    print connected_component(vertex_set, edge_list)
