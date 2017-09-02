import decimal

from scalability_figure import *


def format_str(float_num):
    return str(decimal.Decimal.from_float(float_num).quantize(decimal.Decimal('0.000')))


def display_speedup(edge_num_portion_lst, pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                    data_set_name_lst, title_append_txt=''):
    # init parameters
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title(
        'Computation speedup over different datasets\n' + title_append_txt if title_append_txt != ''
        else 'Computation speedup over different datasets', fontdict=font)

    #  draw speedup
    psca_plus_speedup = map(lambda pair: pair[0] / pair[1],
                            zip(pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst))
    plt.bar(edge_num_portion_lst, psca_plus_speedup, color='m')

    plt.xlabel('real-world data set', fontdict=font)
    plt.xticks(edge_num_portion_lst, data_set_name_lst, rotation=20)
    plt.ylabel('speedup', fontdict=font)
    plt.legend(['parallel pscan+ speedup over serial pscan'])

    plt.ylim([0.0, 25.0])
    plt.savefig('./figures/' + overview_figure_folder + os.sep + title_append_txt.replace(' ', '')
                + '-' + 'runtime-speedup.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


def illustrate_speedup(pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst, pscan_plus_best_parallel_runtime_lst,
                       best_thread_lst):
    data_set_name_lst = ['dblp', 'pokec', 'livejournal', 'orkut', 'uk', 'webbase', 'twitter', 'friendster']
    edge_num_lst = ['2,099,732', '30,282,866', '69,362,378', '234,370,166', '301,136,554', '1,050,026,736',
                    '1,369,000,750', '3,612,134,270']

    x_axis_range = range(1, len(best_thread_lst) + 1)
    # 1st: display speedup
    append_txt = ' - '.join(['eps:0.3', 'min_pts:5', 'with all logical cores'])

    display_speedup(x_axis_range, pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                    data_set_name_lst, append_txt)

    append_txt = ' - '.join(['eps:0.3', 'min_pts:5', 'with best logical thread num'])

    # 2nd: data for generate markdown
    display_speedup(x_axis_range, pscan_serial_runtime_lst, pscan_plus_best_parallel_runtime_lst,
                    data_set_name_lst, append_txt)
    speedup_full_lst = map(lambda pair: format_str(pair[0] / pair[1]) + "",
                           zip(pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst))
    speedup_best_lst = map(lambda pair: format_str(pair[0] / pair[1]),
                           zip(pscan_serial_runtime_lst, pscan_plus_best_parallel_runtime_lst))
    pscan_serial_runtime_lst = map(lambda num: str(num) + 's', pscan_serial_runtime_lst)

    print 'dataset lst:', data_set_name_lst, '\n', 'edge_num lst:', edge_num_lst
    print 'pscan serial runtime lst:', pscan_serial_runtime_lst
    print 'speedup lst 40-core:', speedup_full_lst
    print 'speedup lst best thread num:', speedup_best_lst
    print 'best performance thread_num_lst:', best_thread_lst, '\n'

    for idx in xrange(len(best_thread_lst)):
        row = [data_set_name_lst[idx], edge_num_lst[idx], pscan_serial_runtime_lst[idx], speedup_full_lst[idx],
               speedup_best_lst[idx], best_thread_lst[idx]]
        print ' | '.join(map(str, row))


def to_grouped_statistics(statistics):
    """
    :type statistics: dict
    """
    return [statistics[total_time_tag], statistics[prune_time_tag] + statistics[first_bsp_time_tag] + statistics[
        second_bsp_time_tag], statistics[core_cluster_time_tag] + statistics[non_core_cluster_time_tag]]


def to_portion_lst(info_lst, input_time_lst, output_time_lst):
    zipped_lst = map(lambda my_tuple: my_tuple[0] + [my_tuple[1], my_tuple[2]],
                     zip(info_lst, input_time_lst, output_time_lst))
    zipped_lst = map(lambda lst: map(lambda ele: float(ele) / lst[0], lst), zipped_lst)

    def transpose():
        return map(lambda idx: map(lambda lst: lst[idx], zipped_lst), range(len(zipped_lst[0])))

    return transpose()


def display_comp_io_portion(portion_lst_lst, data_set_name_lst, title_append_txt=''):
    # init parameters
    filtered_lst = recal_for_bar_char([portion_lst_lst[3], portion_lst_lst[4], portion_lst_lst[1], portion_lst_lst[2]])

    shape_color_lst = ['y', 'r', 'g', 'c']
    hatch_lst = ['\\\\\\', '////', '----', '||||']

    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title(
        'Computation and I/O portition over different datasets\n' + title_append_txt if title_append_txt != ''
        else 'Computation and I/O portition over different datasets', fontdict=font)

    cur_shape_color_idx = 0
    for portion_lst in filtered_lst:
        plt.bar(range(1, len(data_set_name_lst) + 1), portion_lst, color=shape_color_lst[cur_shape_color_idx],
                hatch=hatch_lst[cur_shape_color_idx])
        cur_shape_color_idx += 1

    plt.xlabel('real-world data set', fontdict=font)
    plt.ylim([0.0, 10.0])
    plt.xticks(range(1, len(data_set_name_lst) + 1), data_set_name_lst, rotation=20)
    plt.ylabel('portion', fontdict=font)
    plt.legend(['input time', 'output time', 'parallel part time', 'serial part time'])

    plt.savefig('./figures/' + overview_figure_folder + os.sep + title_append_txt.replace(' ', '')
                + '-' + 'comp-io-portion.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


def illustrate_comp_io_portion(input_time_lst, output_time_lst):
    best_thread_lst = []
    best_info_lst = []
    full_core_info_lst = []

    root_folder = '/mnt/mount-gpu/d2/yche/projects/python_experiments'

    for data_set in data_set_lst:
        eps = 0.3
        min_pts = 5
        time_info_dict = get_statistics(data_set, eps, min_pts, root_folder=root_folder)

        best_obj = sorted(time_info_dict.iteritems(), key=lambda pair: pair[1][total_time_tag])[0]
        best_thread_lst.append(best_obj[0])
        print data_set
        print best_obj[1], '\n', to_grouped_statistics(best_obj[1])
        best_info_lst.append(to_grouped_statistics(best_obj[1]))

        print time_info_dict[40], '\n', to_grouped_statistics(time_info_dict[40]), '\n'
        full_core_info_lst.append(to_grouped_statistics(time_info_dict[40]))

    print 'best_thread_lst:', best_thread_lst, '\n', 'best_info_lst:', best_info_lst
    print 'full_core_info_lst:', full_core_info_lst
    print 'input_time_lst:', input_time_lst, '\n', 'output_time_lst:', output_time_lst

    append_txt = ' - '.join(['eps:0.3', 'min_pts:5', 'with all logical cores'])
    zipped_lst = to_portion_lst(full_core_info_lst, input_time_lst, output_time_lst)
    display_comp_io_portion(zipped_lst, map(lambda my_str: my_str.split('_')[-1], data_set_lst), append_txt)

    append_txt = ' - '.join(['eps:0.3', 'min_pts:5', 'with best logical thread num'])
    zipped_lst = to_portion_lst(best_info_lst, input_time_lst, output_time_lst)
    display_comp_io_portion(zipped_lst, map(lambda my_str: my_str.split('_')[-1], data_set_lst), append_txt)

    def to_float_str(time_val_lst):
        return eval(format_str(float(time_val_lst[0]) / 1000))

    return best_thread_lst, map(to_float_str, best_info_lst), map(to_float_str, full_core_info_lst)


if __name__ == '__main__':
    overview_figure_folder = 'scalability_overview_robust'
    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster']
    os.system('mkdir -p ./figures/' + overview_figure_folder)

    thread_lst, best_thread_time_lst, full_core_time_lst = \
        illustrate_comp_io_portion(input_time_lst=[217, 1522, 3453, 9720, 13254, 45982, 47910, 215101],
                                   output_time_lst=[298, 325, 2468, 1169, 11487, 62141, 3007, 9967])
    serial_time_lst = [0.5550, 8.5970, 21.8460, 164.2480, 18.4980, 63.7050, 2487.3170, 3726.3020]

    # case study 2
    illustrate_speedup(
        pscan_serial_runtime_lst=serial_time_lst,
        pscan_plus_parallel_runtime_lst=best_thread_time_lst,
        pscan_plus_best_parallel_runtime_lst=full_core_time_lst,
        best_thread_lst=thread_lst)
