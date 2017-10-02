from scalability_figure import *


def display_time_division(filtered_lst, algorithm_name_lst):
    shape_color_lst = ['w', 'w', 'w', 'w']
    hatch_lst = ['////', '----', '||||', '\\\\\\']

    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    # plt.title(
    #     'Computation and I/O portition over different datasets\n' + title_append_txt if title_append_txt != ''
    #     else 'Computation and I/O portition over different datasets', fontdict=font)

    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        plt.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color=shape_color_lst[cur_shape_color_idx],
                hatch=hatch_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    plt.ylim([0.0, 45 * 1.1])
    plt.xticks(range(1, len(algorithm_name_lst) + 1), algorithm_name_lst, rotation=20)
    plt.ylabel('breakdown time (s)', fontdict=font, color='black')
    plt.legend(['other', 'workload reduction computation', 'similarity evaluation'])

    plt.savefig('../statistics/break-down-time.png', bbox_inches='tight', pad_inches=0, transparent=True)
    # plt.show()
    plt.close()


def display_instruction(filtered_lst, algorithm_name_lst):
    shape_color_lst = ['w', 'w', 'w', 'w']
    hatch_lst = ['////', '----', '||||', '\\\\\\']

    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    # plt.title(
    #     'Computation and I/O portition over different datasets\n' + title_append_txt if title_append_txt != ''
    #     else 'Computation and I/O portition over different datasets', fontdict=font)

    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        print portion_lst
        plt.bar(range(1, len(algorithm_name_lst) + 1), portion_lst, color=shape_color_lst[cur_shape_color_idx],
                hatch=hatch_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    plt.ylim([0.0, 45 * 1.1])
    plt.xticks(range(1, len(algorithm_name_lst) + 1), algorithm_name_lst, rotation=20)
    plt.ylabel('breakdown time (s)', fontdict=font)
    plt.legend(['other', 'workload reduction computation', 'similarity evaluation'])

    # plt.savefig('../statistics/break-down-time.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()
    # plt.close()


if __name__ == '__main__':
    lst_lst = [
        [6.28, 5.24, 5.01, 7.94, 6.08, 4.33, 4.26],
        [0.00, 0.68, 0.67, 4.13, 4.06, 0.70, 0.70],
        [38.41, 20.17, 15.02, 17.79, 15.25, 14.64, 11.65]]
    display_time_division(recal_for_bar_char(lst_lst),
                          ['SCAN0', 'SCAN1', 'SCAN2', 'pSCAN0', 'pSCAN1', 'ppSCAN0', 'ppSCAN1'])
