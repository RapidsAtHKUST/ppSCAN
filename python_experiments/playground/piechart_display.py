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


if __name__ == '__main__':
    display_custom_xticks0()
