from run_scalability_experiment import *

if __name__ == '__main__':
    data_set_lst = ['small_snap_dblp',
                    'snap_livejournal', 'snap_orkut', 'snap_pokec',
                    'lfr_benchmark/10million_avgdeg15_maxdeg50_Cdefault',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster']
    parameter_eps_lst = [0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9]
    parameter_min_pts_lst = [5]
    thread_num_lst = [40]

    # let pace-cache flushed by others input and memory usage
    for parameter_eps in parameter_eps_lst:
        scalability_exp(data_set_lst=data_set_lst, parameter_eps_lst=[parameter_eps],
                        parameter_min_pts_lst=parameter_min_pts_lst, thread_num_lst=thread_num_lst,
                        folder_name='scalability_input_output')
