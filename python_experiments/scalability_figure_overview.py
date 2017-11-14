import decimal

from scalability_figure import *


def format_str(float_num):
    return str(decimal.Decimal.from_float(float_num).quantize(decimal.Decimal('0.000')))


def ms_val_to_s(num):
    return eval(str(decimal.Decimal.from_float(float(num) / 1000).quantize(decimal.Decimal('0.0000'))))


def to_grouped_statistics(statistics):
    """
    :type statistics: dict
    """
    return [statistics[total_time_tag], statistics[prune_time_tag] + statistics[first_bsp_time_tag] + statistics[
        second_bsp_time_tag], statistics[core_cluster_time_tag] + statistics[non_core_cluster_time_tag]]


def to_portion_lst(info_lst, input_time_lst, output_time_lst):
    zipped_lst = map(lambda my_tuple: my_tuple[0] + [my_tuple[1], my_tuple[2]],
                     zip(info_lst, input_time_lst, output_time_lst))
    zipped_lst = map(lambda lst: map(lambda ele: 100.0 * float(ele) / sum(lst[1:]), lst), zipped_lst)
    print zipped_lst

    def transpose():
        return map(lambda idx: map(lambda lst: lst[idx], zipped_lst), range(len(zipped_lst[0])))

    return transpose()


class ScalabilityFigureDrawer:
    def get_output_time(self, data_set, eps, min_pts):
        output_time_tag = 'Total output cost'
        file_path = os.sep.join([self.root_dir_path, data_set, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                                 '-'.join(['output', data_set, str(eps), str(min_pts), '40']) + '.txt'])
        with open(file_path) as ifs:
            lines = ifs.readlines()
        outputs = workload_figure.filter_lines_by_tag(output_time_tag, lines)
        return outputs[-1]

    def __init__(self, data_set_lst, input_time_lst, edge_num_lst, server_root_dir_path, overview_folder):
        self.data_set_lst = data_set_lst
        self.input_time_lst = input_time_lst
        self.root_dir_path = server_root_dir_path
        self.edge_num_lst = edge_num_lst
        self.overview_figure_folder = overview_folder
        self.root_folder = '/home/yche/mnt/gpu-23/projects/python_experiments/'

        os.system('mkdir -p ./figures/' + self.overview_figure_folder)

    def display_comp_io_portion(self, portion_lst_lst, data_set_name_lst, title_append_txt=''):
        filtered_lst = recal_for_bar_char(
            [portion_lst_lst[3], portion_lst_lst[4], portion_lst_lst[1], portion_lst_lst[2]])

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
        plt.ylim([0.0, 100.0])
        plt.xticks(range(1, len(data_set_name_lst) + 1), data_set_name_lst, rotation=20)
        plt.ylabel('portion', fontdict=font)
        plt.legend(['input time', 'output time', 'parallel part time', 'serial part time'])

        plt.savefig('./figures/' + self.overview_figure_folder + os.sep + title_append_txt.replace(' ', '')
                    + '-' + 'comp-io-portion.png', bbox_inches='tight', pad_inches=0, transparent=True)
        # plt.show()
        plt.close()

    def illustrate_comp_io_portion(self, output_time_lst, eps, min_pts):
        best_thread_lst = []
        best_info_lst = []
        full_core_info_lst = []

        for data_set in self.data_set_lst:
            time_info_dict = get_statistics(data_set, eps, min_pts, figure_folder=self.overview_figure_folder,
                                            root_folder=self.root_folder)

            best_obj = sorted(time_info_dict.iteritems(), key=lambda pair: pair[1][total_time_tag])[0]
            best_thread_lst.append(best_obj[0])
            best_info_lst.append(to_grouped_statistics(best_obj[1]))

            full_core_info_lst.append(to_grouped_statistics(time_info_dict[40]))

        append_txt = ' - '.join(['eps:' + str(eps), 'min_pts:' + str(min_pts), 'with all logical cores'])
        zipped_lst = to_portion_lst(full_core_info_lst, self.input_time_lst, output_time_lst)
        self.display_comp_io_portion(zipped_lst, map(lambda my_str: my_str.split('_')[-1], self.data_set_lst),
                                     append_txt)

        append_txt = ' - '.join(['eps:' + str(eps), 'min_pts:' + str(min_pts), 'with best logical thread num'])
        zipped_lst = to_portion_lst(best_info_lst, self.input_time_lst, output_time_lst)
        self.display_comp_io_portion(zipped_lst, map(lambda my_str: my_str.split('_')[-1], self.data_set_lst),
                                     append_txt)

        def to_float_str(time_val_lst):
            return eval(format_str(float(time_val_lst[0]) / 1000))

        return best_thread_lst, map(to_float_str, best_info_lst), map(to_float_str, full_core_info_lst)

    def display_speedup(self, edge_num_portion_lst, pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                        data_set_name_lst, title_append_txt=''):
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

        plt.ylim([0.0, 40.0])
        plt.savefig('./figures/' + self.overview_figure_folder + os.sep + title_append_txt.replace(' ', '')
                    + '-' + 'runtime-speedup.png', bbox_inches='tight', pad_inches=0, transparent=True)
        # plt.show()
        plt.close()

    def illustrate_speedup(self, pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                           pscan_plus_best_parallel_runtime_lst,
                           best_thread_lst, eps, min_pts):
        x_axis_range = range(1, len(best_thread_lst) + 1)
        # 1st: display speedup
        append_txt = ' - '.join(['eps:' + str(eps), 'min_pts:' + str(min_pts), 'with all logical cores'])

        self.display_speedup(x_axis_range, pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                             self.data_set_lst, append_txt)

        append_txt = ' - '.join(['eps:' + str(eps), 'min_pts:' + str(min_pts), 'with best logical thread num'])

        # 2nd: data for generate markdown
        self.display_speedup(x_axis_range, pscan_serial_runtime_lst, pscan_plus_best_parallel_runtime_lst,
                             self.data_set_lst, append_txt)
        speedup_full_lst = map(lambda pair: format_str(pair[0] / pair[1]) + "",
                               zip(pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst))
        speedup_best_lst = map(lambda pair: format_str(pair[0] / pair[1]),
                               zip(pscan_serial_runtime_lst, pscan_plus_best_parallel_runtime_lst))
        pscan_serial_runtime_lst = map(lambda num: str(num) + 's', pscan_serial_runtime_lst)

        print 'best performance thread_num_lst:', best_thread_lst, '\n'

        rows = []
        for idx in xrange(len(best_thread_lst)):
            row = [self.data_set_lst[idx], self.edge_num_lst[idx], pscan_serial_runtime_lst[idx], speedup_full_lst[idx],
                   speedup_best_lst[idx], best_thread_lst[idx]]
            rows.append(' | '.join(map(str, row)))
        print '\n'.join(rows)
        return rows

    def draw_overview_figure(self, eps=0.3, min_pts=5):
        serial_time_lst = []
        output_time_lst = []
        for dataset in self.data_set_lst:
            pscan_time = \
                workload_figure.get_workload_statistics(dataset, eps, min_pts, self.root_folder + os.sep + 'worklaod')[
                    workload_figure.runtime_tag + workload_figure.pscan_tag]
            serial_time_lst.append(pscan_time)
            output_time_lst.append(self.get_output_time(data_set=dataset, eps=eps, min_pts=min_pts))
        serial_time_lst = map(ms_val_to_s, serial_time_lst)
        print 'serial runtime lst:', serial_time_lst
        print 'output time lst:', output_time_lst

        thread_lst, best_thread_time_lst, full_core_time_lst = \
            self.illustrate_comp_io_portion(
                output_time_lst=output_time_lst, eps=eps, min_pts=min_pts)
        rows = self.illustrate_speedup(pscan_serial_runtime_lst=serial_time_lst,
                                       pscan_plus_parallel_runtime_lst=full_core_time_lst,
                                       pscan_plus_best_parallel_runtime_lst=best_thread_time_lst,
                                       best_thread_lst=thread_lst,
                                       eps=eps, min_pts=min_pts)
        return thread_lst, rows


if __name__ == '__main__':
    all_data_set_lst = [
        'small_snap_dblp', 'snap_pokec', 'snap_livejournal', 'snap_orkut',
        'webgraph_uk', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
    all_input_time_lst = [217, 1522, 3453, 9720, 13254, 45982, 47910, 215101]
    all_edge_num_lst = ['2,099,732', '30,282,866', '69,362,378', '234,370,166', '301,136,554', '1,050,026,736',
                        '1,369,000,750', '3,612,134,270']

    drawer = ScalabilityFigureDrawer(data_set_lst=all_data_set_lst[1:],
                                     input_time_lst=all_input_time_lst[1:],
                                     edge_num_lst=all_edge_num_lst[1:],
                                     server_root_dir_path='/home/yche/mnt/gpu-23/projects/python_experiments'
                                                          '/scalability_input_output',
                                     overview_folder='scalability_new0')
    drawer.draw_overview_figure()
