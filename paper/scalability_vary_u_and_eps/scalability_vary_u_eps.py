import matplotlib.pyplot as plt
import os

server_scalability_folder = '/home/yche/mnt/luocpu8/nfsshare/share/python_experiments/scalability_simd_paper2'
eps_lst = [float(i + 1) / 10 for i in range(9)]
u_lst = [2, 5, 10, 15]
legend_lst = ['$\\mu = 2 $', '$\\mu = 5 $', '$\\mu = 10 $', '$\\mu = 15 $']


def get_ppscan_runtime(dataset, eps, min_pts, root_dir_path, thread_num=256):
    runtime_tag = 'Total time without IO'

    file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                             '-'.join(['output', dataset, str(eps), str(min_pts), str(thread_num)]) + '.txt'])
    with open(file_path) as ifs:
        lines = ifs.readlines()

    # runtime unit: ms
    runtime_lst = map(lambda time_str: eval(time_str.split('ms')[0]) if 'ms' in time_str else eval(time_str) / 1000,
                      map(lambda my_str: my_str.split(':')[-1], filter(lambda line: runtime_tag in line, lines)))
    if len(runtime_lst) == 0:
        print eps, min_pts
    return float(min(runtime_lst)) / 1000


def get_ppscan_runtime_lst_lst(dataset):
    my_lst_lst = map(lambda u:
                     map(lambda eps:
                         get_ppscan_runtime(dataset, eps, u, root_dir_path=server_scalability_folder), eps_lst),
                     [2, 5, 10, 15])
    return my_lst_lst


def process_dataset(dataset):
    print '\n\ndataset:', dataset
    ppscan_runtime_lst_lst = get_ppscan_runtime_lst_lst(dataset)

    print '\nruntime'
    for ppscan_runtime_lst in ppscan_runtime_lst_lst:
        print ppscan_runtime_lst

    return ppscan_runtime_lst_lst


if __name__ == '__main__':
    ppscan_runtime_tag = 'ppSCAN Runtime (s)'
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']

    exp2_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3))

    for idx, ax in enumerate(ax_tuple):
        ppscan_runtime_lst_lst = process_dataset(data_set_lst[idx])
        shape_lst = ['o-', 's-', '^-', 'x-']
        for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
            ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
        ax.legend(legend_lst)
        ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst)) * 1.1)

    for idx, my_ax in enumerate(ax_tuple):
        my_ax.set_title(sub_titles[idx], fontsize=12)
        if idx == 0:
            my_ax.set_ylabel('Runtime (s)', fontsize=12)
        my_ax.set_xlabel('$\\epsilon = $')
        my_ax.xaxis.set_label_coords(0.00, -0.045)
        my_ax.grid(True)
    exp2_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('scalability_exp2_varying_u_and_eps.' + 'pdf', dpi=1200)
    exp2_figure.show()
