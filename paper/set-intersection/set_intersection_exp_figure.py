import matplotlib.pyplot as plt


def transpose(my_lst_lst):
    col_num = len(my_lst_lst[0])
    return map(lambda idx: map(lambda my_tuple: my_tuple[idx], my_lst_lst), range(col_num))


def get_data(datset_name):
    with open(datset_name + '.md') as ifs:
        lines = map(lambda line: line.strip(), ifs.readlines())
        reduction_factor_rows = map(lambda line: map(float, line.split(' | ')[1:]), lines[4:13])
        bound_ratio_rows = map(lambda line: map(float, line.split(' | ')[1:5]), lines[32:41])
        return transpose(reduction_factor_rows), transpose(bound_ratio_rows)


def accumulate_for_bar_char(breakdown_time_lst):
    for idx in xrange(len(breakdown_time_lst)):
        for idx2 in xrange(idx + 1, len(breakdown_time_lst)):
            breakdown_time_lst[idx] = map(lambda pair: sum(pair),
                                          zip(breakdown_time_lst[idx], breakdown_time_lst[idx2]))
    return breakdown_time_lst


if __name__ == '__main__':
    exp1_figure, ax_tuple = plt.subplots(2, 4, figsize=(16, 6))

    ax_title_lst = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) datset = friendster']
    dataset_lst = ['orkut', 'webbase', 'twitter', 'friendster']
    eps_lst = [float(i + 1) / 10 for i in range(9)]

    # firstly, reduction factor
    legend_lst = ['instruction#', 'memory load#', 'accumulated time']
    for idx, ax in enumerate(ax_tuple[0]):
        shape_lst = ['o-', 's-', '^-', 'x-']
        reduction_lst_lst, bound_lst_lst = get_data(dataset_lst[idx])
        for shape_idx, reduction_lst in enumerate(reduction_lst_lst):
            ax.plot(eps_lst, reduction_lst, shape_lst[shape_idx], markersize=10, markerfacecolor='none')
            ax.set_ylim(1.0, max(map(max, reduction_lst_lst)) * 1.3)
            ax.legend(legend_lst)
            ax.set_title(ax_title_lst[idx], fontsize=12)
            ax.set_xlabel('$\\epsilon = $')
            ax.xaxis.set_label_coords(0.00, -0.045)
            ax.grid(True)
    ax_tuple[0][0].set_ylabel('Reduction Factor', fontsize=12)

    ## secondly, breakdown bound ratio
    legend_lst = ['front-end', 'bad-speculation', 'back-end core', 'back-end memory']
    hatch_lst = ['\\\\\\', '////', '----', '||||']
    color_lst = ['b', 'orange', 'g', 'm']
    for idx, ax in enumerate(ax_tuple[1]):
        reduction_lst_lst, bound_lst_lst = get_data(dataset_lst[idx])
        for shape_idx, bound_lst in enumerate(accumulate_for_bar_char(bound_lst_lst)):
            ax.bar([i + 1 for i in range(9)], bound_lst, hatch=hatch_lst[shape_idx], edgecolor=color_lst[shape_idx],
                   color='w', alpha=1)
            ax.set_ylim(0.0, 1.15)
            ax.legend(legend_lst, ncol=2, loc='best', columnspacing=0.4)
            ax.set_xticklabels([0.0, 0.2, 0.4, 0.6, 0.8])
            ax.set_xlabel('$\\epsilon = $')
            ax.xaxis.set_label_coords(0.00, -0.045)
    ax_tuple[1][0].set_ylabel('CPU Pipeline Bound Ratio', fontsize=10)

    exp1_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('set_intersection_exp.pdf', dpi=1200)
    exp1_figure.show()
