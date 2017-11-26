import os

ppscanno_knl_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/ppSCANNO_knl'
ppscan_knl_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/scalability_simd_paper2'
thread_num_knl = 256

ppscanno_gpu23_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/ppSCANNO_gpu23'
ppscan_gpu23_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/scalability_simd_paper_gpu23'
thread_num_gpu23 = 64

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


if __name__ == '__main__':
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
