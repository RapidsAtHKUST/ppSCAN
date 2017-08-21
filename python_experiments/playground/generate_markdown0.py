def pretty_print(thread_lst, time_lst):
    print
    header_1st_line = ' | '.join(
        ['thread_num', 'prune', 'check-core 1st bsp', 'check-core 2nd bsp', 'cluster-core', 'cluster-non-core',
         'total'])
    header_2nd_line = ' | '.join(['---'] * 7)
    table_lines = [header_1st_line, header_2nd_line]
    for i in xrange(len(thread_lst)):
        row_lst = [thread_lst[i]]
        for phase_lst in time_lst:
            row_lst.append(str(float(phase_lst[i]) / 1000) + 's')
        table_lines.append(' | '.join(map(str, row_lst)))
    print '\n'.join(table_lines)


if __name__ == '__main__':
    with open('scalability_vis_middle_res.txt') as ifs:
        lines = ifs.readlines()
        thread_lst_lst = map(lambda my_str: eval(my_str.split(':')[-1]),
                             filter(lambda line: 'thread list:' in line, lines))

        time_lst_lst = map(lambda my_str: eval(my_str.split(':')[-1]),
                           filter(lambda line: 'time info list:' in line, lines))
        for time_lst in time_lst_lst:
            print time_lst

        for idx, time_lst in enumerate(time_lst_lst):
            thread_lst = thread_lst_lst[idx]
            pretty_print(thread_lst, time_lst)
