import math
import matplotlib.pyplot as plt
import os

server_scalability_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/scalability_simd_paper2'
u = 5
eps = 0.2
thread_lst = [2 ** i for i in xrange(9)]
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
    file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                             '-'.join(['output', dataset, str(eps), str(min_pts), str(thread_num)]) + '.txt'])
    with open(file_path) as ifs:
        lines = ifs.readlines()
        my_lst_lst = map(lambda tag: filter_time_lst(tag, lines), tag_lst)
        min_idx = find_min(my_lst_lst[-1])
        min_lst = map(lambda lst: lst[min_idx], my_lst_lst)
        assert sum(min_lst[:-1]) < min_lst[-1]
        # 1st, 2nd, 3rd, 4th, total
        breakdown_time_lst = [min_lst[0], min_lst[1] + min_lst[2], min_lst[3], min_lst[4], min_lst[5]]
        # print breakdown_time_lst

        # print my_lst_lst
        for i in xrange(len(tag_lst)):
            # print map(len, my_lst_lst)
            assert len(my_lst_lst[0]) == len(my_lst_lst[i])
        return map(lambda integer: float(integer) / 1000, breakdown_time_lst)


if __name__ == '__main__':
    ppscan_runtime_tag = 'ppSCAN Runtime (s)'
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
    time_lst_lst_arr = []
    for data_set in data_set_lst:
        per_thread_breakdown_time_lst = []
        for thread_num in thread_lst:
            per_thread_breakdown_time_lst.append(
                get_ppscan_breakdown_runtime(data_set, eps, u, thread_num, server_scalability_folder))
        time_lst_lst = map(lambda idx:
                           map(lambda lst: lst[idx], per_thread_breakdown_time_lst),
                           range(len(per_thread_breakdown_time_lst[0])))
        print time_lst_lst
        time_lst_lst_arr.append(time_lst_lst)

    exp_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 2.5))

    log_thread_lst = map(lambda ele: int(math.log(ele, 2)), thread_lst)

    for ax_idx, ax in enumerate(ax_tuple):
        ppscan_runtime_lst_lst = time_lst_lst_arr[ax_idx]
        shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']

        for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
            ax.plot(log_thread_lst, runtime_lst, shape_lst[idx], markersize=10, markerfacecolor='none')

        ax.set_yscale('log')
        plt.xticks(log_thread_lst, [1, 2, 4, 8, 16, 32, 64, 128, 256])

        print ax_idx
        ylim_lst = [(10 ** (-1), 10 ** 3), (1, 10 ** 3 * 2), (10 ** (-1), 10 ** 4 * 2), (1, 10 ** 4 * 2)]
        ax.set_ylim(ylim_lst[ax_idx])

        ytick_lst = [[10 ** i for i in [-1, 0, 1, 2, 3]],
                     [10 ** i for i in [0, 1, 2, 3]],
                     [10 ** i for i in [-1, 0, 1, 2, 3, 4]],
                     [10 ** i for i in [0, 1, 2, 3, 4]],
                     ]
        ax.set_yticks(ytick_lst[ax_idx])
        # ax.set_ylim(float(min(map(min, ppscan_runtime_lst_lst))) / factor_lst[ax_idx],
        #             max(map(max, ppscan_runtime_lst_lst)) * mul_factor[ax_idx])

    for idx, my_ax in enumerate(ax_tuple):
        if idx == 0:
            my_ax.set_ylabel('Runtime (seconds)', fontsize=12)
        my_ax.set_xlabel('Number of threads' + '\n' + sub_titles[idx], fontsize=12)
        my_ax.grid(True)

    exp_figure.subplots_adjust(wspace=0)
    plt.tight_layout()

    plt.subplots_adjust(top=0.85)
    plt.legend(legend_lst, ncol=len(legend_lst), prop={'size': 12, "weight": "bold"},
               loc=2, bbox_to_anchor=(-3.55, 1.32, 4.5, 0.0), mode='expand')
    plt.savefig('scalability_varying_threads_knl.' + 'pdf', dpi=1200)
    # exp_figure.show()
    plt.close()
