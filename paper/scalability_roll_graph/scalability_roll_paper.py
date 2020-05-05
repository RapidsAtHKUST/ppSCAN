import matplotlib.pyplot as plt
import os


def process_dataset(dataset, folder_name='self-speedup-gpu23'):
    with open(folder_name + os.sep + dataset + '.md') as ifs:
        lines = ifs.readlines()[2:11]
        data_lst = list(map(lambda line: line.strip().split(' | '), lines))
        ppscan_runtime_lst = list(map(lambda data: float(data[1]), data_lst))
        speedup_lst = list(map(lambda data: float(data[3]), data_lst))
        return ppscan_runtime_lst, speedup_lst


LABEL_SIZE = 24
TICK_SIZE = 24
LEGEND_SIZE = 24
MARK_SIZE = 18

if __name__ == '__main__':
    if not os.path.exists('figures'):
        os.mkdir('figures')

    # gpu23
    ppscan_runtime_lst_lst = []
    ppscan_speedup_lst_lst = []
    for dataset in ['n50_m20_1billion_edge', 'n25_m40_1billion_edge', 'n16dot7_m60_1billion_edge',
                    'n12dot5_m80_1billion_edge']:
        # print dataset
        ppscan_runtime_lst, ppcan_speedup_lst = process_dataset(dataset)
        ppscan_runtime_lst_lst.append(ppscan_runtime_lst)
        ppscan_speedup_lst_lst.append(ppcan_speedup_lst)
    # print ppscan_runtime_lst_lst
    # print ppscan_speedup_lst_lst

    # knl
    ppscan_runtime_lst_lst_knl = []
    ppscan_speedup_lst_lst_knl = []
    for dataset in ['n50_m20_1billion_edge', 'n25_m40_1billion_edge', 'n16dot7_m60_1billion_edge',
                    'n12dot5_m80_1billion_edge']:
        # print dataset
        ppscan_runtime_lst_knl, ppscan_speedup_lst_knl = process_dataset(dataset, folder_name='self-speedup-knl')
        ppscan_runtime_lst_lst_knl.append(ppscan_runtime_lst_knl)
        ppscan_speedup_lst_lst_knl.append(ppscan_speedup_lst_knl)
    # print ppscan_runtime_lst_lst_knl, ppscan_speedup_lst_lst_knl

    eps_lst = [float(i + 1) / 10 for i in range(9)]
    exp2_figure, ax_tuple = plt.subplots(2, 2, figsize=(16, 8))
    # legend_lst = ['avg deg = 40', 'avg deg = 80', 'avg deg = 120', 'avg deg = 160']
    legend_lst = ['ROLL-d40', 'ROLL-d80', 'ROLL-d120', 'ROLL-d160']
    ax_tuple = ax_tuple.flatten()
    ax = ax_tuple[0]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst):
        ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=MARK_SIZE, markerfacecolor='none')
    # ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst)) * 1.1)
    ax.set_ylabel('Runtime (seconds)', fontsize=LABEL_SIZE)
    # ax.set_title('(a) on CPU', fontsize=12)
    ax.set_xlabel('$\\epsilon$' + '\n' + '(a) on CPU', fontsize=LABEL_SIZE)
    # ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)
    ax.set_xticks(eps_lst)
    ax.tick_params(labelsize=LABEL_SIZE)

    ax = ax_tuple[1]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, speedup_lst in enumerate(ppscan_speedup_lst_lst):
        ax.plot(eps_lst, speedup_lst, shape_lst[idx], markersize=MARK_SIZE, markerfacecolor='none')
    # ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_speedup_lst_lst)) * 1.1)
    ax.set_ylabel('Self-Speedup', fontsize=LABEL_SIZE)
    # ax.set_title('on CPU', fontsize=12)
    ax.set_xlabel('$\\epsilon$' + '\n' + 'on CPU', fontsize=LABEL_SIZE)
    # ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)
    ax.set_xticks(eps_lst)
    ax.tick_params(labelsize=LABEL_SIZE)

    ax = ax_tuple[2]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, runtime_lst in enumerate(ppscan_runtime_lst_lst_knl):
        ax.plot(eps_lst, runtime_lst, shape_lst[idx], markersize=MARK_SIZE, markerfacecolor='none')
    # ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_runtime_lst_lst_knl)) * 1.1)
    ax.set_ylabel('Runtime (seconds)', fontsize=LABEL_SIZE)
    # ax.set_title('(b) on KNL', fontsize=12)
    ax.set_xlabel('$\\epsilon$' + '\n' + '(b) on KNL', fontsize=LABEL_SIZE)
    # ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)
    ax.set_xticks(eps_lst)
    ax.tick_params(labelsize=LABEL_SIZE)

    ax = ax_tuple[3]
    shape_lst = ['o-.', 's--', '^:', 'v:', 'x-']
    for idx, speedup_lst in enumerate(ppscan_speedup_lst_lst_knl):
        ax.plot(eps_lst, speedup_lst, shape_lst[idx], markersize=MARK_SIZE, markerfacecolor='none')
    # ax.legend(legend_lst)
    ax.set_ylim(0, max(map(max, ppscan_speedup_lst_lst_knl)) * 1.1)
    ax.set_ylabel('Self-Speedup', fontsize=LABEL_SIZE)
    # ax.set_title('on KNL', fontsize=12)
    ax.set_xlabel('$\\epsilon$' + '\n' + 'on KNL', fontsize=LABEL_SIZE)
    # ax.xaxis.set_label_coords(0.00, -0.045)
    ax.grid(True)
    ax.set_xticks(eps_lst)
    ax.tick_params(labelsize=LABEL_SIZE)

    # exp2_figure.subplots_adjust(wspace=0)
    plt.tight_layout()

    plt.subplots_adjust(top=0.88)
    exp2_figure.legend(legend_lst, ncol=len(legend_lst),
               prop={'size': LEGEND_SIZE, "weight": "bold"}, loc="upper left",
               bbox_to_anchor=(0., 0.92, 1., .102), mode='expand')

    plt.savefig('scalability_roll_graph.' + 'pdf', dpi=300)
    # exp2_figure.show()
    plt.close()
