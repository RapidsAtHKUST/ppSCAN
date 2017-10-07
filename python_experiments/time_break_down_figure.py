from scalability_figure import *


def display_time_division(filtered_lst, algorithm_name_lst, dataset_name):
    shape_color_lst = ['w', 'w', 'w', 'w']
    hatch_lst = ['////', '----', '||||', '\\\\\\']

    plt.figure(figsize=(8, 4))

    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        plt.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color=shape_color_lst[cur_shape_color_idx],
                hatch=hatch_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    plt.ylim([0.0, max(map(max, filtered_lst)) * 1.1])
    plt.xticks(range(1, len(algorithm_name_lst) + 1), algorithm_name_lst, rotation=30, fontsize=12)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.ylabel('Breakdown Time (s)', fontdict=font, color='black')
    plt.legend(['other', 'workload reduction computation', 'similarity evaluation'], fontsize=12)
    # plt.show()
    plt.savefig('../statistics/break-down-time-' + dataset_name + '.png', bbox_inches='tight', pad_inches=0,
                transparent=True)
    plt.close()


if __name__ == '__main__':
    lst_lst = [[6.28, 5.24, 5.01, 7.94, 6.08, 4.33, 4.26, 4.26],
               [0.00, 0.68, 0.67, 4.13, 4.06, 0.70, 0.70, 0.70],
               [38.41, 20.17, 15.02, 17.79, 15.25, 14.64, 11.65, 11.65]]
    display_time_division(recal_for_bar_char(lst_lst),
                          ['SCAN\n$\\epsilon$: 0.2', 'SCAN\n$\\epsilon$: 0.4', 'SCAN\n$\\epsilon$: 0.6',
                           'SCAN\n$\\epsilon$: 0.8',
                           'pSCAN\n$\\epsilon$: 0.2', 'pSCAN\n$\\epsilon$: 0.4', 'pSCAN\n$\\epsilon$: 0.6',
                           'pSCAN\n$\\epsilon$: 0.8'], dataset_name='livejournal')
