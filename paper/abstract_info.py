with open('abstract.txt') as ifs:
    lines = ifs.readlines()
    print len(''.join(lines).split())
