import os
import matplotlib.pyplot as plt

gpu23_tag = 'gpu23'
knl_tag = 'knl'
algorithm_lst = ['scan_super_naive', 'pscan', 'anyscan', 'scan_xp', 'ppscan']


def get_dict(algorithm_name, tag):
    with open('data-json' + os.sep + algorithm_name + '.json') as ifs:
        lines = ifs.readlines()
        return eval(''.join(lines))[tag]


# get time lst, per algorithm, dataset and tag(platform)
def get_time_lst(algorithm_name, tag, data_set, inf_val=10000):
    if algorithm_name == 'anyscan':
        if data_set == 'webgraph_webbase' or data_set == 'snap_friendster':
            return [float(inf_val) for _ in xrange(9)]

    time_str_lst = get_dict(algorithm_name, tag)[data_set]
    time_lst_val = map(
        lambda ele_str: float(inf_val) if ele_str == 'RE' or ele_str == 'TLE'  else float(ele_str) / 1000, time_str_lst)
    return time_lst_val


def draw_figures_per_platform(platform_tag):
    data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    eps_lst = [float(i + 1) / 10 for i in xrange(9)]

    exp_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3))

    for ax_idx, ax in enumerate(ax_tuple):
        time_lst_lst = []
        for idx, algorithm in enumerate(algorithm_lst):
            time_lst = get_time_lst(algorithm, platform_tag, data_set_lst[ax_idx], 10000)
            time_lst_lst.append(time_lst)
            shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
            ax.plot(eps_lst, time_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
            ax.set_yscale('log')
        ax.legend(['SCAN', 'pSCAN', 'anySCAN', 'SCAN-XP', 'ppSCAN'], ncol=2)

        factor_lst = [70, 220, 50, 50]
        if platform_tag == knl_tag:
            factor_lst = [100, 200, 80, 100]
        TLE = 10 ** 4
        upper_bound_lst = [10 ** 4, 8 * TLE, 8 * TLE, 8 * TLE]
        ax.set_ylim(float(min(map(min, time_lst_lst))) / factor_lst[ax_idx], upper_bound_lst[ax_idx])
        if ax_idx > 0:
            ax.text(0.5, 10 ** 4 * 3, '$10^4$ means TLE(>5400s) or RE(runtime error)', horizontalalignment='center')
        else:
            ax.text(0.5, 10 ** 3 * 3.75, '$10^4$ means TLE(>5400s) or RE(runtime error)', horizontalalignment='center')

    sub_titles = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) dataset = friendster']
    for idx, my_ax in enumerate(ax_tuple):
        my_ax.set_title(sub_titles[idx], fontsize=12)
        if idx == 0:
            my_ax.set_ylabel('Runtime (seconds)', fontsize=12)
        my_ax.set_xlabel('$\\epsilon = $')
        my_ax.xaxis.set_label_coords(0.00, -0.045)
        my_ax.grid(True)

    exp_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('comparison_' + platform_tag + '.pdf', dpi=1200)
    exp_figure.show()


if __name__ == '__main__':
    # print get_time_lst('anyscan', gpu23_tag, 'webgraph_webbase', 10000)
    # print get_time_lst('ppscan', gpu23_tag, 'webgraph_webbase')
    # print get_time_lst('scan_super_naive', knl_tag, 'webgraph_webbase')
    draw_figures_per_platform(gpu23_tag)
    draw_figures_per_platform(knl_tag)
