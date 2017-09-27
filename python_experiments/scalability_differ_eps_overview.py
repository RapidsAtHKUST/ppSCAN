from scalability_figure_overview import *

if __name__ == '__main__':
    overview_figure_folder = 'scalability_new3_all_in_parallel'
    markdown_folder = 'case_studies/figures-case-study10-scalability-differ-eps'
    os.system('mkdir -p ' + markdown_folder)
    all_data_set_lst = [
        'small_snap_dblp', 'snap_pokec', 'snap_livejournal', 'snap_orkut',
        # 'webgraph_uk', 'webgraph_webbase',
        # 'webgraph_twitter', 'snap_friendster'
    ]
    all_input_time_lst = [217, 1522, 3453, 9720, 13254, 45982, 47910, 215101]
    all_edge_num_lst = ['2,099,732', '30,282,866', '69,362,378', '234,370,166', '301,136,554', '1,050,026,736',
                        '1,369,000,750', '3,612,134,270']

    drawer = ScalabilityFigureDrawer(
        data_set_lst=all_data_set_lst,
        input_time_lst=all_input_time_lst,
        edge_num_lst=all_edge_num_lst,
        server_root_dir_path='/mnt/mount-gpu/d2/yche/projects/python_experiments'
                             '/scalability_input_output',
        overview_folder=overview_figure_folder)

    min_pts = 5
    with open(markdown_folder + os.sep + 'ReadMe.md', 'w') as ifs:
        def generate_description_table():
            ifs.write(' | '.join(['dataset', 'scalability link']) + '\n')
            ifs.write(' | '.join(['---', '---']) + '\n')
            for data_set in all_data_set_lst:
                file_path = data_set + '.md'
                ifs.write(' | '.join([data_set, '[' + file_path + '](' + file_path + ')']) + '\n')
            ifs.write('\n')


        generate_description_table()

        for eps in [float(i + 1) / 10 for i in xrange(9)]:
            best_thread_num_lst, rows = drawer.draw_overview_figure(eps=eps, min_pts=min_pts)


            def generate_detail_info():
                ifs.write('## eps:' + str(eps) + '\n\n')

                header = '\n'.join(['with 40 full logical threads | with best thread num', '--- | ---'])

                def get_link(txt, link_description):
                    return ''.join(
                        map(str,
                            ['![', txt, '-' + link_description, ']', '(../../figures/', overview_figure_folder, '/',
                             '-'.join(['eps:' + str(eps), 'min_pts:' + str(min_pts),
                                       link_description], ), '.png', ')']))

                link0 = get_link('portion with full logical threads', 'withalllogicalcores-comp-io-portion')
                link1 = get_link('io portition', 'withbestlogicalthreadnum-comp-io-portion')
                link2 = get_link('with full logical threads', 'withalllogicalcores-runtime-speedup')
                link3 = get_link('with best thread num', 'withbestlogicalthreadnum-runtime-speedup')

                ifs.write(header + '\n')
                ifs.write(' | '.join([link0, link1]) + '\n\n')
                ifs.write(header + '\n')
                ifs.write(' | '.join([link2, link3]) + '\n\n')

                ifs.write(
                    '\n'.join(
                        ['```zsh', 'best performance thread_num_lst:' + str(best_thread_num_lst), '```']) + '\n\n')
                ifs.write('\n'.join(['dataset | edge num | pscan runtime | pscan+ 40 logical-core full speedup '
                                     '| pscan+ best thread_num speedup | thread_num choice',
                                     '--- | --- | --- | --- | --- | ---']) + '\n')
                ifs.write('\n'.join(rows) + '\n\n')


            generate_detail_info()
