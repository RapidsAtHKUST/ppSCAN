# 1st: same
edge_tag = 'num of edges'
prune0_tag = 'prune0 definitely not reachable'
prune1_tag = 'prune1 definitely reachable'

# 2nd: different
intersect_tag = 'intersection times'
cmp0_tag = 'cmp0'
cmp1_tag = 'cmp1'
cmp_equ_tag = 'equal cmp'
runtime_tag = 'Total time without IO'

# suffix
pscan_tag = ', pscan'
pscan_plus_tag = ', pscan+'


def pretty_print(my_dict):
    header_1st_line = ' | '.join(
        ['algorithm', edge_tag, prune0_tag.split(' ')[0], prune1_tag.split(' ')[0], intersect_tag,
         cmp0_tag, cmp1_tag, cmp_equ_tag, runtime_tag])
    header_2nd_line = ' | '.join(['---'] * 9)
    table_lines = [header_1st_line, header_2nd_line]
    for algo_tag in [pscan_tag, pscan_plus_tag]:
        row_lst = [algo_tag.split(' ')[-1], my_dict[edge_tag], my_dict[prune0_tag], my_dict[prune1_tag]]
        complete_tag_lst = map(lambda tag: tag + algo_tag,
                               [intersect_tag, cmp0_tag, cmp1_tag, cmp_equ_tag])
        for tag in complete_tag_lst:
            row_lst.append(my_dict[tag])
        row_lst.append(str(float(my_dict[runtime_tag + algo_tag]) / 1000) + 's')
        table_lines.append(' | '.join(map(str, row_lst)))
    print '\n'.join(table_lines), '\n\n'


if __name__ == '__main__':
    with open('workloads_vis_middle_res.txt') as ifs:
        my_dict_lst = map(eval, ifs.readlines())
        for my_dict in my_dict_lst:
            print my_dict
            pretty_print(my_dict)
