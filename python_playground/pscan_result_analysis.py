def get_cores_hubs():
    with open('/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/data/result-0.3-5.txt') as ifs:
        lines = map(lambda my_str: my_str.strip(), ifs.readlines()[1:])
        core_lines = filter(lambda line: 'c' in line, lines)
        hub_lines = filter(lambda line: 'n' in line, lines)
        print len(core_lines), len(hub_lines)
        cores = map(lambda line: tuple(map(int, line.split()[1:])), core_lines)
        hubs = map(lambda line: tuple(map(int, line.split()[1:])), hub_lines)
        return cores, hubs


def statistics_hub_vertices(hubs):
    hub_belong_dict = {}
    for hub in hubs:
        if hub[0] not in hub_belong_dict:
            hub_belong_dict[hub[0]] = 0
        hub_belong_dict[hub[0]] += 1
        if hub_belong_dict[hub[0]] > 1:
            print hub_belong_dict[hub[0]]


if __name__ == '__main__':
    cores, hubs = get_cores_hubs()
    statistics_hub_vertices(hubs)
