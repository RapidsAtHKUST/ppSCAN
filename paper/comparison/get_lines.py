import os
import decimal
import json

total_time_tag = 'Total time without IO:'

root_folder_gpu23 = '/home/yche/mnt/gpu-23/projects/python_experiments'
root_folder_knl = '/home/yche/mnt/luocpu8//nfsshare/share/python_experiments'

gpu23_tag = 'gpu23'
knl_tag = 'knl'

data_set_lst = ['snap_orkut', 'webgraph_webbase', 'webgraph_twitter', 'snap_friendster']
eps_lst = [float(i + 1) / 10 for i in xrange(9)]
min_pts = 5


def get_lines(dataset, eps, min_pts, data_folder, root_folder):
    dir_path = os.sep.join(
        map(str, [root_folder, data_folder, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts)]))
    file_name = '-'.join(map(str, ['output', dataset, eps, min_pts]))
    file_path = dir_path + os.sep + file_name + '.txt'
    if not os.path.isfile(file_path):
        return []
    with open(file_path) as ifs:
        return ifs.readlines()


def get_lines_for_ppscan(dataset, eps, min_pts, data_folder, root_folder, thread_num):
    dir_path = os.sep.join(
        map(str, [root_folder, data_folder, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts)]))
    file_name = '-'.join(map(str, ['output', dataset, eps, min_pts, str(thread_num)]))
    file_path = dir_path + os.sep + file_name + '.txt'
    if not os.path.isfile(file_path):
        return []
    with open(file_path) as ifs:
        return ifs.readlines()


# for scan, pscan, ppscan
def parse_line(line):
    if 'ms' in line:
        return int(line.split(':')[-1][:-len(' ms\n') + 1])
    else:
        return int(line.split(':')[-1]) / 1000


def format_str(float_num):
    if isinstance(float_num, str):
        return float_num
    return str(decimal.Decimal.from_float(float_num).quantize(decimal.Decimal('0.000')))


# 1st: scan super naive 2007 kdd(serial)
def scan_super_naive_json_gen():
    scan_super_naive_json = dict()

    def get_scan_naive_per_env(my_root_folder):
        my_dict = dict()
        for data_set in data_set_lst:
            time_lst = []
            for eps in eps_lst:
                lines = get_lines(data_set, eps, min_pts, data_folder='origin-scan-super-naive-exp',
                                  root_folder=my_root_folder)
                lines = filter(lambda my_str: total_time_tag in my_str, lines)
                assert len(lines) <= 1
                if len(lines) == 1:
                    line = parse_line(lines[0])
                    if line <= 5400000:
                        # print line
                        time_lst.append(line)
                    else:
                        time_lst.append('TLE')
                        # print 'TLE', data_set, eps
                else:
                    time_lst.append('TLE')
                    # print 'TLE', data_set, eps
            my_dict[data_set] = time_lst
        # print my_dict
        return my_dict

    scan_super_naive_json[gpu23_tag] = get_scan_naive_per_env(root_folder_gpu23)
    scan_super_naive_json[knl_tag] = get_scan_naive_per_env(root_folder_knl)
    json_str = json.dumps(scan_super_naive_json, indent=4)
    with open('data-json/scan_super_naive.json', 'w') as ofs:
        ofs.write(json_str)


# 2nd: pscan 2016 icde (serial)
def pscan_json_gen():
    pscan_json_dict = dict()

    def get_pscan_per_env(my_root_folder, data_folder_local):
        my_dict = dict()
        for data_set in data_set_lst:
            time_lst = []
            for eps in eps_lst:
                lines = get_lines(data_set, eps, min_pts, data_folder=data_folder_local,
                                  root_folder=my_root_folder)
                line = filter(lambda my_str: total_time_tag in my_str, lines)[-1]
                line = parse_line(line)
                if line <= 5400000:
                    # print line
                    time_lst.append(line)
                else:
                    time_lst.append('TLE')
                my_dict[data_set] = time_lst

        # print my_dict
        return my_dict

    pscan_json_dict[gpu23_tag] = get_pscan_per_env(root_folder_gpu23, 'worklaod')
    pscan_json_dict[knl_tag] = get_pscan_per_env(root_folder_knl, 'knl-workload-pscan-only')

    json_str = json.dumps(pscan_json_dict, indent=4)
    with open('data-json/pscan.json', 'w') as ofs:
        ofs.write(json_str)


def scan_xp_json_gen():
    scan_xp_json_dict = dict()

    def get_scan_xp_per_env(my_root_folder, data_folder_local):
        my_dict = dict()
        for data_set in data_set_lst:
            time_lst = []
            for eps in eps_lst:
                lines = get_lines(data_set, eps, min_pts, data_folder=data_folder_local,
                                  root_folder=my_root_folder)
                s_unit_tag_lst = ['STEP1', 'STEP2', 'STEP3']
                ms_unit_tag_lst = ['MarkMinEle Cost', 'STEP4']
                tag_line_floats = map(lambda tag:
                                      eval(filter(lambda my_str: tag in my_str, lines)[0].split(':')[-1]),
                                      s_unit_tag_lst)
                ms_tag_ints = map(lambda tag:
                                  eval(filter(lambda my_str: tag in my_str, lines)[0].strip(' ms\n').split(':')[-1]),
                                  ms_unit_tag_lst)

                # print tag_line_floats, ms_tag_lines
                first_three_step = int(sum(tag_line_floats) * 1000)
                total_time = first_three_step + sum(ms_tag_ints)
                if total_time <= 5400000:
                    print total_time
                    time_lst.append(total_time)
                else:
                    time_lst.append('TLE')
            my_dict[data_set] = time_lst
        return my_dict

    scan_xp_json_dict[gpu23_tag] = get_scan_xp_per_env(root_folder_gpu23, 'scan-xp-exp')
    scan_xp_json_dict[knl_tag] = get_scan_xp_per_env(root_folder_knl, 'scan-xp-exp')

    json_str = json.dumps(scan_xp_json_dict, indent=4)
    with open('data-json/scan_xp.json', 'w') as ofs:
        ofs.write(json_str)


def anyscan_json_gen():
    anyscan_super_naive_json = dict()
    any_scan_total_time_tag = 'FINAL WALL TIMES'

    def get_anyscan_per_env(my_root_folder):
        my_dict = dict()
        for data_set in data_set_lst:
            time_lst = []
            for eps in eps_lst:
                lines = get_lines(data_set, eps, min_pts, data_folder='anyscan-exp-1121',
                                  root_folder=my_root_folder)
                lines = filter(lambda my_str: any_scan_total_time_tag in my_str, lines)
                assert len(lines) <= 1
                if len(lines) == 1:
                    line = int(eval(lines[0].split(':')[-1]) * 1000)
                    if line <= 5400000:
                        # print line
                        time_lst.append(line)
                    else:
                        time_lst.append('RE')
                        # print 'TLE', data_set, eps
                else:
                    time_lst.append('RE')
                    # print 'TLE', data_set, eps
            my_dict[data_set] = time_lst
        # print my_dict
        return my_dict

    anyscan_super_naive_json[gpu23_tag] = get_anyscan_per_env(root_folder_gpu23)
    anyscan_super_naive_json[knl_tag] = get_anyscan_per_env(root_folder_knl)
    json_str = json.dumps(anyscan_super_naive_json, indent=4)
    with open('data-json/anyscan.json', 'w') as ofs:
        ofs.write(json_str)


def ppscan_json_gen():
    ppscan_json_dict = dict()

    def get_ppscan_per_env(my_root_folder, data_folder_local, thread_number):
        my_dict = dict()
        for data_set in data_set_lst:
            time_lst = []
            for eps in eps_lst:
                lines = get_lines_for_ppscan(data_set, eps, min_pts, data_folder=data_folder_local,
                                             root_folder=my_root_folder, thread_num=thread_number)
                lines = filter(lambda my_str: total_time_tag in my_str, lines)
                line = min(map(parse_line, lines))
                if line <= 5400000:
                    # print line
                    time_lst.append(line)
                else:
                    time_lst.append('TLE')
                my_dict[data_set] = time_lst

        # print my_dict
        return my_dict

    ppscan_json_dict[gpu23_tag] = get_ppscan_per_env(root_folder_gpu23, 'scalability_simd_paper_gpu23', 64)
    ppscan_json_dict[knl_tag] = get_ppscan_per_env(root_folder_knl, 'scalability_simd_paper2', 256)

    json_str = json.dumps(ppscan_json_dict, indent=4)
    with open('data-json/ppscan.json', 'w') as ofs:
        ofs.write(json_str)


if __name__ == '__main__':
    os.system('mkdir -p data-json')
    # scan_super_naive_json_gen()
    # pscan_json_gen()
    # scan_xp_json_gen()
    # anyscan_json_gen()
    ppscan_json_gen()
