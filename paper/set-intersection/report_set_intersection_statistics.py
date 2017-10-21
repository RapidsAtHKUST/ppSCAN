import os

if __name__ == '__main__':
    amplxe_cl_path = '/opt/intel/vtune_amplifier/bin64/amplxe-cl'

    dataset_dir_path = '/home/yche/GitRepos/ScanOptimizing/dataset/'
    dataset_path_lst = map(lambda file_name: dataset_dir_path + file_name,
                           ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster'])
    eps_lst = [float(i + 1) / 10 for i in range(9)]

    ppscan0_path = '/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/build/pSCANParallelExp0'
    ppscan1_path = '/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/build/pSCANParallelExp1'
    exec_path_lst = [ppscan0_path, ppscan1_path]
    profiler_tag_lst = ['advanced-hotspots', 'memory-access', 'general-exploration']
    result_dir = '/home/yche/workspace/vtune_data/'

    eps = str(eps_lst[2])
    dataset = dataset_path_lst[0].split(os.sep)[-1].split('_')[-1]
    print dataset
    exec_path = exec_path_lst[0].split(os.sep)[-1]
    profiler_tag = profiler_tag_lst[2]

    result_path = result_dir + '-'.join([dataset, exec_path, eps, profiler_tag])
    print result_path
    my_cmd = ' '.join([
        amplxe_cl_path, '-R top-down', '-result-dir', result_path,
        '-group-by function -filter function=GraphParallelExp::IntersectNeighborSets'])
    print my_cmd
    os.system(my_cmd)
