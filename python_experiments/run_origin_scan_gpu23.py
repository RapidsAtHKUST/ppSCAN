import time_out_util
import os, time

if __name__ == '__main__':
    origin_scan_opt_path = '../pSCAN-refactor/build/experimental/scan_origin/SCANOrigin'
    data_set_lst = [
        'snap_livejournal',
        'snap_orkut',
        'webgraph_webbase',
        'webgraph_twitter',
        'snap_friendster'
    ]
    parameter_eps_lst = [float(i + 1) / 10 for i in xrange(9)]
    parameter_min_pts_lst = [5]

    data_set_lst = map(lambda name: os.pardir + os.sep + 'dataset' + os.sep + name, data_set_lst)
    thread_num = 64
    foler_name = 'origin-scan-opt-exp'
    for data_set_path in data_set_lst:
        for eps in parameter_eps_lst:
            for min_pts in parameter_min_pts_lst:
                # statistics output dir and file
                data_set_name = data_set_path.split(os.sep)[-1]
                statistics_dir = os.sep.join(
                    map(str, ['.', foler_name, data_set_name, 'eps-' + str(eps), 'min_pts-' + str(min_pts)]))
                os.system('mkdir -p ' + statistics_dir)
                statistics_file_path = statistics_dir + os.sep + '-'.join(
                    map(str, ['output', data_set_name, eps, min_pts])) + '.txt'

                # header
                my_splitter = '-'.join(['*' for _ in xrange(20)])
                os.system(' '.join(
                    ['echo', my_splitter + time.ctime() + my_splitter, '>>', statistics_file_path]))


                def write_split():
                    with open(statistics_file_path, 'a+') as ifs:
                        ifs.write(my_splitter + my_splitter + '\n')
                        ifs.write(my_splitter + my_splitter + '\n')


                params_lst = map(str, [origin_scan_opt_path, data_set_path,
                                       eps, min_pts, 'output', '>>', statistics_file_path])
                cmd = ' '.join(params_lst)
                print cmd
                tle_flag, info, correct_info = time_out_util.run_with_timeout(cmd, timeout_sec=36000)


                # check md5, line count and byte count
                def check_result():
                    res_file_name = '-'.join(map(str, ['result', eps, min_pts])) + '.txt'
                    res_file_path = data_set_path + os.sep + res_file_name
                    os.system(' '.join(['md5sum', res_file_path, '>>', statistics_file_path]))
                    os.system(' '.join(['wc -l', res_file_path, '>>', statistics_file_path]))
                    os.system(' '.join(['du -b', res_file_path, '>>', statistics_file_path]))


                check_result()
                write_split()

                with open(statistics_file_path, 'a+') as ifs:
                    ifs.write(correct_info)
                    ifs.write(my_splitter + time.ctime() + my_splitter)
                    ifs.write('is_time_out:' + str(tle_flag))
                    ifs.write('\n\n\n\n')
                print 'finish:', '-'.join(map(str, [data_set_path, eps, min_pts]))
    print
