import os
from workload_figure import filter_lines_by_tag
from scalability_figure_overview import ms_val_to_s

runtime_tag = 'Total time without IO'
abs_eval_portion_tag = 'absolute intersect portion'
filtered_eval_portion_tag = 'filtered intersect portion'
portion_tag = 'portion'
avg_eval_tag = 'avg eval'

table_dict = {
    runtime_tag: 'runtime',
    abs_eval_portion_tag: 'abs eval',
    filtered_eval_portion_tag: 'relative eval',
    avg_eval_tag: avg_eval_tag,
    portion_tag: 'cmp ratio'
}


def get_workload_statistics(dataset, eps, min_pts, root_dir_path='.'):
    file_path = os.sep.join([root_dir_path, dataset, 'eps-' + str(eps), 'min_pts-' + str(min_pts),
                             '-'.join(['output', dataset, str(eps), str(min_pts)]) + '.txt'])
    with open(file_path) as ifs:
        lines = ifs.readlines()

        tag_lst = [runtime_tag, abs_eval_portion_tag, filtered_eval_portion_tag]
        ret_dict = dict(map(lambda tag: (tag, filter_lines_by_tag(tag, lines)[-1]), tag_lst))
        avg_eval_line = filter(lambda line: 'avg eval' in line, lines)[-1]

        tag_lst = [portion_tag, avg_eval_tag]
        tmp_lst = map(lambda tag: (tag, filter_lines_by_tag(tag, avg_eval_line.strip().split(' ,'))[-1]), tag_lst)

        for key, val in tmp_lst:
            ret_dict[key] = val

        ret_dict[runtime_tag] = ms_val_to_s(ret_dict[runtime_tag])
        return ret_dict


def get_markdown_table(dataset, tag_lst):
    eps_lst = [float(i + 1) / 10 for i in xrange(9)]
    min_pts_lst = [2, 5, 10, 15]

    header_lst = ['/'.join(map(lambda key: table_dict[key], tag_lst))]
    header_lst.extend(map(lambda eps_val: 'eps-' + str(eps_val), eps_lst))
    header_line = ' | '.join(header_lst)
    second_line = ' | '.join(['---' for _ in xrange(len(eps_lst) + 1)])
    rows = [header_line, second_line]

    def statistics_dict_to_str(statistics_dict):
        return '/'.join(map(lambda tag: str(statistics_dict[tag]), tag_lst))

    folder_path = '/mnt/mount-gpu/d2/yche/projects/python_experiments/eval-workload'
    for min_pts in min_pts_lst:
        my_lst = ['min_pts:' + str(min_pts)]
        my_lst.extend(
            map(lambda eps: statistics_dict_to_str(get_workload_statistics(dataset, eps, min_pts, folder_path)),
                eps_lst))
        rows.append(' | '.join(my_lst))
    return '\n'.join(rows)


def generate_markdown_file(dataset):
    with open(case_study_folder + os.sep + dataset + '.md', 'w') as ofs:
        ofs.write(
            '\n\n'.join(['## Avg Eval/Cmp Ratio', get_markdown_table(dataset, tag_lst=[avg_eval_tag]),
                         get_markdown_table(dataset, tag_lst=[portion_tag])
                            , '## Workload', get_markdown_table(dataset, tag_lst=[abs_eval_portion_tag]),
                         get_markdown_table(dataset, tag_lst=[filtered_eval_portion_tag]), '## Runtime',
                         get_markdown_table(dataset, tag_lst=[runtime_tag])]))


if __name__ == '__main__':
    case_study_folder = 'case_studies/case-study4-workload'
    os.system('mkdir -p ' + case_study_folder)

    data_set_lst = ['small_snap_dblp',
                    'snap_pokec', 'snap_livejournal', 'snap_orkut',
                    'webgraph_uk', 'webgraph_webbase',
                    '10million_avgdeg15_maxdeg50_Cdefault']

    for data_set in data_set_lst:
        generate_markdown_file(dataset=data_set)
