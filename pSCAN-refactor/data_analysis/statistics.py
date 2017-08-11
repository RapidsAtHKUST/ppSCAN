def statistics_intersection():
    lst = [25121224, 715258284, 1810511910, 154796461]
    times = lst[0]
    print 'avg cmp times:', sum(lst[1:]) / float(times)
    print 'cmp(<,>,=) portion:', map(lambda ele: float(ele) / sum(lst[1:]), lst[1:])

    with open('distribution.txt') as ifs:
        cmp_distribution = eval(ifs.readline())

        small_lst = cmp_distribution[0:6]
        middle_lst = cmp_distribution[6:10]
        big_lst = cmp_distribution[10:]

        histogram_list = [sum(small_lst), sum(middle_lst), sum(big_lst)]
        print 'cmp histogram:', histogram_list
        portion_list = map(lambda ele: ele / float(sum(histogram_list)), histogram_list)
        print 'cmp histogram(0:6, 6:10, 10:inf):', portion_list


def statistics_pruning():
    lst = [3369420, 1001839, 25121224]
    portion_lst = map(lambda ele: float(ele) / sum(lst), lst)
    print '\nprune0, prune1, evaluations:', portion_lst
    print 'involved:', sum(lst) * 2, 'total:', 69362378


if __name__ == '__main__':
    statistics_intersection()
    statistics_pruning()
