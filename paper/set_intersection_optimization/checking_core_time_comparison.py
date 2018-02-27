import os
import matplotlib.pyplot as plt

ppscanno_knl_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/ppSCANNO_knl'
ppscan_knl_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/scalability_simd_paper2'
thread_num_knl = 256

ppscanno_gpu23_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/ppSCANNO_gpu23'
ppscan_gpu23_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/scalability_simd_paper_gpu23'
thread_num_gpu23 = 64

gpu23_tag = 'gpu23'
knl_tag = 'knl'

u = 5
eps_lst = [float(i + 1) / 10 for i in xrange(9)]
data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']

tag_lst = ['prune execution time', 'check core first-phase bsp time', 'check core second-phase bsp time',
           '3rd: core clustering time', 'non-core clustering time', 'Total time without IO']
legend_lst = ['1. prune', '2. check core', '3. cluster core', '4. cluster non-core', 'in total']


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


def get_algorithm_checking_core_runtime(dataset, eps, min_pts, thread_num, root_dir_path):
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

        for i in xrange(len(tag_lst)):
            assert len(my_lst_lst[0]) == len(my_lst_lst[i])
        return breakdown_time_lst[1]


def display_comparison_txt():
    print '====================On KNL(AVX512)=========================='
    for data_set in data_set_lst:
        for eps in eps_lst:
            time_no_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_knl, ppscanno_knl_folder)
            time_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_knl, ppscan_knl_folder)
            print time_no_opt, time_opt, float(time_no_opt) / time_opt
        print

    print '====================On GPU23(AVX2)=========================='
    for data_set in data_set_lst:
        for eps in eps_lst:
            time_no_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_gpu23, ppscanno_gpu23_folder)
            time_opt = get_algorithm_checking_core_runtime(data_set, eps, u, thread_num_gpu23, ppscan_gpu23_folder)
            print time_no_opt, time_opt, float(time_no_opt) / time_opt
        print


def get_time_lst(dataset, thread_num, folder):
    def generator_warp():
        for eps in eps_lst:
            yield get_algorithm_checking_core_runtime(dataset, eps, u, thread_num, folder)

    return map(lambda integer: float(integer) / 1000, list(generator_warp()))


def get_speedup_lst(dataset, tag):
    if tag == knl_tag:
        ppscan_no_lst = get_time_lst(dataset, thread_num_knl, ppscanno_knl_folder)
        ppscan_lst = get_time_lst(dataset, thread_num_knl, ppscan_knl_folder)
        return map(lambda my_pair: my_pair[0] / my_pair[1], zip(ppscan_no_lst, ppscan_lst))
    elif tag == gpu23_tag:
        ppscan_no_lst = get_time_lst(dataset, thread_num_gpu23, ppscanno_gpu23_folder)
        ppscan_lst = get_time_lst(dataset, thread_num_gpu23, ppscan_gpu23_folder)
        return map(lambda my_pair: my_pair[0] / my_pair[1], zip(ppscan_no_lst, ppscan_lst))
    else:
        print 'err'


def draw_time():
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    eps_lst = [float(i + 1) / 10 for i in xrange(9)]

    exp_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3))

    for ax_idx, ax in enumerate(ax_tuple):
        time_lst_lst = []
        param_lst = [(thread_num_gpu23, ppscanno_gpu23_folder), (thread_num_gpu23, ppscan_gpu23_folder),
                     (thread_num_knl, ppscanno_knl_folder), (thread_num_knl, ppscan_knl_folder)]
        for idx, param_pair in enumerate(param_lst):
            time_lst = get_time_lst(data_set_lst[ax_idx], param_pair[0], param_pair[1])
            time_lst_lst.append(time_lst)
            shape_lst = ['o-.', 's-.', '^:', 'v:', 'x-']
            ax.plot(eps_lst, time_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
        ax.legend(['ppSCAN on CPU', 'ppSCAN-NO, CPU', 'ppSCAN, KNL', 'ppSCAN-NO, KNL'], ncol=2, columnspacing=0)

        ax.set_ylim(0, float(max(max(time_lst_lst))) * 1.5)

    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
    for idx, my_ax in enumerate(ax_tuple):
        my_ax.set_title(sub_titles[idx], fontsize=12)
        if idx == 0:
            my_ax.set_ylabel('Checking Core Times (s)', fontsize=12)
        my_ax.set_xlabel('$\\epsilon = $')
        my_ax.xaxis.set_label_coords(0.00, -0.045)
        my_ax.grid(True)

    exp_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('set_intersection_opt_time' + '.pdf', dpi=1200)
    plt.close()


def draw_speedup():
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    eps_lst = [float(i + 1) / 10 for i in xrange(9)]

    exp_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3.0))

    for ax_idx, ax in enumerate(ax_tuple):
        time_lst_lst = []
        tag_lst = [gpu23_tag, knl_tag]
        for idx, tag in enumerate(tag_lst):
            time_lst = get_speedup_lst(data_set_lst[ax_idx], tag)
            time_lst_lst.append(time_lst)
            shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
            ax.plot(eps_lst, time_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
        ax.set_ylim(0, float(max(max(time_lst_lst))) * 1.2)
    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
    for idx, my_ax in enumerate(ax_tuple):
        # my_ax.set_title(sub_titles[idx], fontsize=12)
        if idx == 0:
            my_ax.set_ylabel('Core Checking Speedup', fontsize=12)
        my_ax.set_xlabel('$\\epsilon $' + '\n' + sub_titles[idx], fontsize=12)
        my_ax.grid(True)

    exp_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    legend_lst = ['ppSCAN speedup over ppSCAN-NO on CPU (AVX2)',
                  'ppSCAN speedup over ppSCAN-NO on KNL (AVX512)']

    plt.subplots_adjust(top=0.85)
    plt.legend(legend_lst, ncol=len(legend_lst),
               prop={'size': 12, "weight": "bold"}, loc=2,
               bbox_to_anchor=(-2.8, 1.3))

    plt.savefig('set_intersection_opt_speedup' + '.pdf', dpi=300)
    plt.close()


if __name__ == '__main__':
    display_comparison_txt()
    draw_time()

    # print get_speedup_lst('snap_orkut', knl_tag)
    # print get_speedup_lst('snap_orkut', gpu23_tag)
    draw_speedup()
