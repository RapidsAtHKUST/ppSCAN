import decimal


def get_pretty_print_str(thread_lst, time_lst):
    header_1st_line = ' | '.join(
        ['thread_num', 'prune', 'check-core 1st bsp', 'check-core 2nd bsp', 'cluster-core', 'cluster-non-core',
         'total', 'total speedup'])
    header_2nd_line = ' | '.join(['---'] * 8)
    table_lines = [header_1st_line, header_2nd_line]
    for i in xrange(len(thread_lst)):
        row_lst = [thread_lst[i]]
        for phase_lst in time_lst:
            row_lst.append(str(float(phase_lst[i]) / 1000) + 's')
        total_lst = time_lst[-1]
        row_lst.append(
            decimal.Decimal.from_float(total_lst[0] / float(total_lst[i])).quantize(decimal.Decimal('0.000')))
        table_lines.append(' | '.join(map(str, row_lst)))
    return '\n'.join(table_lines)


def pretty_print(thread_lst, time_lst):
    print get_pretty_print_str(thread_lst, time_lst), '\n'


if __name__ == '__main__':
    file_name = 'scalability_robust_vis_res.txt'
    with open(file_name) as ifs:
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
