if __name__ == '__main__':
    with open('/home/yche/GitRepos/ScanOptimizing/pSCAN-refactor/out.txt') as ifs:
        twitter_workload_lst = eval(ifs.readlines()[-3])
        print len(twitter_workload_lst)
        print len(set(twitter_workload_lst))
        print max(twitter_workload_lst)
        print sum(twitter_workload_lst) / float(len(twitter_workload_lst))
        print len(filter(lambda ele: ele == 0, twitter_workload_lst))
