import os

if __name__ == '__main__':
    dir_path = '/home/yche/workspace/acmart/figures'
    for root, dirs, files in os.walk(dir_path):
        name_lst = filter(lambda name: 'eps:' in name, [dir_path + os.sep + my_file for my_file in files])
        rename_lst = map(lambda ele: ele.replace('0.', '0dot'), name_lst)
        for prev, cur in zip(name_lst, rename_lst):
            os.system(' '.join(['mv', prev, cur]))
