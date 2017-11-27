from cmp_figure import *


def get_speedup_lst(lst_big, lst_small):
    return map(lambda pair: float(pair[0]) / pair[1], zip(lst_big, lst_small))


if __name__ == '__main__':
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    any_scan_data_set_lst = ['snap_orkut', 'webgraph_twitter']
    tag = gpu23_tag


    def cmp_with_pscan():
        for data_set in data_set_lst:
            ppscan_lst = get_time_lst('ppscan', tag, data_set)
            pscan_lst = get_time_lst('pscan', tag, data_set)
            print get_speedup_lst(pscan_lst, ppscan_lst)


    def cmp_with_anyscan():
        for data_set in any_scan_data_set_lst:
            ppscan_lst = get_time_lst('ppscan', tag, data_set)
            anyscan_lst = get_time_lst('anyscan', tag, data_set)
            print get_speedup_lst(anyscan_lst, ppscan_lst)


    def cmp_with_scanxp():
        for data_set in data_set_lst:
            ppscan_lst = get_time_lst('ppscan', tag, data_set)
            anyscan_lst = get_time_lst('scan_xp', tag, data_set)
            print get_speedup_lst(anyscan_lst, ppscan_lst)


    # with pscan
    print 'pscan'
    cmp_with_pscan()
    tag = knl_tag
    print
    cmp_with_pscan()
    print

    # with anyscan
    print 'anyscan'
    tag = gpu23_tag
    cmp_with_anyscan()
    tag = knl_tag
    print
    cmp_with_anyscan()

    # with scanxp
    print 'scan_xp'
    tag = gpu23_tag
    cmp_with_scanxp()
    tag = knl_tag
    print
    cmp_with_scanxp()
