import matplotlib.pyplot as plt
import sys
import os
from functools import partial


def play0():
    plt.plot([1, 2, 4, 8, 16, 24, 32, 40], [319, 131, 86, 74, 68, 81, 73, 95], '*y')
    plt.legend(['total time'])
    plt.show()


def parse_line(line):
    return int(line.split(':')[-1][:-len(' ms\n') + 1])


# total time tag, and five separate time tag: first three parallel, last two serial
total_time_tag = 'Total time without IO'
prune_time_tag = 'prune execution time'
first_bsp_time_tag = 'first-phase bsp time'
second_bsp_time_tag = 'second-phase bsp time'
core_cluster_time_tag = 'core clustering time'
non_core_cluster_time_tag = 'non-core clustering time'


def parse_lines(lines):
    total_time_lst = filter(lambda ele: total_time_tag in ele, lines)
    time_int_lst = map(parse_line, total_time_lst)

    def find_min_ele_idx(ele_lst):
        cur_min = sys.maxint
        cur_min_idx = sys.maxint
        for idx, val in enumerate(ele_lst):
            if val < cur_min:
                cur_min = val
                cur_min_idx = idx
        return cur_min_idx

    cur_min_idx = find_min_ele_idx(time_int_lst)
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag]
    my_dict = dict(map(lambda tag: (tag, parse_line(filter(lambda ele: tag in ele, lines)[cur_min_idx])), tag_list))
    my_dict[total_time_tag] = time_int_lst[cur_min_idx]
    return my_dict


def get_statistics(dataset, eps, min_pts):
    info_dict = dict()
    dir_path = os.sep.join(['.', 'scalability', dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts)])
    for root, dirs, files in os.walk(dir_path):
        for file in files:
            with open(dir_path + os.sep + file) as ifs:
                lines = ifs.readlines()
                thread_num = int(file.split('-')[-1][:-(len('.txt'))])
                info_dict[thread_num] = parse_lines(lines)
    return info_dict


if __name__ == '__main__':
    time_info_dict = get_statistics('small_snap_dblp', 0.3, 5)
    sorted_info_lst = sorted(time_info_dict.items(), key=lambda pair: pair[0])

    thread_lst = map(lambda pair: pair[0], sorted_info_lst)
    time_info_lst = map(lambda pair: pair[1], sorted_info_lst)
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, total_time_tag]
    time_lst_lst = map(lambda tag: map(lambda my_dict: my_dict[tag], time_info_lst), tag_list)
    print thread_lst
    print time_lst_lst

    shape_color_lst = ['bo', 'g^', 'r*', 'ch', 'm+', 'yx']
    prev_partial_func = plt.plot
    cur_shape_color_idx = 0
    for time_lst in time_lst_lst:
        prev_partial_func = partial(prev_partial_func, thread_lst, time_lst, shape_color_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1
    prev_partial_func()
    plt.legend(tag_list)
    plt.show()
