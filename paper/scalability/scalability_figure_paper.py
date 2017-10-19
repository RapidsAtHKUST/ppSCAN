import matplotlib.pyplot as plt

thread_str_lst = ['1', '4', '8', '16', '24', '32', '40']
pscan_runtime_lst = [164.248, 63.705, 2487.317, 3726.302]


def accumulate_for_bar_char(breakdown_time_lst):
    for idx in xrange(len(breakdown_time_lst)):
        for idx2 in xrange(idx + 1, len(breakdown_time_lst)):
            breakdown_time_lst[idx] = map(lambda pair: sum(pair),
                                          zip(breakdown_time_lst[idx], breakdown_time_lst[idx2]))
    return breakdown_time_lst


def get_exp1_data():
    def is_digit_start(my_str):
        for thread_num_str in thread_str_lst:
            if my_str.startswith(thread_num_str):
                return True
        return False

    def get_time_tuple(my_data):
        return map(lambda one_case:
                   map(lambda my_str: float(my_str.replace('s', '')),
                       one_case.split(' | ')[1:6]), my_data)

    def get_four_stage_lst(my_data):
        four_stage = []
        for idx in xrange(4):
            four_stage.append(map(lambda one_case: one_case[idx], my_data))
        return four_stage

    data_lst = []
    with open('exp1_fix_u_5_eps_3over10.md') as ifs:
        lines = ifs.readlines()
        lines = map(lambda my_str: my_str.strip(), filter(is_digit_start, lines))
        for i in xrange(len(lines) / len(thread_str_lst)):
            data_lst.append(lines[i * len(thread_str_lst): (i + 1) * len(thread_str_lst)])

    processed_lst = []
    for data in data_lst:
        data = get_time_tuple(data)
        data = map(lambda one_case: [one_case[0], one_case[1] + one_case[2], one_case[3], one_case[4]], data)
        data = get_four_stage_lst(data)
        processed_lst.append(data)

    return processed_lst


def get_speedup_lst_lst(my_data, idx):
    total_time_lst = map(sum, zip(my_data[0], my_data[1], my_data[2], my_data[3]))
    total_time_lst2 = map(sum, zip(my_data[0], my_data[1], my_data[2], my_data[3]))
    check_core_time_lst = my_data[1]
    other_stage_time_lst = map(sum, zip(my_data[0], my_data[2], my_data[3]))
    divider_lst = [pscan_runtime_lst[idx], total_time_lst[0], check_core_time_lst[0], other_stage_time_lst[0]]
    return [map(lambda num: divider_lst[0] / float(num), total_time_lst),
            map(lambda num: divider_lst[1] / float(num), total_time_lst2),
            map(lambda num: divider_lst[2] / float(num), check_core_time_lst),
            map(lambda num: divider_lst[3] / float(num), other_stage_time_lst)]


def first_scalability_exp_differ_thread_num(suffix_str='pdf'):
    tuple_lst = get_exp1_data()

    exp1_figure, ax_tuple = plt.subplots(2, 4, figsize=(16, 6))

    ax_title_lst = ['(a) dataset = orkut', '(b) dataset = webbase', '(c) dataset = twitter', '(d) datset = friendster']
    thread_lst = map(int, thread_str_lst)

    # runtime figure
    for idx in xrange(4):
        ax = ax_tuple[0][idx]
        data = accumulate_for_bar_char(tuple_lst[idx])

        # print 'runtime figure:', data
        hatch_lst = ['\\\\\\', '////', '----', '||||']
        cur_shape_color_idx = 0
        ax.plot(thread_lst, [pscan_runtime_lst[idx] for _ in xrange(len(thread_lst))], 'k--')
        for time_lst in data:
            ax.bar(thread_lst, time_lst, hatch=hatch_lst[cur_shape_color_idx], color='w', alpha=1, width=1.2)
            cur_shape_color_idx += 1
        ax.set_xticks(thread_lst)
        ax.set_xticklabels(thread_str_lst)
        ax.set_title(ax_title_lst[idx], fontsize=12)
        ax.legend(('pSCAN', 'ppSCAN pruning', 'ppSCAN checking core', 'ppSCAN clustering core',
                   'ppSCAN clustering non-core'))

    ax_tuple[0][0].set_ylabel('Runtime (s)', fontsize=12)

    tuple_lst = get_exp1_data()
    # for idx, data in enumerate(tuple_lst):
    #     print get_speedup_lst_lst(data, idx)

    # speedup figure
    for idx in xrange(4):
        ax = ax_tuple[1][idx]
        speedup_lst = get_speedup_lst_lst(tuple_lst[idx], idx)

        ax.plot(thread_lst, speedup_lst[0], 's-', label='over-pSCAN', linewidth=1.0, markersize=8,
                markerfacecolor='none')
        ax.plot(thread_lst, speedup_lst[1], '^-', label='self', linewidth=1.0, markersize=8,
                markerfacecolor='none')
        ax.plot(thread_lst, speedup_lst[2], 'D-', label='check-core-self', linewidth=1.0, markersize=8,
                markerfacecolor='none')
        ax.plot(thread_lst, speedup_lst[3], 'x-', label='other-stage-self', linewidth=1.0, markersize=8,
                markerfacecolor='none')
        ax.set_ylim(0, max(speedup_lst[0][-1], speedup_lst[1][-1], speedup_lst[2][-1], speedup_lst[3][-1]) * 1.3)
        ax.set_xlabel('thread number')
        ax.set_xticks(thread_lst)
        ax.set_xticklabels(thread_str_lst)
        ax.grid(True)
        ax.legend(('ppSCAN over pSCAN', 'ppSCAN self', 'checking core stage', 'other stage'))

    ax_tuple[1][0].set_ylabel('Speedup', fontsize=12)

    exp1_figure.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('scalability_exp_fix_parameters_vary_t.' + suffix_str, dpi=1200)
    exp1_figure.show()


if __name__ == '__main__':
    first_scalability_exp_differ_thread_num()
