import os


def generate_different_avg(vertex_num, avg_deg, cluster_coefficient):
    max_degree = 500

    param_lst = map(str, ['/home/yche/src_refactor_cpp/build/undirected_graph/lfr_undir_net', '-N', vertex_num,
                          '-k', avg_deg, '-maxk',
                          max_degree, '-mu', 0.1, '-C', cluster_coefficient])
    data_folder = '/home/yche/lfr_dataset' + os.sep + 'n' + str(vertex_num) + '_k' + str(avg_deg) + '_C' + str(
        cluster_coefficient).replace('.', 'dot')
    os.system('mkdir -p ' + data_folder)

    cmd = 'pushd ' + data_folder + ';' + '' + ' '.join(param_lst) + ';popd'
    print cmd
    os.system(cmd)


if __name__ == '__main__':
    # vertex_number, avg_degree = 2400000, 100
    # vertex_number, avg_degree = 3000000, 80
    # vertex_number, avg_degree = 4000000, 60
    # vertex_number, avg_degree = 6000000, 40
    vertex_number, avg_degree = 12000000, 20
    for coefficient in [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7]:
        generate_different_avg(vertex_num=vertex_number, avg_deg=avg_degree, cluster_coefficient=coefficient)
