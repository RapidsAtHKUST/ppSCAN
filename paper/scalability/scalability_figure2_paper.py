import matplotlib.pyplot as plt
import os

server_workload_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/workload-work-efficient-2'
server_scalability_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/scalability_new5_exp_paper'
eps_lst = [float(i + 1) / 10 for i in range(8)]
u_lst = [2, 5, 10, 15]
legend_lst = ['$\\mu = 2 $', '$\\mu = 5 $', '$\\mu = 10 $', '$\\mu = 15 $']


def get_pscan_runtime(dataset, eps, min_pts, root_dir_path='.'):
    runtime_tag = 'Total time without IO'

    file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                             '-'.join(['output', dataset, str(eps), str(min_pts)]) + '.txt'])
    with open(file_path) as ifs:
        lines = ifs.readlines()

    # runtime unit: ms
    runtime_lst = map(lambda time_str: eval(time_str.split('ms')[0]) if 'ms' in time_str else eval(time_str) / 1000,
                      map(lambda my_str: my_str.split(':')[-1], filter(lambda line: runtime_tag in line, lines)))
    assert len(runtime_lst) == 4
    return float(runtime_lst[-1]) / 1000


def get_pscan_runtime_lst_lst(dataset):
    my_lst_lst = map(lambda u:
                     map(lambda eps:
                         get_pscan_runtime(dataset, eps, u, server_workload_folder), eps_lst), u_lst)
    return my_lst_lst


def get_ppscan_runtime(dataset, eps, min_pts, root_dir_path='.'):
    runtime_tag = 'Total time without IO'

    file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                             '-'.join(['output', dataset, str(eps), str(min_pts), '40']) + '.txt'])
    with open(file_path) as ifs:
        lines = ifs.readlines()

    # runtime unit: ms
    runtime_lst = map(lambda time_str: eval(time_str.split('ms')[0]) if 'ms' in time_str else eval(time_str) / 1000,
                      map(lambda my_str: my_str.split(':')[-1], filter(lambda line: runtime_tag in line, lines)))
    return float(min(runtime_lst)) / 1000


def get_ppscan_runtime_lst_lst(dataset):
    my_lst_lst = map(lambda u:
                     map(lambda eps:
                         get_ppscan_runtime(dataset, eps, u, server_scalability_folder), eps_lst), [2, 5, 10, 15])
    return my_lst_lst


def process_dataset(dataset):
    print '\n\ndataset:', dataset
    ppscan_runtime_lst_lst = get_ppscan_runtime_lst_lst(dataset)

    pscan_runtime_lst_lst = get_pscan_runtime_lst_lst(dataset)

    speedup_lst_lst = map(
        lambda my_pair: map(lambda idx: float(my_pair[1][idx]) / my_pair[0][idx], range(len(my_pair[0]))),
        zip(ppscan_runtime_lst_lst, pscan_runtime_lst_lst))

    print '\nruntime'
    for ppscan_runtime_lst in ppscan_runtime_lst_lst:
        print ppscan_runtime_lst

    print '\nspeedup over pscan'
    for speedup_lst in speedup_lst_lst:
        print speedup_lst
    return ppscan_runtime_lst_lst, speedup_lst_lst


if __name__ == '__main__':
    ppscan_runtime_tag = 'ppSCAN Runtime (s)'
    ppscan_speedup_tag = 'Speedup over pSCAN'
    friendster_tag = '(a) dataset = friendster'
    twitter_tag = '(b) dataset = twitter'
    y_label = [ppscan_runtime_tag, ppscan_speedup_tag, ppscan_runtime_tag, ppscan_speedup_tag]
    sub_titles = [friendster_tag, friendster_tag, twitter_tag, twitter_tag]

    exp2_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3))

    # (a) friendster runtime
    ppscan_runtime_lst_lst, speedup_lst_lst = process_dataset('snap_friendster')
    ax = ax_tuple[0]
    shape_lst = ['o-', 's-', '^-', 'x-']
    for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
        ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst)) * 1.1)

    # (a) friendster speedup over pscan
    ax = ax_tuple[1]
    shape_lst = ['o-', 's-', '^-', 'x-']
    for idx, speedup_lst in enumerate(speedup_lst_lst):
        ax.plot(eps_lst, speedup_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, speedup_lst_lst)) * 1.1)

    # (b) twitter runtime
    ppscan_runtime_lst_lst, speedup_lst_lst = process_dataset('webgraph_twitter')
    ax = ax_tuple[2]
    for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
        ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst)) * 1.1)

    # (b) twitter speedup over pscan
    ax = ax_tuple[3]
    shape_lst = ['o-', 's-', '^-', 'x-']
    for idx, speedup_lst in enumerate(speedup_lst_lst):
        ax.plot(eps_lst, speedup_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, speedup_lst_lst)) * 1.1)

    for idx, my_ax in enumerate(ax_tuple):
        my_ax.set_title(sub_titles[idx], fontsize=12)
        my_ax.set_ylabel(y_label[idx], fontsize=12)
        my_ax.set_xlabel('$\\epsilon = $')
        my_ax.xaxis.set_label_coords(0.00, -0.045)

    exp2_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('scalability_exp2_varying_everything.' + 'pdf', dpi=1200)
    exp2_figure.show()
