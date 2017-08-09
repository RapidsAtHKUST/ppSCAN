if __name__ == '__main__':
    lst = [25121224, 715258284, 1810511910, 154796461]
    times = lst[0]
    less_time = lst[1]
    greater_time = lst[2]
    equal_time = lst[3]
    print 'avg:', sum(lst[1:]) / float(times)
    print 'portion:', map(lambda ele: float(ele) / sum(lst[1:]), lst[1:])
