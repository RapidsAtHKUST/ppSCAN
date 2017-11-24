import matplotlib.pyplot as plt


def get_exp_data(input_file_path):
    def split_to_float(your_str):
        return map(float, your_str.strip().split(' | ')[1:])

    with open(input_file_path) as ifs:
        lines = ifs.readlines()
        max_eval_lines = map(split_to_float, filter(lambda line: line.startswith('max eval number'), lines))
        pscan_eval_lines = map(split_to_float, filter(lambda line: line.startswith('pscan eval number'), lines))
        ppscan_eval_lines = map(split_to_float, filter(lambda line: line.startswith('ppscan eval number'), lines))
        tuple_lst = zip(max_eval_lines, pscan_eval_lines, ppscan_eval_lines)
        print tuple_lst
        return tuple_lst


def first_exp_fix_orkut_vary_u(suffix_str='pdf'):
    data_lst = get_exp_data('exp1_fix_okrut.md')
    exp1_figure, ax_tuple = plt.subplots(1, 4, sharey=True, sharex=True, figsize=(16, 3))
    x_lst = [float(i + 1) / 10 for i in range(9)]
    print x_lst
    print data_lst
    print len(data_lst)
    ax_title_lst = ['(a) $\\mu$ = 2', '(b) $\\mu$ = 5', '(c) $\\mu$ = 15', '(d) $\\mu$ = 25']
    for idx, ax in enumerate(ax_tuple):
        print data_lst[idx]
        # ax.plot(x_lst, data_lst[idx][0], 'o-', label='pSCAN-Naive', linewidth=1.0, markersize=10,
        #         markerfacecolor='none')
        ax.plot(x_lst, data_lst[idx][1], 's-', label='pSCAN', linewidth=1.0, markersize=10, markerfacecolor='none')
        ax.plot(x_lst, data_lst[idx][2], '^-', label='ppSCAN', linewidth=1.0, markersize=10, markerfacecolor='none')
        ax.grid(True)
        ax.set_title(ax_title_lst[idx], fontsize=12)
        ax.set_ylim(0.0, 1.1)
        ax.set_xlabel('$\\epsilon = $')
        ax.xaxis.set_label_coords(0.00, -0.045)
        # ax.legend(('pSCAN-Naive', 'pSCAN', 'ppSCAN'))
        ax.legend(('pSCAN', 'ppSCAN'))
    ax_tuple[0].set_ylabel('Normalized Similarity Eval #', fontsize=12)

    exp1_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('workload_exp_fix_orkut.' + suffix_str, dpi=1200)
    exp1_figure.show()


def second_exp_fix_u_vary_datasets(suffix_str='pdf'):
    data_lst = get_exp_data('exp2_fix_u15.md')
    exp2_figure, ax_tuple = plt.subplots(1, 4, sharey=True, sharex=True, figsize=(16, 3))
    x_lst = [float(i + 1) / 10 for i in range(9)]
    print x_lst
    print data_lst
    print len(data_lst)
    ax_title_lst = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) datset = friendster']

    my_legends = [None] * len(ax_tuple)
    for idx, ax in enumerate(ax_tuple):
        print data_lst[idx]
        # line0, = ax.plot(x_lst, data_lst[idx][0], 'o-', label='pSCAN-Naive', linewidth=1.0, markersize=10,
        #                  markerfacecolor='none')
        line1, = ax.plot(x_lst, data_lst[idx][1], 's-', label='pSCAN', linewidth=1.0, markersize=10,
                         markerfacecolor='none')
        line2, = ax.plot(x_lst, data_lst[idx][2], '^-', label='ppSCAN', linewidth=1.0, markersize=10,
                         markerfacecolor='none')
        # my_legends[idx] = [line0, line1, line2]
        my_legends[idx] = [line1, line2]
        ax.grid(True)
        ax.set_title(ax_title_lst[idx], fontsize=12)
        ax.set_ylim(0.0, 1.1)
        ax.set_xlabel('$\\epsilon = $')
        ax.xaxis.set_label_coords(0.00, -0.045)
        # ax.legend(('pSCAN-Naive', 'pSCAN', 'ppSCAN'))
        ax.legend(('pSCAN', 'ppSCAN'))

    ax_tuple[0].set_ylabel('Normalized Similarity Eval #', fontsize=12)

    exp2_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('workload_exp_fix_u_15.' + suffix_str, dpi=1200)
    exp2_figure.show()


if __name__ == '__main__':
    first_exp_fix_orkut_vary_u()
    second_exp_fix_u_vary_datasets()

    # first_exp_fix_orkut_vary_u('png')
    # second_exp_fix_u_vary_datasets('png')
