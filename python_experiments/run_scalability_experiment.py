import os
import time


def scalability_exp(data_set_lst, parameter_eps_lst, parameter_min_pts_lst, thread_num_lst, folder_name):
    data_set_lst = map(lambda name: os.pardir + os.sep + 'dataset' + os.sep + name, data_set_lst)

    print 'data set:', data_set_lst
    for data_set in data_set_lst:
        assert 0 == os.system('ls ' + data_set + '>' + 'tmp_ls_files.txt')
    print parameter_eps_lst
    print parameter_min_pts_lst
    print thread_num_lst

    for data_set_path in data_set_lst:
        for eps in parameter_eps_lst:
            for min_pts in parameter_min_pts_lst:
                for thread_num in thread_num_lst:
                    # statistics output dir and file
                    data_set_name = data_set_path.split(os.sep)[-1]
                    statistics_dir = os.sep.join(
                        map(str,
                            ['.', folder_name, data_set_name, 'eps-' + str(eps), 'min_pts-' + str(min_pts)]))
                    os.system('mkdir -p ' + statistics_dir)
                    statistics_file_path = statistics_dir + os.sep + '-'.join(
                        map(str, ['output', data_set_name, eps, min_pts, thread_num])) + '.txt'

                    # 1st: splitter, record start time
                    my_splitter = '-'.join(['*' for _ in xrange(20)])
                    os.system(' '.join(
                        ['echo', my_splitter + time.ctime() + my_splitter, '>>', statistics_file_path]))

                    # 2nd: execute pscan+ with different parameters
                    params_lst = map(str, ['../pSCAN-refactor/build/pSCANParallelExp0', data_set_path,
                                           eps, min_pts, thread_num, 'output', '>>', statistics_file_path])
                    my_cmd = ' '.join(params_lst)
                    os.system(my_cmd)

                    # 3rd: check md5, line count and byte count
                    res_file_name = '-'.join(map(str, ['result', eps, min_pts])) + '.txt'
                    res_file_path = data_set_path + os.sep + res_file_name
                    os.system(' '.join(['md5sum', res_file_path, '>>', statistics_file_path]))
                    os.system(' '.join(['wc -l', res_file_path, '>>', statistics_file_path]))
                    os.system(' '.join(['du -b', res_file_path, '>>', statistics_file_path]))

                    with open(statistics_file_path, 'a+') as ifs:
                        ifs.write(my_splitter + time.ctime() + my_splitter)
                        ifs.write('\n\n\n\n')
                    print 'finish:', '-'.join(map(str, [data_set_path, eps, min_pts, thread_num]))


if __name__ == '__main__':
    # parameters
    data_set_lst = ['small_snap_dblp',
                    'snap_livejournal', 'snap_orkut', 'snap_pokec',
                    'lfr_benchmark/10million_avgdeg15_maxdeg50_Cdefault',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster']
    parameter_eps_lst = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    # parameter_eps_lst = [0.3]
    parameter_min_pts_lst = [5]
    # thread_num_lst = [1, 2, 4, 8, 16, 24, 32, 40]
    thread_num_lst = [1, 4, 8, 16, 24, 32, 40]

    # loop run experiments
    loop_count = 1
    for i in xrange(loop_count):
        scalability_exp(data_set_lst=data_set_lst, parameter_eps_lst=parameter_eps_lst,
                        parameter_min_pts_lst=parameter_min_pts_lst, thread_num_lst=thread_num_lst,
                        folder_name='scalability_new4_all_in_parallel_deg_scheduler')
