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
        while cur_node_ref.next_ is not None:
            cur_node_ref.first_ = self
            cur_node_ref = cur_node_ref.next_

        # update last pointer
        self.last_ = cur_node_ref


class LinkedList:
    def __init__(self, x):
        self.head_node = LinkedListNode(x)
        self.node_size = 1

    def union(self, another_list):
        # choose biggest as the head
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
        while iter_node.next_ is not None:
            if iter_node.value_ == x:
                return True
        return False


class NaiveDisjointSet:
    def __init__(self):
        self.set_list = []

    # users need to keep the disjoint property
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
