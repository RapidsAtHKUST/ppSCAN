if __name__ == '__main__':
    lst = [25121224, 715258284, 1810511910, 154796461]
    times = lst[0]
    less_time = lst[1]
    greater_time = lst[2]
    equal_time = lst[3]
    print 'avg:', sum(lst[1:]) / float(times)
    print 'portion:', map(lambda ele: float(ele) / sum(lst[1:]), lst[1:])

    with open('distribution.txt') as ifs:
        cmp_distribution = eval(ifs.readline())
        print len(cmp_distribution)

        small_lst = cmp_distribution[0:6]
        middle_lst = cmp_distribution[6:10]
        big_lst = cmp_distribution[10:]

        histogram_list = [sum(small_lst), sum(middle_lst), sum(big_lst)]
        print histogram_list
        portion_list = map(lambda ele: ele / float(sum(histogram_list)), histogram_list)
        print portion_list
