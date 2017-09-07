# return -1 if not found, else return idx
def binary_search(array, beg, end, val):
    if beg == end:
        return -1
    mid = (beg + end) / 2
    if array[mid] == val:
        return mid
    return binary_search(array, beg, mid, val) if val < array[mid] else binary_search(array, mid + 1, end, val)


# return end if not found, return else the first one >= val
def binary_search2(array, beg, end, val):
    assert 0 <= beg < end <= len(array)

    if array[end - 1] < val:
        return end

    mid = (beg + end) / 2
    if array[mid] == val:
        return mid

    if array[mid] < val:
        return mid if mid + 1 == end else binary_search2(array, mid + 1, end, val)

    if mid == beg:
        return beg
    return binary_search2(array, beg, mid, val)


def binary_search3(array, beg, end, val):
    assert 0 <= beg < end <= len(array)
    assert array[end - 1] > val

    mid = (beg + end) / 2
    if array[mid] == val:
        return mid

    if array[mid] < val:
        return mid if mid + 1 == end else binary_search3(array, mid + 1, end, val)

    if mid == beg:
        return beg
    return binary_search3(array, beg, mid, val)


# return end if not found, return else the first one >= val
def galloping_search(array, beg, end, val):
    assert 0 <= beg < end <= len(array)
    if array[end - 1] < val:
        return end

    # galloping advance
    if array[beg] >= val:
        return beg
    if array[beg + 1] >= val:
        return beg + 1
    if array[beg + 2] >= val:
        return beg + 2

    jump_idx = 4
    is_working = True
    while is_working:
        peek_idx = beg + jump_idx
        if peek_idx >= end:
            peek_idx = end - 1
            is_working = False

        if array[peek_idx] == val:
            return peek_idx
        if array[peek_idx] > val:
            return binary_search3(array, jump_idx / 2 + beg + 1, peek_idx + 1, val)
        jump_idx *= 2


if __name__ == '__main__':
    lst1 = sorted(set([1, 2, 4, 5, 11, 12, 18, 20, 31, 33, 40, 55] + range(60, 90)))
    lst1.remove(74)
    lst2 = range(30)

    test_val_lst = [20, 18, 40, 19, 41, -1, 7, 70, 73, 74]


    def pretty_print():
        # print test_val_lst
        print map(lambda idx: lst1[idx] if idx < len(lst1) else None, idx_lst)


    idx_lst = map(lambda val: binary_search(lst1, 0, len(lst1), val), test_val_lst)
    pretty_print()

    idx_lst = map(lambda val: binary_search2(lst1, 0, len(lst1), val), test_val_lst)
    pretty_print()

    idx_lst = map(lambda val: binary_search2(lst1, 0, len(lst1), val), test_val_lst)
    pretty_print()
