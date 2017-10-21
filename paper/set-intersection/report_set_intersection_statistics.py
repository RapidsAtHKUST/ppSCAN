import os

if __name__ == '__main__':
    amplxe_cl_path = '/opt/intel/vtune_amplifier/bin64/amplxe-cl'
    # dataset and parameters
    dataset_dir_path = '/home/yche/GitRepos/ScanOptimizing/dataset/'
    dataset_path_lst = map(lambda file_name: dataset_dir_path + file_name,
                           ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster'])
    eps_lst = map(str, [float(i + 1) / 10 for i in range(9)])

    # exec path
    ppscan0_path = '/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/build/pSCANParallelExp0'
    ppscan1_path = '/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/build/pSCANParallelExp1'
    exec_path_lst = [ppscan0_path, ppscan1_path]

    # profiler tag, and filter list
    profiler_tag_lst = ['advanced-hotspots', 'memory-access', 'general-exploration']
    advanced_hotspots_tag_lst = ['-column="CPU Time:Self"', '-column="Instructions Retired:Self"']
    memory_access_tag_lst = ['-column="CPU Time:Self"', '-column="Loads:Self"']
    general_exploration_tag_lst = ['-column="Front-End Bound:Self"', '-column="Bad Speculation:Self"',
                                   '-column="Back-End Bound:Core Bound:Self"',
                                   '-column="Back-End Bound:Memory Bound:Self"',
                                   '-column="Retiring:Self"']
    profiler_filter_lst = [' '.join(advanced_hotspots_tag_lst), ' '.join(memory_access_tag_lst),
                           ' '.join(general_exploration_tag_lst)]
    profiler_tag_abbr_lst = ['ah', 'macc', 'ge']
    # result file root folder
    result_dir = '/home/yche/workspace/vtune_data/'
    csv_report_root_dir = '/home/yche/workspace/vtune_report/'

    for dataset_path in dataset_path_lst:
        dataset_name = dataset_path.split(os.sep)[-1].split('_')[-1]
        my_folder_prefix = csv_report_root_dir + dataset_name
        os.system('mkdir -p ' + my_folder_prefix)

        for eps in eps_lst:
            for exec_path in exec_path_lst:
                exec_name = exec_path.split(os.sep)[-1]
                for idx, profiler_tag in enumerate(profiler_tag_lst):
                    profiler_filter_tag = profiler_filter_lst[idx]
                    result_path = result_dir + '-'.join([dataset_name, exec_name, eps, profiler_tag])
                    csv_file_path = my_folder_prefix + os.sep + '-'.join(
                        [eps, exec_name, profiler_tag_abbr_lst[idx]])
                    my_cmd = ' '.join([
                        amplxe_cl_path, '-R top-down', '-result-dir', result_path,
                        '-group-by function', '-filter function=GraphParallelExp::IntersectNeighborSets',
                        profiler_filter_tag, '-report-output', csv_file_path, '-format csv -csv-delimiter comma'])
                    print my_cmd
                    os.system(my_cmd)
