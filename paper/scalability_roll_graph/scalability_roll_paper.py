import matplotlib.pyplot as plt
import os


def process_dataset(dataset, folder_name='self-speedup-gpu23'):
    with open(folder_name + os.sep + dataset + '.md') as ifs:
        lines = ifs.readlines()[2:11]
        data_lst = map(lambda line: line.strip().split(' | '), lines)
        ppscan_runtime_lst = map(lambda data: float(data[1]), data_lst)
        speedup_lst = map(lambda data: float(data[3]), data_lst)
        return ppscan_runtime_lst, speedup_lst


if __name__ == '__main__':
    if not os.path.exists('figures'):
        os.mkdir('figures')

    # gpu23
    ppscan_runtime_lst_lst = []
    ppscan_speedup_lst_lst = []
    for dataset in ['n50_m20_1billion_edge', 'n25_m40_1billion_edge', 'n16dot7_m60_1billion_edge',
                    'n12dot5_m80_1billion_edge']:
        print dataset
        ppscan_runtime_lst, ppcan_speedup_lst = process_dataset(dataset)
        ppscan_runtime_lst_lst.append(ppscan_runtime_lst)
        ppscan_speedup_lst_lst.append(ppcan_speedup_lst)
    print ppscan_runtime_lst_lst
    print ppscan_speedup_lst_lst

    # knl
    ppscan_runtime_lst_lst_knl = []
    ppscan_speedup_lst_lst_knl = []
    for dataset in ['n50_m20_1billion_edge', 'n25_m40_1billion_edge', 'n16dot7_m60_1billion_edge',
                    'n12dot5_m80_1billion_edge']:
        print dataset
        ppscan_runtime_lst_knl, ppscan_speedup_lst_knl = process_dataset(dataset, folder_name='self-speedup-knl')
        ppscan_runtime_lst_lst_knl.append(ppscan_runtime_lst_knl)
        ppscan_speedup_lst_lst_knl.append(ppscan_speedup_lst_knl)
    print ppscan_runtime_lst_lst_knl, ppscan_speedup_lst_lst_knl

    eps_lst = [float(i + 1) / 10 for i in range(9)]
    exp2_figure, ax_tuple = plt.subplots(1, 4, sharex=True, figsize=(16, 3))
    # legend_lst = ['avg deg = 40', 'avg deg = 80', 'avg deg = 120', 'avg deg = 160']
    legend_lst = ['ROLL-d40', 'ROLL-d80', 'ROLL-d120', 'ROLL-d160']

    ax = ax_tuple[0]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
        ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst)) * 1.1)
    ax.set_ylabel('Runtime (s)', fontsize=12)
    ax.set_title('(a) on CPU', fontsize=12)
    ax.set_xlabel('$\\epsilon = $')
    ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)

    ax = ax_tuple[1]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, speedup_lst in enumerate(ppscan_speedup_lst_lst):
        ax.plot(eps_lst, speedup_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_speedup_lst_lst)) * 1.1)
    ax.set_ylabel('Self-Speedup', fontsize=12)
    ax.set_title('on CPU', fontsize=12)
    ax.set_xlabel('$\\epsilon = $')
    ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)

    ax = ax_tuple[2]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst_knl):
        ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst_knl)) * 1.1)
    ax.set_ylabel('Runtime (s)', fontsize=12)
    ax.set_title('(b) on KNL', fontsize=12)
    ax.set_xlabel('$\\epsilon = $')
    ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)

    ax = ax_tuple[3]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, speedup_lst in enumerate(ppscan_speedup_lst_lst_knl):
        ax.plot(eps_lst, speedup_lst, shape_lst[idx], markersize=10, markerfacecolor='none')
    ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_speedup_lst_lst_knl)) * 1.1)
    ax.set_ylabel('Self-Speedup', fontsize=12)
    ax.set_title('on KNL', fontsize=12)
    ax.set_xlabel('$\\epsilon = $')
    ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)

    exp2_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    # plt.savefig('figures/scalability_roll_graph.' + 'pdf', dpi=1200)
    plt.savefig('scalability_roll_graph.' + 'pdf', dpi=1200)
    exp2_figure.show()
