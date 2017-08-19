import os, sys, re
import numpy as np
import matplotlib.pyplot as plt

pattern_elapsed_str = re.compile(r'.*Time.*')
pattern_time_str = re.compile(r'[0-9]+\.[0-9]+')
pattern_int_str = re.compile(r'[0-9]+')


def draw_bar(tuple_list, title_name):
    N = len(tuple_list)
    bar_one_tuple = ()
    bar_two_tuple = ()
    for my_tuple in tuple_list:
        bar_one_tuple += (float(my_tuple[1]),)
        bar_two_tuple += (float(my_tuple[3]),)
    menMeans = bar_one_tuple
    womenMeans = bar_two_tuple
    menStd = ()
    womenstd = ()
    for i in range(0, 32):
        menStd += (0,)
        womenstd += (0,)
    ind = np.arange(N)  # the x locationget_statistics.py:56s for the groups
    width = 0.8  # the width of the bars: can also be len(x) sequence

    p1 = plt.bar(ind, menMeans, width, color='red', yerr=menStd)
    p2 = plt.bar(ind, womenMeans, width, color='lime',
                 bottom=menMeans, yerr=womenstd)
    plt.ylabel('RunTime/s')

    plt.title(title_name.replace('_', ' '))
    max_val = max(enumerate(bar_two_tuple), key=lambda x: x[1])[1]
    max_val2 = max(enumerate(bar_one_tuple), key=lambda x: x[1])[1]
    max_val = max(max_val, max_val2)
    print 'max_val:' + str(max_val)
    plt.yticks(np.arange(0, max_val * 1.5, round(max_val / 8, 1)))

    second_phase_str = 'Sequential Merge'
    if 'reduce' in title_name:
        second_phase_str = 'Merge With Reduce'

    plt.legend((p1[0], p2[0]), ('Parallel Computation', second_phase_str))
    plt.show()

# my_dir = sys.argv[1]
# my_prefix_str = sys.argv[2]
# pattern_cis = re.compile(my_prefix_str + '_*cis.*')
# pattern_demon = re.compile(my_prefix_str + '_*demon.*')
