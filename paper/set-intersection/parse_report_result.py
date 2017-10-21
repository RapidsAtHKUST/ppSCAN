import os, decimal

csv_report_root_dir = '/home/yche/workspace/vtune_report/'

ppscan0_path = 'pSCANParallelExp0'
ppscan1_path = 'pSCANParallelExp1'

accumulated_time_tag = 'CPU Time:Self'
mem_load_tag = 'Loads:Self'
instr_num_tag = 'Instructions Retired:Self'

front_end_bound_tag = 'Front-End Bound:Self(%)'
bad_speculation_tag = 'Bad Speculation:Self(%)'
back_end_core_bound_tag = 'Back-End Bound:Core Bound:Self(%)'
back_end_mem_bound_tag = 'Back-End Bound:Memory Bound:Self(%)'
retire_tag = 'Retiring:Self(%)'

ah_tag = 'ah'
macc_tag = 'macc'
ge_tag = 'ge'

eps_lst = map(str, [float(i + 1) / 10 for i in range(9)])


def format_str(float_num):
    float_num = float(float_num)
    return str(decimal.Decimal.from_float(float_num).quantize(decimal.Decimal('0.000')))


def get_my_dict(dataset, eps, algo, profiler_tag):
    with open(csv_report_root_dir + dataset + os.sep + '-'.join([eps, algo, profiler_tag])) as ifs:
        lines = map(lambda line: line.strip(), ifs.readlines())
        tag_line = filter(lambda line: line.startswith('Function Stack'), lines)[0]
        value_line = filter(lambda line: line.strip().startswith('GraphParallelExp::IntersectNeighborSets'), lines)[0]
        my_dict = dict(zip(tag_line.split(','), value_line.split(',')))
        return my_dict


def get_reduction_info(dataset):
    print '\nreduction info, dataset:', dataset
    speedup_rows = []
    detail_rows = []

    def get_reduction_info_single_eps(eps):
        ppscan0_ah_dict = get_my_dict(dataset, eps, ppscan0_path, ah_tag)
        ppscan1_ah_dict = get_my_dict(dataset, eps, ppscan1_path, ah_tag)
        ppscan0_macc_dict = get_my_dict(dataset, eps, ppscan0_path, macc_tag)
        ppscan1_macc_dict = get_my_dict(dataset, eps, ppscan1_path, macc_tag)

        ppscan0_data = [ppscan0_ah_dict[instr_num_tag], ppscan0_macc_dict[mem_load_tag],
                        min(ppscan0_ah_dict[accumulated_time_tag], ppscan0_macc_dict[accumulated_time_tag])]
        ppscan1_data = [ppscan1_ah_dict[instr_num_tag], ppscan1_macc_dict[mem_load_tag],
                        min(ppscan1_ah_dict[accumulated_time_tag], ppscan1_macc_dict[accumulated_time_tag])]
        speedup_data = map(lambda my_pair: float(my_pair[1]) / float(my_pair[0]), zip(ppscan0_data, ppscan1_data))
        my_lst = []
        for idx in xrange(3):
            if idx < 2:
                my_lst.append(format_str(float(ppscan0_data[idx]) / (10 ** 9)) + 'b')
                my_lst.append(format_str(float(ppscan1_data[idx]) / (10 ** 9)) + 'b')
            else:
                my_lst.append(format_str(ppscan0_data[idx]))
                my_lst.append(format_str(ppscan1_data[idx]))

        return map(format_str, speedup_data), my_lst

    for eps in eps_lst:
        speedup, detail = get_reduction_info_single_eps(eps)
        speedup_rows.append(speedup)
        detail_rows.append(detail)
    return speedup_rows, detail_rows


def get_bound_info(dataset):
    print '\nbound info, dataset:', dataset
    ppscan0_bound_lst = []
    ppscan1_bound_lst = []

    def get_bound_info_single_eps(eps, exec_path):
        exec_path_ge_dict = get_my_dict(dataset, eps, exec_path, ge_tag)
        return [exec_path_ge_dict[front_end_bound_tag], exec_path_ge_dict[bad_speculation_tag],
                exec_path_ge_dict[back_end_core_bound_tag], exec_path_ge_dict[back_end_mem_bound_tag],
                exec_path_ge_dict[retire_tag]]

    for eps in eps_lst:
        ppscan0_bound_lst.append(get_bound_info_single_eps(eps, ppscan0_path))
        ppscan1_bound_lst.append(get_bound_info_single_eps(eps, ppscan1_path))
    return ppscan0_bound_lst, ppscan1_bound_lst


def process_one_dataset(dataset):
    speedup_rows, detail_rows = get_reduction_info(dataset)
    ppscan0_bound_rows, ppscan1_bound_rows = get_bound_info(dataset)

    speedup_table = [' | '.join(['eps', 'instr#', 'mem load#', 'time']), ' | '.join(['---'] * 4)]
    detail_table = [
        ' | '.join(['eps', 'opt instr#', 'prev instr#', 'opt mem load#', 'prev mem load#', 'opt time', 'prev time']),
        ' | '.join(['---'] * 7)]
    bound_table_header = ' | '.join(['eps', 'front-end', 'bad-spec', 'back-end-core', 'back-end-mem', 'retired-instr'])
    ppscan0_bound_table = [bound_table_header, ' | '.join(['---'] * 6)]
    ppscan1_bound_table = [bound_table_header, ' | '.join(['---'] * 6)]

    for idx, eps in enumerate(eps_lst):
        speedup_rows[idx].insert(0, eps)
        detail_rows[idx].insert(0, eps)
        ppscan0_bound_rows[idx].insert(0, eps)
        ppscan1_bound_rows[idx].insert(0, eps)

    for idx in xrange(len(eps_lst)):
        speedup_table.append(' | '.join(speedup_rows[idx]))
        detail_table.append(' | '.join(detail_rows[idx]))
        ppscan0_bound_table.append(' | '.join(ppscan0_bound_rows[idx]))
        ppscan1_bound_table.append(' | '.join(ppscan1_bound_rows[idx]))

    print '\n'.join(speedup_table)
    print '\n'.join(detail_table)
    print '\n'.join(ppscan0_bound_table)
    print '\n'.join(ppscan1_bound_table)

    with open(dataset + '.md', 'w') as ofs:
        ofs.write('### reduction factor\n\n')
        ofs.write('\n'.join(speedup_table) + '\n\n')

        ofs.write('### reduction detail\n\n')
        ofs.write('\n'.join(detail_table) + '\n\n')

        ofs.write('### ppscan0(opt) bound ratio\n\n')
        ofs.write('\n'.join(ppscan0_bound_table) + '\n\n')

        ofs.write('### ppscan1(prev) bound ratio\n\n')
        ofs.write('\n'.join(ppscan1_bound_table) + '\n\n')

    print speedup_rows, '\n', detail_rows
    print ppscan0_bound_rows, '\n', ppscan1_bound_rows


if __name__ == '__main__':
    process_one_dataset('orkut')
    process_one_dataset('webbase')
    process_one_dataset('twitter')
    process_one_dataset('friendster')
