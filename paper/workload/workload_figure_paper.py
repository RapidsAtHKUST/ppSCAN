import matplotlib.pyplot as plt

LABEL_SIZE = 24
TICK_SIZE = 24
LEGEND_SIZE = 24
MARK_SIZE = 18


def get_exp_data(input_file_path):
    def split_to_float(your_str):
        return list(map(float, your_str.strip().split(' | ')[1:]))

    with open(input_file_path) as ifs:
        lines = ifs.readlines()
        max_eval_lines = list(map(split_to_float, filter(lambda line: line.startswith('max eval number'), lines)))
        pscan_eval_lines = list(map(split_to_float, filter(lambda line: line.startswith('pscan eval number'), lines)))
        ppscan_eval_lines = list(map(split_to_float, filter(lambda line: line.startswith('ppscan eval number'), lines)))
        tuple_lst = zip(max_eval_lines, pscan_eval_lines, ppscan_eval_lines)
        return list(tuple_lst)


def first_exp_fix_orkut_vary_u(suffix_str='pdf'):
    data_lst = get_exp_data('exp_data/exp1_fix_okrut.md')
    exp1_figure, ax_tuple = plt.subplots(2, 2, figsize=(16, 8))
    x_lst = [float(i + 1) / 10 for i in range(9)]
    ax_title_lst = ['(a) $\\mu$ = 2', '(b) $\\mu$ = 5', '(c) $\\mu$ = 15', '(d) $\\mu$ = 25']
    ax_tuple = ax_tuple.flatten()

    for idx, ax in enumerate(ax_tuple):
        ax.plot(x_lst, data_lst[idx][1], 'o-.', label='pSCAN', linewidth=1.0, markersize=MARK_SIZE,
                markerfacecolor='none')
        ax.plot(x_lst, data_lst[idx][2], 's:', label='ppSCAN', linewidth=1.0, markersize=MARK_SIZE,
                markerfacecolor='none')
        ax.grid(True)
        ax.set_title(ax_title_lst[idx], fontsize=12)
        ax.set_ylim(0.0, 1.1)
        ax.set_xlabel('$\\epsilon = $')
        ax.tick_params(labelsize=TICK_SIZE)
        ax.xaxis.set_label_coords(0.00, -0.045)
        # ax.legend(('pSCAN-Naive', 'pSCAN', 'ppSCAN'))
        ax.legend(('pSCAN', 'ppSCAN'), prop={'size': 12, "weight": "bold"})
        ax.set_ylabel('Normalized Similarity Eval #', fontsize=12)

    exp1_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('figures/workload_exp_fix_orkut.' + suffix_str, dpi=1200)
    exp1_figure.show()


def second_exp_fix_u_vary_datasets(input_name, output_name, suffix_str='pdf'):
    data_lst = get_exp_data('exp_data/' + input_name)
    print(data_lst)
    exp2_figure, ax_tuple = plt.subplots(2, 2, figsize=(16, 8))
    ax_tuple = ax_tuple.flatten()

    x_lst = list([float(i + 1) / 10 for i in range(9)])
    ax_title_lst = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) datset = friendster']

    my_legends = list([None] * len(ax_tuple))
    for idx, ax in enumerate(ax_tuple):
        # print data_lst[idx]
        # line0, = ax.plot(x_lst, data_lst[idx][0], 'o-', label='pSCAN-Naive', linewidth=1.0, markersize=10,
        #                  markerfacecolor='none')
        line1, = ax.plot(x_lst, data_lst[idx][1], 'o-.', label='pSCAN', linewidth=1.0, markersize=MARK_SIZE,
                         markerfacecolor='none')
        line2, = ax.plot(x_lst, data_lst[idx][2], 's--', label='ppSCAN', linewidth=1.0, markersize=MARK_SIZE,
                         markerfacecolor='none')
        # my_legends[idx] = [line0, line1, line2]
        my_legends[idx] = [line1, line2]
        ax.grid(True)
        # ax.set_title(ax_title_lst[idx], fontsize=12)
        ax.set_ylim(0.0, 1.1)
        ax.tick_params(labelsize=TICK_SIZE)
        # ax.set_xlabel('$\\epsilon = $')
        ax.set_xlabel('$\\epsilon$' + '\n' + ax_title_lst[idx], fontsize=LABEL_SIZE)
        ax.legend(('pSCAN', 'ppSCAN'), prop={'size': LEGEND_SIZE, "weight": "bold"})
        ax.set_xticks(x_lst)
        ax.set_ylabel('Normalized Number', fontsize=LABEL_SIZE - 2)

    exp2_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('figures/' + output_name + '.' + suffix_str, dpi=300)
    plt.close()


if __name__ == '__main__':
    second_exp_fix_u_vary_datasets('exp2_fix_u5.md', 'workload_exp_fix_u_5', 'pdf')
    second_exp_fix_u_vary_datasets('exp2_fix_u10.md', 'workload_exp_fix_u_10', 'pdf')
    second_exp_fix_u_vary_datasets('exp2_fix_u15.md', 'workload_exp_fix_u_15', 'pdf')
