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
prune_time_tag = '1st: prune execution time'
first_bsp_time_tag = '2nd: check core first-phase bsp time'
second_bsp_time_tag = '2nd: check core second-phase bsp time'
core_cluster_time_tag = '3rd: core clustering time'
non_core_cluster_time_tag = '4th: non-core clustering time'


# should guarantee the format of the file, no empty body
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


def get_statistics(dataset, eps, min_pts, root_folder='.'):
    info_dict = dict()
    dir_path = os.sep.join([root_folder, 'scalability', dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts)])
    for root, dirs, files in os.walk(dir_path):
        for file in files:
            with open(dir_path + os.sep + file) as ifs:
                lines = ifs.readlines()
                thread_num = int(file.split('-')[-1][:-(len('.txt'))])
                info_dict[thread_num] = parse_lines(lines)
    return info_dict


# display 0: overall time cost in five phases
def display_overview(statistics_dic, title_append_txt=''):
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, total_time_tag]

    def post_process():
        sorted_info_lst = sorted(statistics_dic.items(), key=lambda pair: pair[0])

        thread_lst = map(lambda pair: pair[0], sorted_info_lst)
        time_info_lst = map(lambda pair: pair[1], sorted_info_lst)

        time_lst_lst = map(lambda tag: map(lambda my_dict: my_dict[tag], time_info_lst), tag_list)
        print 'thread list:', thread_lst
        print 'time info list:', time_lst_lst
        return thread_lst, time_lst_lst

    # draw after get data, with partial binding technique
    thread_lst, time_lst_lst = post_process()
    shape_color_lst = ['bo', 'g^', 'r*', 'ch', 'ys', 'mx']
    prev_partial_func = plt.plot
    cur_shape_color_idx = 0
    for time_lst in time_lst_lst:
        # partially bind parameters
        prev_partial_func = partial(prev_partial_func, thread_lst, time_lst, shape_color_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1
    prev_partial_func()

    plt.legend(tag_list)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Time cost overview\n' + title_append_txt if title_append_txt != '' else 'Time cost overview',
              fontdict=font)
    plt.xlabel('thread num', fontdict=font)
    plt.ylabel('time (ms)', fontdict=font)
    plt.show()


# display 1: total time and parallel part(hotspots)
def display_filtered_tags(statistics_dic, title_append_txt=''):
    filtered_tag_list = [total_time_tag, first_bsp_time_tag, second_bsp_time_tag, prune_time_tag]

    def post_process():
        sorted_info_lst = sorted(statistics_dic.items(), key=lambda pair: pair[0])

        thread_lst = map(lambda pair: pair[0], sorted_info_lst)
        time_info_lst = map(lambda pair: pair[1], sorted_info_lst)

        tag_time_lst_lst = map(lambda tag: (tag, map(lambda my_dict: my_dict[tag], time_info_lst)), filtered_tag_list)
        print 'thread list:', thread_lst
        print 'tag time info list:', tag_time_lst_lst
        return thread_lst, tag_time_lst_lst

    # init parameters
    thread_lst, tag_time_lst_lst = post_process()
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, total_time_tag]
    shape_color_lst = ['bo', 'g^', 'r*', 'ch', 'ys', 'mx']
    shape_color_dict = dict(zip(tag_list, shape_color_lst))

    prev_partial_func = plt.plot
    for tag, time_lst in tag_time_lst_lst:
        # partially bind parameters
        prev_partial_func = partial(prev_partial_func, thread_lst, time_lst, shape_color_dict[tag])
    prev_partial_func()

    # draw figures
    plt.legend(filtered_tag_list)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Total and hotspot time cost\n' +
              title_append_txt if title_append_txt != '' else 'Total and hotspot time cost\n', fontdict=font)
    plt.xlabel('thread num', fontdict=font)
    plt.ylabel('time (ms)', fontdict=font)
    plt.show()


if __name__ == '__main__':
    data_set_lst = ['small_snap_dblp',
                    'snap_livejournal', 'snap_orkut', 'snap_pokec',
                    '10million_avgdeg15_maxdeg50_Cdefault',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster']

    for data_set in data_set_lst:
        time_info_dict = get_statistics(data_set, 0.3, 5,
                                        root_folder='/mnt/mount-gpu/d2/yche/projects/python_experiments')
        if data_set == '10million_avgdeg15_maxdeg50_Cdefault':
            data_set = 'lfr_10million_avgdeg15'

        append_txt = ' - '.join([data_set, 'eps:' + str(0.3), 'min_pts:' + str(5)])
        # 1st: overall time cost
        display_overview(time_info_dict, append_txt)

        # 2nd: runtime for filtered tags
        display_filtered_tags(time_info_dict, append_txt)
