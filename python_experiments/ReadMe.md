# Experiments
## Scalability
### Plan

runtime part, add state-of-art pscan runtime as a horizontal line to show parallel algorithm's contribution.

### Problem

* need to interpret the performance degradation in webbase.

![webbase-overview](figures/webgraph_webbase-eps:0.3-min_pts:5-overview.png)

![webbase-speedup](figures/webgraph_webbase-eps:0.3-min_pts:5-runtime-speedup.png)

* need to analyze why lfr benchmark incurs more cost in union-find disjoint-set operation

![lfr-10million-avg15-overview](figures/lfr_10million_avgdeg15-eps:0.3-min_pts:5-overview.png)

![lfr-10million-avg15-speedup](figures/lfr_10million_avgdeg15-eps:0.3-min_pts:5-runtime-speedup.png)

* need to interpret the speedup difference among different input graphs.

### Deprecated

[scalability_figure.py](scalability_figure.py)

```python

# deprecated
def post_process_and_display_tag(statistics_dic, tag_str, title_append_txt=''):
    def post_process():
        sorted_info_lst = sorted(statistics_dic.items(), key=lambda pair: pair[0])

        thread_lst = map(lambda pair: pair[0], sorted_info_lst)
        time_info_lst = map(lambda pair: pair[1], sorted_info_lst)

        time_lst = map(lambda my_dict: my_dict[tag_str], time_info_lst)
        print 'thread list:', thread_lst
        print 'tag time info list:', time_lst
        return thread_lst, time_lst

    # init parameters
    thread_lst, time_lst = post_process()
    tag_list = [prune_time_tag, first_bsp_time_tag, second_bsp_time_tag, core_cluster_time_tag,
                non_core_cluster_time_tag, total_time_tag]
    shape_color_lst = ['bo', 'g^', 'r*', 'ch', 'ys', 'mx']
    shape_color_dict = dict(zip(tag_list, shape_color_lst))

    # draw figures
    plt.plot(thread_lst, time_lst, shape_color_dict[tag_str])
    plt.legend(tag_list)
    font = {'family': 'serif', 'color': 'darkred', 'weight': 'normal', 'size': 12, }
    plt.title('Time cost' + '\n ' +
              title_append_txt if title_append_txt != '' else 'Time cost\n' + tag_str, fontdict=font)
    plt.xlabel('thread num', fontdict=font)
    plt.ylabel('time (ms)', fontdict=font)
    plt.show()
```