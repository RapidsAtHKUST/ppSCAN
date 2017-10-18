import matplotlib.pyplot as plt
import numpy as np

x_label = "Percentage of computed source vertices"
v_size = 3.5
d17_knc_we = [18.6413, 38.0379, 55.209, 74.8671, 84.7064, 108.733, 132.209, 150.944, 159.577, 182.999]
d17_knc_hybrid = [19.1711, 34.4665, 52.3645, 75.2577, 91.2469, 107.445, 141.024, 149.422, 156.733, 178.92]
d17_knc_1_deg = [19.3719, 36.3215, 52.8099, 70.2425, 88.0959, 107.775, 126.43, 147.911, 152.238, 176.194]
d17_knc_static = [19.3369, 37.6855, 59.8228, 72.8868, 91.8513, 103.344, 138.005, 142.849, 167.422, 175.782]

d17_knl_we = [7.66781, 14.7215, 21.8407, 29.4956, 37.1857, 44.3433, 50.1171, 58.0227, 66.0501, 71.3788]
d17_knl_hybrid = [8.30653, 14.8145, 21.8704, 29.7544, 36.3582, 43.914, 51.1906, 59.5881, 66.6089, 70.9231]
d17_knl_1_deg = [7.64442, 14.7256, 22.2512, 29.5719, 37.98, 44.8056, 50.1526, 58.5009, 66.5349, 73.0537]
d17_knl_static = [8.05157, 15.6002, 22.1642, 30.2022, 37.7766, 44.1086, 51.5989, 57.8864, 66.5574, 73.7597]

d17_cpu_we = [7.92808, 14.0702, 22.4746, 27.156, 36.6575, 40.5894, 50.9973, 53.8429, 64.2043, 66.1022]
d17_cpu_hybrid = [7.90877, 13.994, 22.3328, 27.328, 36.5353, 40.3604, 50.9741, 53.7826, 64.8632, 66.1333]
d17_cpu_1_deg = [8.16191, 14.1579, 22.3922, 27.4464, 36.7004, 40.2593, 51.4501, 53.5133, 65.2999, 66.725]
d17_cpu_static = [7.41953, 14.2521, 20.8835, 27.7985, 34.8896, 41.047, 48.1257, 54.8109, 62.4582, 68.4668]

d17_gpu_we = [35.44178, 70.8376541, 106.20118, 141.616058, 177.007004, 212.457275, 247.871445, 283.191254, 318.640198,
              353.991241]
d17_gpu_hybrid = [36.5964546, 73.1559601, 109.631493, 146.09584, 182.566376, 219.149506, 255.555191, 292.022369,
                  328.54837, 365.179688]
d17_gpu_1_deg = [36.021328, 72.0097885, 107.974327, 143.984177, 179.958359, 215.897125, 251.892273, 287.840393,
                 323.775177, 359.81424]
d17_gpu_static = [35.3266373, 70.95802, 106.409904, 141.867767, 177.140427, 212.541519, 248.006454, 283.339294,
                  318.882294, 354.26791]

if __name__ == '__main__':
    fig3, (ax4, ax1, ax2, ax3) = plt.subplots(1, 4, sharey=True, sharex=True, figsize=(16, v_size))

    line_styles = ['-', 'o-', 'v-', '-^', '-<', '->', '-s',
                   'p-', '*-', 'd-', 'x', 'D', 'd', '|', '--', '-.', '.-', ',-', '-+']
    line_lables = ['Work-effcient-only', 'Hybrid with sampling', '1-degree reduction', 'GPU hybrid_BC', "static"]

    # fig1
    xtick1 = np.arange(1, 11, 1)
    ax1.set_xlim(0.5, 10.5)
    ax1.set_xticks(xtick1)
    ax1.set_xticklabels(['10%', '20%', '30%', '40%', '50%', '60%', '70%', '80%', '90%', '100%'])
    line1, = ax1.plot(xtick1, d17_knc_we, '.-', label=line_lables[0], linewidth=1.0, markersize=10)
    line2, = ax1.plot(xtick1, d17_knc_hybrid, 'v-', label=line_lables[1], linewidth=1.0, markersize=7)
    line3, = ax1.plot(xtick1, d17_knc_1_deg, '--', label=line_lables[2], linewidth=1.0, markersize=10)
    line4, = ax1.plot(xtick1, d17_knc_static, '*-', label=line_lables[4], linewidth=1.0, markersize=7)

    ax2.set_xticks(xtick1)
    ax2.set_xticklabels(['10%', '20%', '30%', '40%', '50%', '60%', '70%', '80%', '90%', '100%'])
    ax2.plot(xtick1, d17_knl_we, '.-', label=line_lables[0], linewidth=1.0, markersize=10)
    ax2.plot(xtick1, d17_knl_hybrid, 'v-', label=line_lables[1], linewidth=1.0, markersize=7)
    ax2.plot(xtick1, d17_knl_1_deg, '--', label=line_lables[2], linewidth=1.0, markersize=10)
    ax2.plot(xtick1, d17_knl_static, '*-', label=line_lables[4], linewidth=1.0, markersize=7)

    ax3.set_xticks(xtick1)
    ax3.set_xticklabels(['10%', '20%', '30%', '40%', '50%', '60%', '70%', '80%', '90%', '100%'])
    ax3.plot(xtick1, d17_cpu_we, '.-', label=line_lables[0], linewidth=1.0, markersize=10)
    ax3.plot(xtick1, d17_cpu_hybrid, 'v-', label=line_lables[1], linewidth=1.0, markersize=7)
    ax3.plot(xtick1, d17_cpu_1_deg, '--', label=line_lables[2], linewidth=1.0, markersize=10)
    ax3.plot(xtick1, d17_cpu_static, '*-', label=line_lables[4], linewidth=1.0, markersize=7)

    ax4.set_xticks(xtick1)
    ax4.set_xticklabels(['10%', '20%', '30%', '40%', '50%', '60%', '70%', '80%', '90%', '100%'])
    ax4.plot(xtick1, d17_gpu_we, '.-', label=line_lables[0], linewidth=1.0, markersize=10)
    ax4.plot(xtick1, d17_gpu_hybrid, 'v-', label=line_lables[1], linewidth=1.0, markersize=7)
    ax4.plot(xtick1, d17_gpu_1_deg, '--', label=line_lables[2], linewidth=1.0, markersize=10)
    ax4.plot(xtick1, d17_gpu_static, '*-', label=line_lables[4], linewidth=1.0, markersize=7)

    ax1.yaxis.set_ticks(np.arange(0, 360, 50))
    ax2.yaxis.set_ticks(np.arange(0, 360, 50))
    ax3.yaxis.set_ticks(np.arange(0, 360, 50))
    ax4.yaxis.set_ticks(np.arange(0, 360, 50))

    ax1.grid(True)
    ax2.grid(True)
    ax3.grid(True)
    ax4.grid(True)

    # Create new figure and two subplots, sharing both axes
    # ax1.scatter(19,150)
    # ax1.plot((18,19),(k40[7],k40_half_d[0]), linestyles[9], label='k40_half')

    # ax1.legend(loc=0,prop={'size':11})
    ax4.set_ylabel('Time in seconds')

    ax4.set_xlabel(x_label)
    ax1.set_xlabel(x_label)
    ax2.set_xlabel(x_label)
    ax3.set_xlabel(x_label)

    ax4.set_title('(a) GPU')
    ax1.set_title('(b) KNC')
    ax2.set_title('(c) KNL')
    ax3.set_title('(d) CPU')

    fig3.legend((line1, line2, line4, line3),
                ('work-efficient-only', 'sampling-dynamic-switching', 'sampling-static-switching',
                 '1-degree work-efficient'),
                loc=2, bbox_to_anchor=[0.789, 0.89])

    fig3.subplots_adjust(wspace=0)
    plt.tight_layout()
    plt.savefig('delaunay_n17_scale_three.pdf', dpi=1200)
    fig3.show()
