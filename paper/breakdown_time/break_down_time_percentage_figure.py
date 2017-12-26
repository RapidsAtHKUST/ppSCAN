from scalability_figure import *


def display_time_division(filtered_lst, algorithm_name_lst, title, ax):
    hatch_lst = ['///', '||', '--']
    color_lst = ['g', 'b', 'orange']
    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        ax.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color='w',
               hatch=hatch_lst[cur_shape_color_idx], alpha=1, edgecolor=color_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    ax.set_ylim([0.0, max(map(max, filtered_lst)) * 1.2])
    ax.set_xticks(range(1, len(xtick_label_lst) + 1))
    ax.set_xticklabels(xtick_label_lst, rotation=30)
    ax.set_title(title, fontsize=12)
    ax.legend(['other computation', 'workload reduction computation', 'similarity evaluation'])


def display_percentage_division(filtered_lst, algorithm_name_lst, title, ax):
    hatch_lst = ['///', '||', '--']
    color_lst = ['g', 'b', 'orange']
    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        ax.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color='w',
               hatch=hatch_lst[cur_shape_color_idx], alpha=1, edgecolor=color_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    ax.set_ylim([0.0, max(map(max, filtered_lst)) * 1.8])
    ax.set_yticks([0.0, 0.2, 0.4, 0.6, 0.8, 1.0])
    ax.set_xticks(range(1, len(xtick_label_lst) + 1))
    ax.set_xticklabels(xtick_label_lst, rotation=30)
    ax.set_title(title, fontsize=12)
    ax.legend(['other computation', 'workload reduction computation', 'similarity evaluation'])


if __name__ == '__main__':
    with open('breadown_time_gpu23.md') as ifs:
        all_data = map(lambda my_str:
                       map(float, list(reversed(my_str.split('|')[1:4]))),
                       filter(lambda line: 'SCAN' in line, ifs.readlines()))
        my_data_lst = [all_data[0:8], all_data[8:16], all_data[16:24]]
        dataset_name_lst = ['livejournal', 'orkut', 'twitter']

        breakdown_time_figure, ax_tuple = plt.subplots(1, 2, sharex=True, figsize=(12, 3))

        # 1st: draw breakdown time
        lst_lst = my_data_lst[2]
        lst_lst = [map(lambda my_tuple: my_tuple[0], lst_lst), map(lambda my_tuple: my_tuple[1], lst_lst),
                   map(lambda my_tuple: my_tuple[2], lst_lst)]
        xtick_label_lst = ['SCAN\n$\\epsilon$: 0.2', 'SCAN\n$\\epsilon$: 0.4', 'SCAN\n$\\epsilon$: 0.6',
                           'SCAN\n$\\epsilon$: 0.8', 'pSCAN\n$\\epsilon$: 0.2', 'pSCAN\n$\\epsilon$: 0.4',
                           'pSCAN\n$\\epsilon$: 0.6', 'pSCAN\n$\\epsilon$: 0.8']
        display_time_division(recal_for_bar_char(lst_lst), xtick_label_lst, title='(a) Breakdown Time',
                              ax=ax_tuple[0])
        ax_tuple[0].set_ylabel('Runtime (s)', fontsize=12)

        # 2nd: draw percentage
        lst_lst = my_data_lst[2]
        lst_lst = map(lambda my_tuple:
                      map(lambda ele: ele / sum(my_tuple), my_tuple),
                      lst_lst)
        print 'orignal:', lst_lst
        lst_lst = [map(lambda my_tuple: my_tuple[0], lst_lst), map(lambda my_tuple: my_tuple[1], lst_lst),
                   map(lambda my_tuple: my_tuple[2], lst_lst)]
        xtick_label_lst = ['SCAN\n$\\epsilon$: 0.2', 'SCAN\n$\\epsilon$: 0.4', 'SCAN\n$\\epsilon$: 0.6',
                           'SCAN\n$\\epsilon$: 0.8', 'pSCAN\n$\\epsilon$: 0.2', 'pSCAN\n$\\epsilon$: 0.4',
                           'pSCAN\n$\\epsilon$: 0.6', 'pSCAN\n$\\epsilon$: 0.8']
        display_percentage_division(recal_for_bar_char(lst_lst), xtick_label_lst, title='(b) Breakdown Percentage',
                                    ax=ax_tuple[1])
        ax_tuple[1].set_ylabel('Percentage', fontsize=12)

        breakdown_time_figure.subplots_adjust(wspace=0)
        plt.tight_layout()
        plt.savefig('breakdown_time.' + 'pdf', dpi=1200)
        breakdown_time_figure.show()
