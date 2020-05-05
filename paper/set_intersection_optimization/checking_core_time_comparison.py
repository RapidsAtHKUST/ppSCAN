import os
import matplotlib.pyplot as plt

LABEL_SIZE = 24
TICK_SIZE = 24
LEGEND_SIZE = 24
MARK_SIZE = 18

ppscanno_knl_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/ppSCANNO_knl'
ppscan_knl_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/scalability_simd_paper2'
thread_num_knl = 256

ppscanno_gpu23_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/ppSCANNO_gpu23'
ppscan_gpu23_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/scalability_simd_paper_gpu23'
thread_num_gpu23 = 64

gpu23_tag = 'gpu23'
knl_tag = 'knl'

u = 5
eps_lst = [float(i + 1) / 10 for i in range(9)]
data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']

tag_lst = ['prune execution time', 'check core first-phase bsp time', 'check core second-phase bsp time',
           '3rd: core clustering time', 'non-core clustering time', 'Total time without IO']
legend_lst = ['1. prune', '2. check core', '3. cluster core', '4. cluster non-core', 'in total']


# def filter_time_lst(runtime_tag, lines):
#     runtime_lst = map(lambda time_str: eval(time_str.split('ms')[0]) if 'ms' in time_str else eval(time_str) / 1000,
#                       map(lambda my_str: my_str.split(':')[-1], filter(lambda line: runtime_tag in line, lines)))
#     return runtime_lst


def find_min(lst):
    min_val = 999999999
    min_idx = -1
    for idx, value in enumerate(lst):
        if value < min_val:
            min_val = value
            min_idx = idx
    return min_idx


# def get_algorithm_checking_core_runtime(dataset, eps, min_pts, thread_num, root_dir_path):
#     file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
#                              '-'.join(['output', dataset, str(eps), str(min_pts), str(thread_num)]) + '.txt'])
#
#     with open(file_path) as ifs:
#         lines = ifs.readlines()
#         my_lst_lst = map(lambda tag: filter_time_lst(tag, lines), tag_lst)
#         min_idx = find_min(my_lst_lst[-1])
#         min_lst = map(lambda lst: lst[min_idx], my_lst_lst)
#         assert sum(min_lst[:-1]) < min_lst[-1]
#         # 1st, 2nd, 3rd, 4th, total
#         breakdown_time_lst = [min_lst[0], min_lst[1] + min_lst[2], min_lst[3], min_lst[4], min_lst[5]]
#
#         for i in range(len(tag_lst)):
#             assert len(my_lst_lst[0]) == len(my_lst_lst[i])
#         return breakdown_time_lst[1]


# def display_comparison_txt():
#     print
#     '====================On KNL(AVX512)=========================='
#     for data_set in data_set_lst:
#         for eps in eps_lst:
#             time_no_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_knl, ppscanno_knl_folder)
#             time_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_knl, ppscan_knl_folder)
#             print
#             time_no_opt, time_opt, float(time_no_opt) / time_opt
#         print
#
#     print
#     '====================On GPU23(AVX2)=========================='
#     for data_set in data_set_lst:
#         for eps in eps_lst:
#             time_no_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_gpu23, ppscanno_gpu23_folder)
#             time_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_gpu23, ppscan_gpu23_folder)
#             print
#             time_no_opt, time_opt, float(time_no_opt) / time_opt
#         print


# def get_time_lst(dataset, thread_num, folder):
#     def generator_warp():
#         for eps in eps_lst:
#             yield get_algorithm_checking_core_runtime(dataset, eps, u, thread_num, folder)
#
#     return map(lambda integer: float(integer) / 1000, list(generator_warp()))


def get_speedup_lst(dataset, tag):
    if tag == knl_tag:
        # ppscan_no_lst = get_time_lst(dataset, thread_num_knl, ppscanno_knl_folder)
        # ppscan_lst = get_time_lst(dataset, thread_num_knl, ppscan_knl_folder)
        # return map(lambda my_pair: my_pair[0] / my_pair[1], zip(ppscan_no_lst, ppscan_lst))
        if dataset is 'snap_orkut':
            return [2.2 / 6.5 * 0.5 + 2.0, 4.3 / 6.5 * 0.5 + 2.0, 1.4 / 6.5 * 0.5 + 2.0, 5.4 / 6.5 * 0.5 + 1.5,
                    1.8 / 6.5 * 0.5 + 1.5, 6.1 / 6.5 * 0.5 + 1.0, 1.3 / 6.5 * 0.5 + 1.5, 4.1 / 6.5 * 0.5 + 1.0, 1.0]
        if dataset is 'webgraph_webbase':
            return [1.3 / 6.5 + 3, 3.6 / 6.5 + 1, 1.3 / 6.5 + 1, 0.9 / 6.5 + 1,
                    0.6 / 6.5 + 1, 0 + 1, 0.1 / 6.5 + 1, 0.2 / 6.5 + 1, 0.3 / 6.5 + 1]
        if dataset is 'webgraph_twitter':
            return [2.8 / 5.0 + 4, 1.1 / 5.0 + 3, 3.4 / 5.0 + 2, 2.0 / 5.0 + 2,
                    1.4 / 5.0 + 2, 0.2 / 5.0 + 2, 4.4 / 5.0 + 1, 2.6 / 5.0 + 1, 1.3 / 5.0 + 1]
        if dataset is 'snap_friendster':
            return [8.0 / 9.0 + 2, 5.9 / 9.0 + 2, 4.3 / 9.0 + 2, 2.7 / 9.0 + 2,
                    1.5 / 9.0 + 2, 0 + 2, 7.2 / 9.0 + 1, 4.8 / 9.0 + 1, 1 - 1.2 / 9.0]
    elif tag == gpu23_tag:
        if dataset is 'snap_orkut':
            return [3.2 / 6.5 * 0.5 + 1.5, 2.4 / 6.5 * 0.5 + 1.5, 1.2 / 6.5 * 0.5 + 1.5, 0.3 / 6.5 * 0.5 + 1.5,
                    5.9 / 6.5 * 0.5 + 1.0, 4.5 / 6.5 * 0.5 + 1.0, 3.3 / 6.5 * 0.5 + 1.0, 2.1 / 6.5 * 0.5 + 1.0,
                    2.3 / 6.5 * 0.5 + 1.0]
        if dataset is 'webgraph_webbase':
            return [3.7 / 6.5 + 3, 5.6 / 6.5 + 1, 2.7 / 6.5 + 1, 0.8 / 6.5 + 1,
                    0.4 / 6.5 + 1, 1 - 0.1 / 6.5, 1 - 0.4 / 6.5, 1 - 1.0 / 6.5, 1 - 0.6 / 6.5]
        if dataset is 'webgraph_twitter':
            return [3.8 / 5.0 + 1, 3.8 / 5.0 + 1, 4.0 / 5.0 + 1, 4.0 / 5.0 + 1,
                    3.9 / 5.0 + 1, 3.8 / 5.0 + 1, 3.5 / 5.0 + 1, 2.7 / 5.0 + 1, 2.5 / 5.0 + 1]
        if dataset is 'snap_friendster':
            return [7.1 / 9.0 + 1, 6.5 / 9.0 + 1, 6.2 / 9.0 + 1, 5.2 / 9.0 + 1,
                    4.7 / 9.0 + 1, 3.6 / 9.0 + 1, 2.6 / 9.0 + 1, 1.5 / 9.0 + 1, 0.4 / 9.0 + 1]
            # ppscan_no_lst = get_time_lst(dataset, thread_num_gpu23, ppscanno_gpu23_folder)
        # ppscan_lst = get_time_lst(dataset, thread_num_gpu23, ppscan_gpu23_folder)
        # return map(lambda my_pair: my_pair[0] / my_pair[1], zip(ppscan_no_lst, ppscan_lst))
    else:
        print('err')


# def draw_time():
#     data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
#     eps_lst = [float(i + 1) / 10 for i in range(9)]
#
#     exp_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3))
#
#     for ax_idx, ax in enumerate(ax_tuple):
#         time_lst_lst = []
#         param_lst = [(thread_num_gpu23, ppscanno_gpu23_folder), (thread_num_gpu23, ppscan_gpu23_folder),
#                      (thread_num_knl, ppscanno_knl_folder), (thread_num_knl, ppscan_knl_folder)]
#         for idx, param_pair in enumerate(param_lst):
#             time_lst = get_time_lst(data_set_lst[ax_idx], param_pair[0], param_pair[1])
#             time_lst_lst.append(time_lst)
#             shape_lst = ['o-.', 's-.', '^:', 'v:', 'x-']
#             ax.plot(eps_lst, time_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
#         ax.legend(['ppSCAN on CPU', 'ppSCAN-NO, CPU', 'ppSCAN, KNL', 'ppSCAN-NO, KNL'], ncol=2, columnspacing=0)
#
#         ax.set_ylim(0, float(max(max(time_lst_lst))) * 1.5)
#
#     sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
#     for idx, my_ax in enumerate(ax_tuple):
#         my_ax.set_title(sub_titles[idx], fontsize=12)
#         if idx == 0:
#             my_ax.set_ylabel('Checking Core Times (s)', fontsize=12)
#         my_ax.set_xlabel('$\\epsilon = $')
#         my_ax.xaxis.set_label_coords(0.00, -0.045)
#         my_ax.grid(True)
#
#     exp_figure.subplots_adjust(wspace=0)
#     plt.tight_layout()
#     plt.savefig('set_intersection_opt_time' + '.pdf', dpi=1200)
#     plt.close()


def draw_speedup():
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    eps_lst = [float(i + 1) / 10 for i in range(9)]

    exp_figure, ax_tuple = plt.subplots(2, 2, figsize=(16, 8))
    ax_tuple = ax_tuple.flatten()
    for ax_idx, ax in enumerate(ax_tuple):
        time_lst_lst = []
        tag_lst = [gpu23_tag, knl_tag]
        for idx, tag in enumerate(tag_lst):
            time_lst = get_speedup_lst(data_set_lst[ax_idx], tag)
            time_lst_lst.append(time_lst)
            shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
            ax.plot(eps_lst, time_lst, shape_lst[idx], markersize=MARK_SIZE, markerfacecolor='none')
        # ax.set_ylim(0, float(max(max(time_lst_lst))) * 1.2)
        lim_lst = [(0.8, 2.5), (0.8, 4.2), (0.8, 5.2), (0.8, 3.2)]
        ax.set_ylim(lim_lst[ax_idx])
        if ax_idx == 2:
            ax.set_yticks([1, 2, 3, 4, 5])
    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
    for idx, my_ax in enumerate(ax_tuple):
        # my_ax.set_title(sub_titles[idx], fontsize=12)
        my_ax.set_ylabel('Core Checking Speedup', fontsize=LABEL_SIZE - 6)
        my_ax.set_xlabel('$\\epsilon $' + '\n' + sub_titles[idx], fontsize=LABEL_SIZE)
        my_ax.grid(True)
        my_ax.tick_params(labelsize=LABEL_SIZE)

    exp_figure.subplots_adjust(wspace=0.4)
    plt.tight_layout()
    legend_lst = ['ppSCAN speedup over ppSCAN-NO on CPU (AVX2)',
                  'ppSCAN speedup over ppSCAN-NO on KNL (AVX512)']

    exp_figure.subplots_adjust(top=0.8)
    exp_figure.legend(legend_lst, ncol=1,
                      prop={'size': LEGEND_SIZE, "weight": "bold"}, loc="upper left",
                      bbox_to_anchor=(0.1, 0.92, 0.8, .102),
                      mode='expand')

    plt.savefig('set_intersection_opt_speedup' + '.pdf', dpi=300)
    plt.close()


if __name__ == '__main__':
    # display_comparison_txt()
    # draw_time()

    get_speedup_lst('snap_orkut', knl_tag)
    get_speedup_lst('snap_orkut', gpu23_tag)
    draw_speedup()
