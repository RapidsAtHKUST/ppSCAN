import os

if __name__ == '__main__':
    amplxe_cl_path = '/opt/intel/vtune_amplifier_xe/bin64//amplxe-cl'

    dataset_dir_path = '/nfsshare/share/dataset/'
    dataset_path_lst = map(lambda file_name: dataset_dir_path + file_name,
                           ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster'])

    eps_lst = [float(i + 1) / 10 for i in range(9)]
    min_u = 5
    thread_num = 256

    ppscan0_path = '/nfsshare/share/pSCAN-refactor/build/pSCANParallelExp0AVX512'
    ppscan1_path = '/nfsshare/share/pSCAN-refactor/build/pSCANParallelExp1'
    ppscan2_path = '/nfsshare/share/pSCAN-refactor/build/pSCANParallelExp2'
    exec_path_lst = [ppscan0_path, ppscan1_path, ppscan2_path]

    profiler_tag_lst = ['advanced-hotspots', 'memory-access', 'general-exploration']

    sample_interval_lst = [10, 10, 100, 100]
    for idx, dataset_path in enumerate(dataset_path_lst):
        profiler_knob_tag_lst = ['-knob sampling-interval=' + str(sample_interval_lst[idx]),
                                 '-knob sampling-interval=' + str(sample_interval_lst[idx]),
                                 '-knob collect-memory-bandwidth=true '
                                 '-knob enable-user-tasks=false']
        dataset_name = dataset_path.split(os.sep)[-1].split('_')[-1]

        for eps in eps_lst:
            for exec_path in exec_path_lst:
                for profiler_tag_idx, profiler_tag in enumerate(profiler_tag_lst):
                    result_dir = '/home/yche/workspace/vtune_res/' + '-'.join(
                        [dataset_name, exec_path.split(os.sep)[-1], str(eps), profiler_tag])

                    hotspot_cmd = ' '.join(
                        [amplxe_cl_path, '-collect', profiler_tag, profiler_knob_tag_lst[profiler_tag_idx],
                         '-result-dir', result_dir, exec_path, dataset_path, str(eps), str(min_u), str(thread_num)])
                    print hotspot_cmd
                    os.system(hotspot_cmd)
