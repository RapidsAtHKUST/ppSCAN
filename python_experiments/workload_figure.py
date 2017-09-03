import os, re
from functools import partial

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
    return map(lambda my_str: eval(my_str.split(':')[-1].replace('ms', '')),
               filter(lambda line: tag in line, lines))


def get_workload_statistics(dataset, eps, min_pts, root_dir_path='.'):
    file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                             '-'.join(['output', dataset, str(eps), str(min_pts)]) + '.txt'])
    with open(file_path) as ifs:
        lines = ifs.readlines()

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


def case_study_fix_eps_min_pts(data_set_lst, eps, min_pts, root_dir_path='.'):
    for data_set in data_set_lst:
        ret_dict = get_workload_statistics(data_set, eps, min_pts, root_dir_path)
        print ret_dict


# tags for figure drawing, legends
display_pscan_runtime_tag = 'serial pscan runtime'
display_pscan_plus_runtime_tag = 'serial pscan+ runtime'

display_pscan_eval_tag = 'pscan eval number'
display_pscan_plus_eval_tag = 'pscan+ eval number'
display_prune0_tag = prune0_tag
display_prune1_tag = prune1_tag


def min_runtime(statistics_dict_lst):
    pscan_runtime_lst = map(lambda statistics_dict: statistics_dict[runtime_tag + pscan_tag], statistics_dict_lst)
    pscan_plus_runtime_lst = map(lambda statistics_dict: statistics_dict[runtime_tag + pscan_plus_tag],
                                 statistics_dict_lst)
    return min(min(pscan_plus_runtime_lst, pscan_runtime_lst))


def to_display_dict(statistics_dict, min_runtime_val):
    keys = [display_prune0_tag, display_prune1_tag, display_pscan_eval_tag, display_pscan_plus_eval_tag]
    values = map(lambda value: float(value * 2) / statistics_dict[edge_tag],
                 [statistics_dict[prune0_tag], statistics_dict[prune1_tag],
                  statistics_dict[intersect_tag + pscan_tag], statistics_dict[intersect_tag + pscan_plus_tag]])
    display_dict = dict(zip(keys, values))

    display_dict[display_pscan_runtime_tag] = float(statistics_dict[runtime_tag + pscan_tag]) / min_runtime_val
    display_dict[display_pscan_plus_runtime_tag] = float(
        statistics_dict[runtime_tag + pscan_plus_tag]) / min_runtime_val
    return display_dict


def display_workload_runtime(eps_lst, display_lst, title_append_txt=''):
    tag_list = [display_pscan_plus_eval_tag, display_pscan_eval_tag,
                display_prune0_tag, display_prune1_tag]

    # draw after get data, with partial binding technique
    shape_color_lst = ['g^-', 'rs-', 'c<-', 'y>-', 'mx-', 'k--']
    prev_partial_func = plt.plot
    cur_shape_color_idx = 0

    result_lst = map(lambda tag:
                     map(lambda display: display[tag], display_lst), tag_list)

    for portion_lst in result_lst:
        # partially bind parameters
        prev_partial_func = partial(prev_partial_func, eps_lst, portion_lst,
                                    shape_color_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1
    prev_partial_func()

    plt.legend(tag_list)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Workload portion\n' + title_append_txt
              if title_append_txt != '' else 'Workload portion', fontdict=font)
    plt.xlabel('eps', fontdict=font)
    plt.ylabel('portion', fontdict=font)
    plt.ylim([-0.1, 1.1])
    os.system('mkdir -p ./figures/workload')
    plt.savefig('./figures/workload' + os.sep + title_append_txt.replace(' ', '') + '-' + 'workload.png',
                bbox_inches='tight', pad_inches=0, transparent=True)
    # plt.show()
    plt.close()


def display_runtime(eps_lst, display_lst, title_append_txt=''):
    tag_list = [display_pscan_plus_runtime_tag, display_pscan_runtime_tag]

    # draw after get data, with partial binding technique
    shape_color_lst = ['mx-', 'k--']
    prev_partial_func = plt.plot
    cur_shape_color_idx = 0

    result_lst = map(lambda tag:
                     map(lambda display: display[tag], display_lst), tag_list)

    for portion_lst in result_lst:
        # partially bind parameters
        prev_partial_func = partial(prev_partial_func, eps_lst, portion_lst,
                                    shape_color_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1
    prev_partial_func()

    plt.legend(tag_list)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Runtime\n' + title_append_txt
              if title_append_txt != '' else 'Runtime', fontdict=font)
    plt.xlabel('eps', fontdict=font)
    plt.ylabel('time (s)', fontdict=font)

    os.system('mkdir -p ./figures/workload')
    plt.savefig('./figures/workload' + os.sep + title_append_txt.replace(' ', '') + '-' + 'runtime.png',
                bbox_inches='tight', pad_inches=0, transparent=True)
    # plt.show()
    plt.close()


def case_study0():
    # case study 0
    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster',
                    '10million_avgdeg15_maxdeg50_Cdefault']
    eps = 0.3
    min_pts = 5
    root_dir_path = '/mnt/mount-gpu/d2/yche/projects/python_experiments/worklaod'
    case_study_fix_eps_min_pts(data_set_lst, eps, min_pts, root_dir_path)
    print


def case_study1():
    # case study 1
    min_pts = 5
    root_dir_path = '/mnt/mount-gpu/d2/yche/projects/python_experiments/worklaod'

    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster',
                    '10million_avgdeg15_maxdeg50_Cdefault']
    parameter_eps_lst = [float(i + 1) / 10 for i in xrange(9)]
    for data_set in data_set_lst:
        statistics_lst = map(lambda eps: get_workload_statistics(data_set, eps, min_pts, root_dir_path),
                             parameter_eps_lst)
        display_lst = map(lambda statistics_dict: to_display_dict(statistics_dict, 1000), statistics_lst)
        append_txt = ' - '.join([data_set, 'min_pts:' + str(min_pts)])
        display_workload_runtime(parameter_eps_lst, display_lst, title_append_txt=append_txt)
        display_runtime(parameter_eps_lst, display_lst, title_append_txt=append_txt)


if __name__ == '__main__':
    case_study0()
    case_study1()
