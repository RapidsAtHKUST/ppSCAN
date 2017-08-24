import os
import matplotlib.pyplot as plt
import decimal


def format_str(float_num):
    return str(decimal.Decimal.from_float(float_num).quantize(decimal.Decimal('0.000')))


def display_filtered_tags(edge_num_portion_lst, pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                          data_set_name_lst, title_append_txt=''):
    # init parameters
    shape_color_lst = ['k--', 'mx']
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title(
        'Computation speedup over different datasets\n' + title_append_txt if title_append_txt != ''
        else 'Computation speedup over different datasets', fontdict=font)

    #  draw speedup
    pscan_speedup = [1 for _ in xrange(len(edge_num_portion_lst))]
    psca_plus_speedup = map(lambda pair: pair[0] / pair[1],
                            zip(pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst))
    plt.plot(edge_num_portion_lst, pscan_speedup, shape_color_lst[0])
    plt.plot(edge_num_portion_lst, psca_plus_speedup, shape_color_lst[1])
    plt.xlabel('real-world data set', fontdict=font)
    plt.xticks(edge_num_portion_lst, data_set_name_lst, rotation=20)
    plt.ylabel('speedup', fontdict=font)
    plt.legend(['pscan serial', 'pscan+ parallel'])

    # show the whole runtime/speedup figure
    plt.savefig('./figures/scalability_overview' + os.sep + title_append_txt.replace(' ', '')
                + '-' + 'runtime-speedup.png', bbox_inches='tight', pad_inches=0, transparent=True)
    plt.show()


if __name__ == '__main__':
    data_set_name_lst = ['dblp', 'pokec', 'livejournal', 'orkut', 'uk', 'webbase', 'twitter', 'friendster']
    edge_num_lst = ['2,099,732', '30,282,866', '69,362,378', '234,370,166', '301,136,554', '1,050,026,736',
                    '1,369,000,750', '3,612,134,270']
    # edge_num_lst = map(lambda my_str: int(my_str.replace(',', '')), edge_num_lst)

    pscan_serial_runtime_lst = [0.5550, 8.5970, 21.8460, 164.2480, 18.4980, 63.7050, 2487.3170, 3726.3020]
    pscan_plus_parallel_runtime_lst = [0.209, 0.931, 2.285, 9.756, 3.879, 26.854, 129.427, 168.866]

    os.system('mkdir -p ./figures/scalability_overview')

    append_txt = ' - '.join(['eps:0.3', 'min_pts:5', 'with all logical cores'])

    display_filtered_tags(range(1, 9), pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst,
                          data_set_name_lst, append_txt)

    append_txt = ' - '.join(['eps:0.3', 'min_pts:5', 'with best logical thread num'])

    pscan_plus_best_parallel_runtime_lst = [0.133, 0.878, 2.285, 9.756, 3.879, 13.753, 129.427, 168.866]
    display_filtered_tags(range(1, 9), pscan_serial_runtime_lst, pscan_plus_best_parallel_runtime_lst,
                          data_set_name_lst, append_txt)

    print 'dataset lst:', data_set_name_lst
    print 'edge_num lst:', edge_num_lst

    speedup_full_lst = map(lambda pair: format_str(pair[0] / pair[1]) + "",
                           zip(pscan_serial_runtime_lst, pscan_plus_parallel_runtime_lst))
    speedup_best_lst = map(lambda pair: format_str(pair[0] / pair[1]),
                           zip(pscan_serial_runtime_lst, pscan_plus_best_parallel_runtime_lst))
    print 'speedup lst 40-core:', speedup_full_lst

    print 'speedup lst best thread num:', speedup_best_lst

    pscan_serial_runtime_lst = map(lambda num: str(num) + 's', pscan_serial_runtime_lst)
    print 'pscan serial runtime lst:', pscan_serial_runtime_lst

    best_thread_lst = [16, 32, 40, 40, 40, 32, 40, 40]
    print 'best performance thread_num_lst:', best_thread_lst

    print
    for idx in xrange(len(best_thread_lst)):
        row = [data_set_name_lst[idx], edge_num_lst[idx], pscan_serial_runtime_lst[idx], speedup_full_lst[idx],
               speedup_best_lst[idx], best_thread_lst[idx]]

        print ' | '.join(map(str,row))
