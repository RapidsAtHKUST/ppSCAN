from workload_figure import *
import decimal

eval_workload_increase_tag = 'pscan+/pscan eval'
runtime_increase_tag = 'pscan+/pscan runtime'


def pretty_float(float_num):
    return decimal.Decimal(float_num).quantize(decimal.Decimal('0.0000'))


def generate_table(eps_lst, display_lst):
    tag_list = [display_prune0_tag, display_prune1_tag,
                display_pscan_eval_tag, display_pscan_plus_eval_tag,
                display_pscan_runtime_tag, display_pscan_plus_runtime_tag,
                eval_workload_increase_tag, runtime_increase_tag]
    eps_lst = map(lambda eps_val: 'eps-' + str(eps_val), eps_lst)
    result_lst = map(lambda tag:
                     map(lambda display: str(pretty_float(display[tag])), display_lst), tag_list)

    first_line_things = ['tag']
    first_line_things.extend(map(str, eps_lst))
    header_1st_line = ' | '.join(first_line_things)
    header_2nd_line = ' | '.join(['---'] * len(first_line_things))
    print header_1st_line
    print header_2nd_line
    for idx, tag in enumerate(tag_list):
        row = [tag]
        row.extend(result_lst[idx])
        print ' | '.join(row)

    # print result_lst
    print


if __name__ == '__main__':
    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    'webgraph_twitter', 'snap_friendster',
                    '10million_avgdeg15_maxdeg50_Cdefault']

    min_pts = 5
    root_dir_path = '/mnt/mount-gpu/d2/yche/projects/python_experiments/worklaod'
    parameter_eps_lst = [float(i + 1) / 10 for i in xrange(9)]
    for data_set in data_set_lst:
        statistics_lst = map(lambda eps: get_workload_statistics(data_set, eps, min_pts, root_dir_path), parameter_eps_lst)
        display_lst = map(lambda statistics_dict: to_display_dict(statistics_dict, 1000), statistics_lst)
        for display in display_lst:
            display[eval_workload_increase_tag] = display[display_pscan_plus_eval_tag] / display[display_pscan_eval_tag]
            display[runtime_increase_tag] = \
                display[display_pscan_plus_runtime_tag] / display[display_pscan_runtime_tag]
        generate_table(parameter_eps_lst, display_lst)
