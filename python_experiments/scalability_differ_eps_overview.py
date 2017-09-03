from scalability_figure_overview import *

if __name__ == '__main__':
    min_pts = 5

    with open('case_studies/figures-case-study3-scalability-differ-eps/ReadMe.md', 'w') as ifs:
        ifs.write(' | '.join(['dataset', 'scalability link']) + '\n')
        ifs.write(' | '.join(['---', '---']) + '\n')
        for data_set in data_set_lst:
            file_path = data_set + '.md'
            ifs.write(' | '.join([data_set, '[' + file_path + '](' + file_path + ')']) + '\n')
        ifs.write('\n')

        for eps in [float(i + 1) / 10 for i in xrange(9)]:
            best_thread_num_lst, rows = draw_overview_figure(eps=eps, min_pts=min_pts)

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
