def statistics_intersection(lst):
    times = lst[0]
    print 'avg cmp times:', sum(lst[1:]) / float(times)
    print 'cmp(<,>,=) portion:', map(lambda ele: float(ele) / sum(lst[1:]), lst[1:]), '\n'


def statistics_histogram():
    with open('distribution.txt') as ifs:
        cmp_distribution = eval(ifs.readline())

        small_lst = cmp_distribution[0:6]
        middle_lst = cmp_distribution[6:10]
        big_lst = cmp_distribution[10:]

        histogram_list = [sum(small_lst), sum(middle_lst), sum(big_lst)]
        print 'cmp histogram:', histogram_list
        portion_list = map(lambda ele: ele / float(sum(histogram_list)), histogram_list)
        print 'cmp histogram(0:6, 6:10, 10:inf):', portion_list, '\n'


def statistics_pruning():
    lst = [3369420, 1001839, 25121224]
    portion_lst = map(lambda ele: float(ele) / sum(lst), lst)
    print '\nprune0, prune1, evaluations:', portion_lst
    edge_num = 69362378
    print 'involved:', sum(lst) * 2, 'total:', edge_num, 'ratio:', float(sum(lst) * 2) / edge_num


if __name__ == '__main__':
    with_early_exit_lst = [25121224, 715258284, 1810511910, 154796461]
    without_early_exit_lst = [25121224, 2776231078, 1640997995, 202595282]
    without_early_stop_check_core_list = [26355704, 1827106259, 771060399, 167979628]
    statistics_intersection(with_early_exit_lst)
    statistics_intersection(without_early_stop_check_core_list)
    print 'cmp count(to without early exit):', sum(without_early_exit_lst[1:]), sum(
        with_early_exit_lst[1:]), 'ratio:', sum(
        with_early_exit_lst[1:]) / float(sum(without_early_exit_lst[1:]))

    print 'cmp count(to without early stop check core):', sum(with_early_exit_lst[1:]) / float(
        sum(without_early_stop_check_core_list[1:])), '\n'

    statistics_histogram()

    statistics_pruning()
