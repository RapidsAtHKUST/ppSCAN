import os
import time

if __name__ == '__main__':
    # lfr_data_set_lst = ['n6000000_k40_C0dot3_Bin', 'n4000000_k60_C0dot3_Bin',
    #                     'n3000000_k80_C0dot3_Bin', 'n2400000_k100_C0dot3_Bin']
    roll_graph_data_set_lst = [
        # 'n50_m20_1billion_edge',
        'n25_m40_1billion_edge',
        'n16dot7_m60_1billion_edge', 'n12dot5_m80_1billion_edge']

    data_set_lst = roll_graph_data_set_lst
    # parameter_eps_lst = reversed([float(i + 1) / 10 for i in xrange(9)])
    parameter_eps_lst = [float(i + 1) / 10 for i in xrange(9)]
    parameter_min_pts_lst = [5]
    thread_num = 256
    data_set_lst = map(lambda name: os.pardir + os.sep + 'dataset' + os.sep + name, data_set_lst)

    print 'data set:', data_set_lst
    for data_set in data_set_lst:
        assert 0 == os.system('ls ' + data_set + '>' + 'tmp_ls_files.txt')
    print str(parameter_eps_lst)
    print parameter_min_pts_lst

    for data_set_path in data_set_lst:
        for eps in parameter_eps_lst:
            for min_pts in parameter_min_pts_lst:
                # statistics output dir and file
                data_set_name = data_set_path.split(os.sep)[-1]
                statistics_dir = os.sep.join(
                    map(str,
                        ['.', 'paper-roll-graph-exp-11-24-knl', data_set_name, 'eps-' + str(eps),
                         'min_pts-' + str(min_pts)]))
                os.system('mkdir -p ' + statistics_dir)
                statistics_file_path = statistics_dir + os.sep + '-'.join(
                    map(str, ['output', data_set_name, eps, min_pts])) + '.txt'

                # 1st: splitter, record start time
                my_splitter = '-'.join(['*' for _ in xrange(20)])
                os.system(' '.join(
                    ['echo', my_splitter + time.ctime() + my_splitter, '>>', statistics_file_path]))


                # 2nd: execute pscan+ with different parameters
                def write_split():
                    with open(statistics_file_path, 'a+') as ifs:
                        ifs.write(my_splitter + my_splitter + '\n')
                        ifs.write(my_splitter + my_splitter + '\n')


                # check md5, line count and byte count
                def check_result():
                    res_file_name = '-'.join(map(str, ['result', eps, min_pts])) + '.txt'
                    res_file_path = data_set_path + os.sep + res_file_name
                    os.system(' '.join(['md5sum', res_file_path, '>>', statistics_file_path]))
                    os.system(' '.join(['wc -l', res_file_path, '>>', statistics_file_path]))
                    os.system(' '.join(['du -b', res_file_path, '>>', statistics_file_path]))


                params_lst = map(str, ['../pSCAN-refactor/build/pSCANParallelExp0AVX512', data_set_path,
                                       eps, min_pts, thread_num, 'output', '>>', statistics_file_path])
                os.system(' '.join(params_lst))
                check_result()
                write_split()

                # params_lst = map(str, ['../pSCAN-refactor/build/pSCANSerial', data_set_path,
                #                        eps, min_pts, 'output', '>>', statistics_file_path])
                params_lst = map(str, ['../pSCAN-refactor/build/pSCANParallelExp0AVX512', data_set_path,
                                       eps, min_pts, 1, 'output', '>>', statistics_file_path])
                os.system(' '.join(params_lst))
                check_result()

                with open(statistics_file_path, 'a+') as ifs:
                    ifs.write(my_splitter + time.ctime() + my_splitter)
                    ifs.write('\n\n\n\n')
                print 'finish:', '-'.join(map(str, [data_set_path, eps, min_pts]))
