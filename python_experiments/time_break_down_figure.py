from scalability_figure import *


def display_time_division(filtered_lst, algorithm_name_lst, dataset_name):
    # shape_color_lst = ['w', 'w', 'w', 'w']
    hatch_lst = ['//', '||', '--']

    plt.figure(figsize=(8, 4))

    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        # plt.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color=shape_color_lst[cur_shape_color_idx],
        #         hatch=hatch_lst[cur_shape_color_idx], alpha=1)
        plt.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color='w',
                hatch=hatch_lst[cur_shape_color_idx], alpha=1)
        cur_shape_color_idx += 1

    plt.ylim([0.0, max(map(max, filtered_lst)) * 1.1])
    plt.xticks(range(1, len(algorithm_name_lst) + 1), algorithm_name_lst, rotation=30, fontsize=12)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.ylabel('Breakdown Time (s)', fontdict=font, color='black')
    plt.legend(['other', 'workload reduction computation', 'similarity evaluation'], fontsize=12)
    # plt.show()
    plt.savefig('../statistics/break-down-time-' + dataset_name + '.pdf', bbox_inches='tight', pad_inches=0,
                transparent=True)
    plt.close()


if __name__ == '__main__':
    with open('../statistics/breadown_time_gpu23.md') as ifs:
        all_data = map(lambda my_str:
                       map(float, list(reversed(my_str.split('|')[1:4]))),
                       filter(lambda line: 'SCAN' in line, ifs.readlines()))
        my_data_lst = [all_data[0:8], all_data[8:16], all_data[16:24]]
        dataset_name_lst = ['livejournal', 'orkut', 'twitter']

        for idx, lst_lst in enumerate(my_data_lst):
            lst_lst = [map(lambda my_tuple: my_tuple[0], lst_lst), map(lambda my_tuple: my_tuple[1], lst_lst),
                       map(lambda my_tuple: my_tuple[2], lst_lst)]
            display_time_division(recal_for_bar_char(lst_lst),
                                  ['SCAN\n$\\epsilon$: 0.2', 'SCAN\n$\\epsilon$: 0.4', 'SCAN\n$\\epsilon$: 0.6',
                                   'SCAN\n$\\epsilon$: 0.8', 'pSCAN\n$\\epsilon$: 0.2', 'pSCAN\n$\\epsilon$: 0.4',
                                   'pSCAN\n$\\epsilon$: 0.6', 'pSCAN\n$\\epsilon$: 0.8'],
                                  dataset_name=dataset_name_lst[idx])
