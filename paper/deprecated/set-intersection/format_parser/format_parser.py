def zip_report_data(file_path):
    with open(file_path) as ifs:
        lines = ifs.readlines()
        name_lst = lines[0].split(',')
        value_lst = lines[2].split(',')
        print zip(name_lst, value_lst)


if __name__ == '__main__':
    zip_report_data('MyReport-ah.csv')
    zip_report_data('MyReport-macc.csv')
    # zip_report_data('MyReport-ge.csv')
