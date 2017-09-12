import matplotlib.pyplot as plt
import sys, os
from functools import partial
import workload_figure

# total time tag, and five separate time tag: first three parallel, last two serial
total_time_tag = 'Total time without IO'
prune_time_tag = '1st: prune execution time'
first_bsp_time_tag = '2nd: check core first-phase bsp time'
second_bsp_time_tag = '2nd: check core second-phase bsp time'
core_cluster_time_tag = '3rd: core clustering time'
non_core_cluster_time_tag = '4th: non-core clustering time'

pscan_runtime_tag = 'serial pscan runtime'


def parse_line(line):
    return int(line.split(':')[-1][:-len(' ms\n') + 1])


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


def get_statistics(dataset, eps, min_pts, figure_folder, root_folder='.'):
    info_dict = dict()
    dir_path = os.sep.join([root_folder, figure_folder, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts)])
    pscan_time = workload_figure.get_workload_statistics(dataset, eps, min_pts, root_folder + os.sep + 'worklaod')[
        workload_figure.runtime_tag + workload_figure.pscan_tag]

    for root, dirs, files in os.walk(dir_path):
        for file in files:
            with open(dir_path + os.sep + file) as ifs:
                lines = ifs.readlines()
                thread_num = int(file.split('-')[-1][:-(len('.txt'))])
                info_dict[thread_num] = parse_lines(lines)
                info_dict[thread_num][pscan_runtime_tag] = pscan_time
    return info_dict


def recal_for_bar_char(filtered_lst):
    for idx in xrange(len(filtered_lst)):
        for idx2 in xrange(idx + 1, len(filtered_lst)):
            filtered_lst[idx] = map(lambda pair: sum(pair), zip(filtered_lst[idx], filtered_lst[idx2]))
    return filtered_lst


# display 0: overall time cost in five phases
def display_overview(statistics_dic, title_append_txt='', figure_folder='.'):
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, pscan_runtime_tag]

    def post_process():
        sorted_info_lst = sorted(statistics_dic.items(), key=lambda pair: pair[0])

        thread_lst = map(lambda pair: pair[0], sorted_info_lst)
        time_info_lst = map(lambda pair: pair[1], sorted_info_lst)

        time_lst_lst = map(lambda tag: map(lambda my_dict: my_dict[tag], time_info_lst), tag_list)
        print 'thread list:', thread_lst
        new_lst = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                   non_core_cluster_time_tag, total_time_tag]

        print 'time info list:', map(lambda tag: map(lambda my_dict: my_dict[tag], time_info_lst), new_lst)
        return thread_lst, time_lst_lst

    # draw after get data, with partial binding technique
    thread_lst, time_lst_lst = post_process()

    # bar charts
    shape_color_lst = ['b', 'g', 'r', 'c', 'y', 'm']
    hatch_lst = ['', '\\\\\\', '////', '----', '||||']
    cur_shape_color_idx = 0

    filtered_lst = recal_for_bar_char(time_lst_lst[:-1])
    plt.plot(thread_lst, map(lambda integer: float(integer) / 1000, time_lst_lst[-1]), 'k--')
    for time_lst in filtered_lst:
        plt.bar(thread_lst, map(lambda integer: float(integer) / 1000, time_lst),
                color=shape_color_lst[cur_shape_color_idx], hatch=hatch_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    plt.legend([tag_list[-1]] + tag_list[:-1])

    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Time cost overview\n' + title_append_txt if title_append_txt != '' else 'Time cost overview',
              fontdict=font)
    plt.xlabel('thread num', fontdict=font)
    plt.ylabel('time (s)', fontdict=font)

    plt.savefig('./figures/' + figure_folder + os.sep + title_append_txt.replace(' ', '') + '-' + 'overview.png',
                bbox_inches='tight', pad_inches=0, transparent=True)
    # plt.show()
    plt.close()


# display 1: total time and parallel part(hotspots)
def display_filtered_tags(statistics_dic, title_append_txt='', figure_folder='.'):
    filtered_tag_list = [total_time_tag, first_bsp_time_tag, second_bsp_time_tag, prune_time_tag]

    def post_process():
        sorted_info_lst = sorted(statistics_dic.items(), key=lambda pair: pair[0])

        thread_lst = map(lambda pair: pair[0], sorted_info_lst)
        time_info_lst = map(lambda pair: pair[1], sorted_info_lst)

        tag_time_lst_lst = map(lambda tag: (tag, map(lambda my_dict: my_dict[tag], time_info_lst)), filtered_tag_list)
        return thread_lst, tag_time_lst_lst

    # init parameters
    thread_lst, tag_time_lst_lst = post_process()
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, total_time_tag]
    shape_color_lst = ['bo-', 'g^-', 'rs-', 'c<-', 'y>-', 'mx-']
    shape_color_dict = dict(zip(tag_list, shape_color_lst))

    # 1st: draw runtime
    prev_partial_func = plt.plot
    for tag, time_lst in tag_time_lst_lst:
        # partially bind parameters
        prev_partial_func = partial(prev_partial_func, thread_lst, map(lambda time: float(time) / 1000, time_lst),
                                    shape_color_dict[tag])
    plt.subplot(211)
    prev_partial_func()

    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Total and hotspot time cost/speedup\n' +
              title_append_txt if title_append_txt != '' else 'Total and hotspot time cost\n', fontdict=font)
    plt.legend(filtered_tag_list)
    plt.ylabel('time (s)', fontdict=font)

    # 2nd: draw speedup
    tag_speedup_lst = map(lambda tag_time_lst_pair: (
        tag_time_lst_pair[0], map(lambda ele: max(tag_time_lst_pair[1]) / float(ele), tag_time_lst_pair[1])),
                          tag_time_lst_lst)
    prev_partial_func = plt.plot
    for tag, speedup_lst in tag_speedup_lst:
        # partially bind parameters
        prev_partial_func = partial(prev_partial_func, thread_lst, speedup_lst, shape_color_dict[tag])
    plt.subplot(212)
    prev_partial_func()

    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.xlabel('thread num', fontdict=font)
    plt.ylabel('speedup', fontdict=font)
    plt.ylim([0, 28.5])

    # show the whole runtime/speedup figure
    plt.savefig(
        './figures/' + figure_folder + os.sep + title_append_txt.replace(' ', '') + '-' + 'runtime-speedup.png',
        bbox_inches='tight', pad_inches=0, transparent=True)
    # plt.show()
    plt.close()


if __name__ == '__main__':
    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster',
                    '10million_avgdeg15_maxdeg50_Cdefault']

    figure_folder = 'scalability_new0'
    os.system('mkdir -p ./figures/' + figure_folder)

    for data_set in data_set_lst:
        time_info_dict = get_statistics(data_set, 0.3, 5, figure_folder=figure_folder,
                                        root_folder='/mnt/mount-gpu/d2/yche/projects/python_experiments')
        if data_set == '10million_avgdeg15_maxdeg50_Cdefault':
            data_set = 'lfr_10million_avgdeg15'

        append_txt = ' - '.join([data_set, 'eps:' + str(0.3), 'min_pts:' + str(5)])
        # 1st: overall time cost
        display_overview(time_info_dict, append_txt, figure_folder)

        # 2nd: runtime/speedup for filtered tags
        display_filtered_tags(time_info_dict, append_txt, figure_folder)
