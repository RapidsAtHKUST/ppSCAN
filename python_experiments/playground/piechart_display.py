from matplotlib import pyplot as plt
import numpy as np


def display_pie_charts():
    data1 = np.array([0.9, 0.1])
    data2 = np.array([0.6, 0.4])

    # create a figure with two subplots
    fig, (ax1, ax2) = plt.subplots(1, 2)

    # plot each pie chart in a separate subplot
    ax1.pie(data1)
    ax2.pie(data2)

    ax1.legend(['1st', '2nd'])
    ax2.legend(['1st', '2nd'])
    plt.show()


def display_custom_xticks0():
    x = [1, 2, 3, 4]
    my_xticks = ['John', 'Arnold', 'Mavis', 'Matt']
    plt.plot(x, [1.3, 3.1, 0.3, 2.2], 'b*')
    plt.xticks(x, my_xticks, rotation=45)

    plt.legend(['tmp'])
    plt.show()


def display_stack_bar_chart():
    # a stacked bar plot with errorbars
    N = 5
    menMeans = (20, 35, 30, 35, 27)
    womenMeans = (25, 32, 34, 20, 25)
    # menStd = (2, 3, 4, 1, 2)
    # womenStd = (3, 5, 2, 3, 3)
    ind = np.arange(N)  # the x locations for the groups
    width = 0.35  # the width of the bars: can also be len(x) sequence

    # p1 = plt.bar(ind, menMeans, width, color='#d62728', yerr=menStd)
    # p2 = plt.bar(ind, womenMeans, width,
    #              bottom=menMeans, yerr=womenStd)
    p1 = plt.bar(ind, menMeans, width, color='#d62728')
    p2 = plt.bar(ind, womenMeans, width, bottom=menMeans)

    plt.ylabel('Scores')
    plt.title('Scores by group and gender')
    plt.xticks(ind, ('G1', 'G2', 'G3', 'G4', 'G5'))
    plt.yticks(np.arange(0, 81, 10))
    plt.legend((p1[0], p2[0]), ('Men', 'Women'))

    plt.show()


if __name__ == '__main__':
    display_stack_bar_chart()
    display_custom_xticks0()
