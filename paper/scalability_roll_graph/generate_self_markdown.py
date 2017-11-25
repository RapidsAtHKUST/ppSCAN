import os, decimal


def format_str(float_num):
    if isinstance(float_num, str):
        return float_num
    return str(decimal.Decimal.from_float(float_num).quantize(decimal.Decimal('0.000')))


def parse_line(line):
    if 'ms' in line:
        return int(line.split(':')[-1][:-len(' ms\n') + 1])
    else:
        return int(line.split(':')[-1]) / 1000


total_time_tag = 'Total time without IO'


# should guarantee the format of the file, no empty body
def parse_lines(lines):
    total_time_lst = filter(lambda ele: total_time_tag in ele, lines)
    return map(parse_line, total_time_lst)


def get_statistics(dataset, eps, min_pts, graph_folder, root_folder='.'):
    dir_path = os.sep.join(
        map(str, [root_folder, graph_folder, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts)]))
    file_name = '-'.join(map(str, ['output', dataset, eps, min_pts]))
    file_path = dir_path + os.sep + file_name + '.txt'
    with open(file_path) as ifs:
        tmp_lines = parse_lines(ifs.readlines())
        time_pair = (min(tmp_lines), max(tmp_lines))
        time_pair = map(lambda time: float(time) / 1000, time_pair)
        time_pair.append(time_pair[1] / time_pair[0])
        time_pair.insert(0, str(eps))
        return time_pair


def generate_per_env(root_folder, data_folder, output_folder):
    min_pts = 5
    for dataset in ['n50_m20_1billion_edge', 'n25_m40_1billion_edge', 'n16dot7_m60_1billion_edge',
                    'n12dot5_m80_1billion_edge'
                    ]:
        print '\n', 'dataset:', dataset
        row_lst = []
        for eps in [float(i + 1) / 10 for i in xrange(9)]:
            single_eps_data = get_statistics(dataset, eps, min_pts, data_folder, root_folder=root_folder)
            row_lst.append(single_eps_data)
        if not os.path.exists(output_folder):
            os.mkdir(output_folder)
        with open(output_folder + os.sep + dataset + '.md', 'w') as ofs:
            ofs.write('eps | ppscan time | ppscan serial time | self-speedup \n')
            ofs.write('--- | --- | --- | ---\n')
            ofs.write('\n'.join(map(lambda row: ' | '.join(map(format_str, row)), row_lst)) + '\n')


if __name__ == '__main__':
    generate_per_env('/home/yche/mnt/luocpu8/nfsshare/share/python_experiments', 'paper-roll-graph-exp-11-24-knl',
                     'self-speedup-knl')
    generate_per_env('/home/yche/mnt/gpu-23/projects/python_experiments', 'paper-roll-graph-exp-11-24-gpu23',
                     'self-speedup-gpu23')
