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
                    params_lst = map(str, ['../pSCAN-refactor/build/pSCANParallelExp1', data_set_path,
                                           eps, min_pts, thread_num, '>>', statistics_file_path])
                    my_cmd = ' '.join(params_lst)
                    os.system(my_cmd)

                    with open(statistics_file_path, 'a+') as ifs:
                        ifs.write(my_splitter + time.ctime() + my_splitter)
                        ifs.write('\n\n\n\n')
                    print 'finish:', '-'.join(map(str, [data_set_path, eps, min_pts, thread_num]))


if __name__ == '__main__':
    # parameters
    data_set_lst = [
        'snap_orkut', 'webgraph_webbase',
        'webgraph_twitter', 'snap_friendster'
    ]
    parameter_eps_lst = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    parameter_min_pts_lst = [5]
    thread_num_lst = [64]
    # loop run experiments
    loop_count = 1
    for i in xrange(loop_count):
        scalability_exp(data_set_lst=data_set_lst, parameter_eps_lst=parameter_eps_lst,
                        parameter_min_pts_lst=parameter_min_pts_lst, thread_num_lst=thread_num_lst,
                        folder_name='ppSCANNO_gpu23')
