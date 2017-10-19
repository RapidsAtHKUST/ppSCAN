from scalability_figure import *


#
# x_lst = [float(i + 1) / 10 for i in range(9)]
# print x_lst
# print data_lst
# print len(data_lst)
# ax_title_lst = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) datset = friendster']



def display_time_division(filtered_lst, algorithm_name_lst, title, ax):
    hatch_lst = ['///', '|||', '---']

    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        ax.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color='w',
               hatch=hatch_lst[cur_shape_color_idx], alpha=1)
        cur_shape_color_idx += 1

    ax.set_ylim([0.0, max(map(max, filtered_lst)) * 1.2])
    ax.set_xticks(range(1, len(xtick_label_lst) + 1))
    ax.set_xticklabels(xtick_label_lst, rotation=30)
    ax.set_title(title)
    ax.legend(['other computation', 'workload reduction computation', 'similarity evaluation'])


if __name__ == '__main__':
    with open('breadown_time_gpu23.md') as ifs:
        all_data = map(lambda my_str:
                       map(float, list(reversed(my_str.split('|')[1:4]))),
                       filter(lambda line: 'SCAN' in line, ifs.readlines()))
        my_data_lst = [all_data[0:8], all_data[8:16], all_data[16:24]]
        dataset_name_lst = ['livejournal', 'orkut', 'twitter']

        breakdown_time_figure, ax_tuple = plt.subplots(1, 3, sharex=True, figsize=(16, 3))
        ax_title_lst = ['(a) dataset = livejournal', '(b) dataset = orkut', '(c) dataset = twitter']

        for idx, lst_lst in enumerate(my_data_lst):
            lst_lst = [map(lambda my_tuple: my_tuple[0], lst_lst), map(lambda my_tuple: my_tuple[1], lst_lst),
                       map(lambda my_tuple: my_tuple[2], lst_lst)]
            xtick_label_lst = ['SCAN\n$\\epsilon$: 0.2', 'SCAN\n$\\epsilon$: 0.4', 'SCAN\n$\\epsilon$: 0.6',
                               'SCAN\n$\\epsilon$: 0.8', 'pSCAN\n$\\epsilon$: 0.2', 'pSCAN\n$\\epsilon$: 0.4',
                               'pSCAN\n$\\epsilon$: 0.6', 'pSCAN\n$\\epsilon$: 0.8']
            display_time_division(recal_for_bar_char(lst_lst), xtick_label_lst, title=ax_title_lst[idx],
                                  ax=ax_tuple[idx])
        ax_tuple[0].set_ylabel('Breakdown Runtime (s)', fontsize=12)

        breakdown_time_figure.subplots_adjust(wspace=0)
        plt.tight_layout()
        plt.savefig('breakdown_time.' + 'pdf', dpi=1200)
        breakdown_time_figure.show()
