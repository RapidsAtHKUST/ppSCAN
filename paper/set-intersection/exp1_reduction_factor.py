if __name__ == '__main__':
    with open('exp1-eps1over10-u5.md') as ifs:
        lines = map(lambda my_str: my_str.strip(), ifs.readlines()[4:7])
        print lines
        for line in lines:
            my_data = map(lambda my_str: float(my_str.replace('b', '')), line.split(' | ')[1:])
            reduction_factor = [my_data[1] / my_data[0], my_data[3] / my_data[2], my_data[5] / my_data[4],
                                my_data[7] / my_data[6]]
            print reduction_factor
        print

    with open('exp2-u5-differ-dataset-eps.md') as ifs:
        lines = map(lambda my_str: my_str.strip(), ifs.readlines())
        sublines_lst = [lines[4:8], lines[13:17], lines[22:26], lines[32:36]]
        for lines in sublines_lst:
            for line in lines:
                my_data = map(lambda my_str: float(my_str.replace('b', '')), line.split(' | ')[1:])
                reduction_factor = [my_data[1] / my_data[0], my_data[3] / my_data[2], my_data[5] / my_data[4],
                                    my_data[7] / my_data[6]]
                print reduction_factor
            print
