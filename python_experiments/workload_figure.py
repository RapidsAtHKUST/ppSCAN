import os, sys, re
import numpy as np
import matplotlib.pyplot as plt

# 1st: same
edge_tag = 'num of edges'
prune0_tag = 'prune0 definitely not reachable'
prune1_tag = 'prune1 definitely reachable'

# 2nd: different
intersect_tag = 'intersection times'
cmp0_tag = 'cmp0'
cmp1_tag = 'cmp1'
cmp_equ_tag = 'equal cmp'
runtime_tag = 'Total time without IO'

# suffix
pscan_tag = ', pscan'
pscan_plus_tag = ', pscan+'


def filter_lines_by_tag(tag, lines):
    return map(lambda my_str: eval(my_str.split(':')[-1]),
               filter(lambda line: tag in line, lines))


def get_statistics(lines):
    tag_lst = [prune0_tag, prune1_tag, intersect_tag, cmp0_tag, cmp1_tag, cmp_equ_tag]
    statistics_lst_dict = dict(map(lambda tag: (tag, filter_lines_by_tag(tag, lines)), tag_lst))
    # runtime unit: ms
    runtime_lst = map(lambda time_str: eval(time_str.split('ms')[0]) if 'ms' in time_str else eval(time_str) / 1000,
                      map(lambda my_str: my_str.split(':')[-1], filter(lambda line: runtime_tag in line, lines)))

    edge_num = eval(filter(lambda line: re.match('.*n:.*m:.*', line), lines)[0].split('m:')[-1])

    # 1st: same
    ret_dict = {edge_tag: edge_num, prune0_tag: statistics_lst_dict[prune0_tag][-1],
                prune1_tag: statistics_lst_dict[prune1_tag][-1]}

    # 2nd: different
    eval_comp_tag_lst = [intersect_tag, cmp0_tag, cmp1_tag, cmp_equ_tag]
    for tag in eval_comp_tag_lst:
        ret_dict[tag + pscan_tag] = statistics_lst_dict[tag][-1]
        ret_dict[tag + pscan_plus_tag] = statistics_lst_dict[tag][-2]
    ret_dict[runtime_tag + pscan_tag] = runtime_lst[-1]
    ret_dict[runtime_tag + pscan_plus_tag] = runtime_lst[-2]

    return ret_dict


if __name__ == '__main__':
    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster',
                    '10million_avgdeg15_maxdeg50_Cdefault']
    eps = 0.3
    min_pts = 5
    root_dir_path = '/mnt/mount-gpu/d2/yche/projects/python_experiments/worklaod2'
    for data_set in data_set_lst:
        file_path = os.sep.join([root_dir_path, data_set, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                                 '-'.join(['output', data_set, str(eps), str(min_pts)]) + '.txt'])
        with open(file_path) as ifs:
            lines = ifs.readlines()
            ret_dict = get_statistics(lines)
            print ret_dict
