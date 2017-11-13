from scalability_figure_knl import *
from playground.generate_markdown0_scalability import *


def post_process(statistics_dic):
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, total_time_tag]
    sorted_info_lst = sorted(statistics_dic.items(), key=lambda pair: pair[0])

    thread_lst = map(lambda pair: pair[0], sorted_info_lst)
    time_info_lst = map(lambda pair: pair[1], sorted_info_lst)

    time_lst_lst = map(lambda tag: map(lambda my_dict: my_dict[tag], time_info_lst), tag_list)
    print 'thread list:', thread_lst
    new_lst = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
               non_core_cluster_time_tag, total_time_tag]

    print 'time info list:', map(lambda tag: map(lambda my_dict: my_dict[tag], time_info_lst), new_lst)
    return thread_lst, time_lst_lst


if __name__ == '__main__':
    data_set_lst = [
        # 'small_snap_dblp',
        # 'snap_pokec',
        # 'snap_livejournal',
        'snap_orkut',
        # 'webgraph_uk',
        'webgraph_webbase',
        'webgraph_twitter',
        # 'snap_friendster'
    ]

    figure_folder = 'scalability_simd_paper2'
    markdown_folder = 'case_studies/figures-case-study15-scalability-knl'

    os.system('mkdir -p ./figures/' + figure_folder)
    os.system('mkdir -p ' + markdown_folder)
    min_pts = 5
    eps_lst = [float(i + 1) / 10 for i in xrange(9)]
    # eps_lst = [0.1, 0.2, 0.3, 0.4]

    for data_set in data_set_lst:
        with open(markdown_folder + os.sep + data_set + '.md', 'w') as ifs:
            # should generate a markdown, e.g, small_snap_dblp.md
            for eps in eps_lst:
                time_info_dict = get_statistics(data_set, eps, min_pts, figure_folder=figure_folder,
                                                root_folder='/home/yche/mnt/luocpu8/nfsshare/share/python_experiments')
                append_txt = ' - '.join([data_set, 'eps:' + str(eps), 'min_pts:' + str(min_pts)])
                # 1st: overall time cost
                display_overview(time_info_dict, append_txt, figure_folder)

                # 2nd: runtime/speedup for filtered tags
                display_filtered_tags(time_info_dict, append_txt, figure_folder)
                thread_lst, time_lst_lst = post_process(statistics_dic=time_info_dict)

                ifs.write('## eps:' + str(eps) + '\n\n')
                ifs.write('\n'.join(['overview | speedup', '--- | ---']) + '\n')


                def get_link(link_description):
                    return ''.join(
                        map(str, ['![', data_set, '-' + link_description, ']', '(../../figures/', figure_folder, '/',
                                  '-'.join([data_set, 'eps:' + str(eps), 'min_pts:' + str(min_pts),
                                            link_description], ), '.png', ')']))


                ifs.write(' | '.join([get_link('overview'), get_link('runtime-speedup')]) + '\n\n')
                ifs.write(get_pretty_print_str(thread_lst, time_lst_lst) + '\n\n')
