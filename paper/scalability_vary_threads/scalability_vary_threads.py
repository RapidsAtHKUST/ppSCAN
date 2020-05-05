import math
import matplotlib.pyplot as plt
import os

LABEL_SIZE = 24
TICK_SIZE = 24
LEGEND_SIZE = 24
MARK_SIZE = 18

server_scalability_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/scalability_simd_paper2'
u = 5
eps = 0.2
thread_lst = [2 ** i for i in range(9)]
tag_lst = ['prune execution time', 'check core first-phase bsp time', 'check core second-phase bsp time',
           '3rd: core clustering time', 'non-core clustering time', 'Total time without IO']
legend_lst = ['1. Similarity Pruning', '2. Core Checking and Consolidating', '3. Core Clustering',
              '4. Non-Core Clustering', 'The Whole ppSCAN']


def filter_time_lst(runtime_tag, lines):
    runtime_lst = map(lambda time_str: eval(time_str.split('ms')[0]) if 'ms' in time_str else eval(time_str) / 1000,
                      map(lambda my_str: my_str.split(':')[-1], filter(lambda line: runtime_tag in line, lines)))
    return runtime_lst


def find_min(lst):
    min_val = 999999999
    min_idx = -1
    for idx, value in enumerate(lst):
        if value < min_val:
            min_val = value
            min_idx = idx
    return min_idx


def get_ppscan_breakdown_runtime(dataset, eps, min_pts, thread_num, root_dir_path='.'):
    # file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
    #                          '-'.join(['output', dataset, str(eps), str(min_pts), str(thread_num)]) + '.txt'])
    # with open(file_path) as ifs:
    #     lines = ifs.readlines()
    #     my_lst_lst = map(lambda tag: filter_time_lst(tag, lines), tag_lst)
    #     min_idx = find_min(my_lst_lst[-1])
    #     min_lst = map(lambda lst: lst[min_idx], my_lst_lst)
    #     assert sum(min_lst[:-1]) < min_lst[-1]
    # 1st, 2nd, 3rd, 4th, total
    # breakdown_time_lst = [min_lst[0], min_lst[1] + min_lst[2], min_lst[3], min_lst[4], min_lst[5]]
    # print breakdown_time_lst

    # print my_lst_lst
    # for i in range(len(tag_lst)):
    # print map(len, my_lst_lst)
    # assert len(my_lst_lst[0]) == len(my_lst_lst[i])
    # return map(lambda integer: float(integer) / 1000, breakdown_time_lst)
    with open('case_study_15/{}.md'.format(dataset)) as ifs:
        def merge_lst(lst):
            return [lst[0], lst[1] + lst[2], lst[3], lst[4], lst[5]]

        def parse_line(line):
            t = [eval(e.replace('s', '')) for e in line.split(' | ')[0:7]]
            return (t[0], merge_lst(t[1:]))

        lines = ifs.readlines()[2:]
        my_dict = dict([(parse_line(l)) for l in lines])

    return my_dict[thread_num]


if __name__ == '__main__':
    # print(get_ppscan_breakdown_runtime(dataset='snap_orkut', eps=0.2, min_pts=5, thread_num=256))
    ppscan_runtime_tag = 'ppSCAN Runtime (s)'
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
    time_lst_lst_arr = []
    for data_set in data_set_lst:
        per_thread_breakdown_time_lst = []
        for thread_num in thread_lst:
            per_thread_breakdown_time_lst.append(
                get_ppscan_breakdown_runtime(data_set, eps, u, thread_num, server_scalability_folder))
        time_lst_lst = list(map(lambda idx:
                                list(map(lambda lst: lst[idx], per_thread_breakdown_time_lst)),
                                range(len(per_thread_breakdown_time_lst[0]))))
        time_lst_lst_arr.append(time_lst_lst)

    exp_figure, ax_tuple = plt.subplots(2, 2, figsize=(16, 8))
    ax_tuple = ax_tuple.flatten()
    log_thread_lst = list(map(lambda ele: int(math.log(ele, 2)), thread_lst))

    for ax_idx, ax in enumerate(ax_tuple):
        ppscan_runtime_lst_lst = time_lst_lst_arr[ax_idx]
        shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']

        for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
            ax.plot(log_thread_lst, runtime_lst, shape_lst[idx], markersize=MARK_SIZE, markerfacecolor='none')

        ax.set_yscale('log')
        plt.xticks(log_thread_lst, [1, 2, 4, 8, 16, 32, 64, 128, 256])

        ylim_lst = [(10 ** (-1), 10 ** 3), (1, 10 ** 3 * 2), (10 ** (-1), 10 ** 4 * 2), (1, 10 ** 4 * 2)]
        ax.set_ylim(ylim_lst[ax_idx])

        ytick_lst = [[10 ** i for i in [-1, 0, 1, 2, 3]],
                     [10 ** i for i in [0, 1, 2, 3]],
                     [10 ** i for i in [-1, 0, 1, 2, 3, 4]],
                     [10 ** i for i in [0, 1, 2, 3, 4]],
                     ]
        ax.set_yticks(ytick_lst[ax_idx])
        ax.tick_params(labelsize=LABEL_SIZE)
        # ax.set_ylim(float(min(map(min, ppscan_runtime_lst_lst))) / factor_lst[ax_idx],
        #             max(map(max, ppscan_runtime_lst_lst)) * mul_factor[ax_idx])

    for idx, my_ax in enumerate(ax_tuple):
        # if idx == 0:
        my_ax.set_ylabel('Runtime (seconds)', fontsize=TICK_SIZE)
        my_ax.set_xlabel('Number of threads' + '\n' + sub_titles[idx], fontsize=TICK_SIZE)
        my_ax.grid(True)

    exp_figure.subplots_adjust(wspace=0)
    plt.tight_layout()

    plt.subplots_adjust(top=0.85)
    exp_figure.legend(legend_lst, ncol=3, prop={'size': LEGEND_SIZE - 4, "weight": "bold"},
                      loc="upper left", bbox_to_anchor=(0., 0.92, 1., .102), mode='expand')
    plt.savefig('scalability_varying_threads_knl.' + 'pdf', dpi=300)
    plt.close()
