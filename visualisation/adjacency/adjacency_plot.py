import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as spst
from scipy.optimize import curve_fit as cf
from joblib import Parallel, delayed
from tqdm import trange
import warnings as warn
from pprint import pprint

colour = ['red', 'green', 'blue', 'yellow', 'orange', 'purple']

# name = 'length_nanmean_no_norm'
# name = 'length_nanmean_yes_norm'
# name = 'length_mean_yes_norm'
name = 'length_mean_no_norm'

# plot_transitions = True
plot_transitions = False

def f(x, a, b, c, d):
    # return a * np.log(b * (x-c)) + d
    # return a* (1 - np.exp(-x/b)) + c
    # return a* np.exp(-(x-c)/b)
    return a/(x-c) + b

def make_plot(i, c, plot=False):
    length = np.load('./output/adjacency_processed/{0}_{1}.npy'.format(name, i))

    # To export data for LaTeX pgfplots
    fp = open('./output/adjacency_processed/adj_{0}_{1}.dat'.format(name, i), 'w')
    for j in length.T:
        print('{0} {1}'.format(j[0], j[1]), file=fp)
    fp.close()

    x = length[0].flatten()
    y = length[1].flatten()

    if plot:
        plt.scatter(x, y, color=colour[c])

    turning_pts = []
    for j in range(len(y)):
        if(y[j] > 5 and y[j] < 9):
            turning_pts.append(x[j])

    # for j in range(len(x)):
    #     if(np.isnan(y[j])):
    #         pass
    #     else:
    #         pass
            # return [i, x[j]]

    turning_pts = np.array(turning_pts)
    # pprint(turning_pts)
    return [i, np.mean(turning_pts)]

def main():
    print(name)

    p = []
    for i in range(10,51,10):
        p.append(make_plot(i, int(i/10-1), plot_transitions))

    p.append(make_plot(300, 5, plot_transitions))
    p = np.array(p)

    np.savetxt('./output/adjacency_processed/midpoints.dat', p)
    # polyfit = np.polyfit(p.T[0], p.T[1], 1)

    # print(polyfit)
    # print(np.corrcoef(p.T[0], p.T[1]))

    # popt, pcov = cf(f, p.T[0], p.T[1])

    if plot_transitions is False:

        xl = np.linspace(10, 300, 300)
        # yl = polyfit[1] + polyfit[0] * xl
        # yl = f(xl, 6.2, 1/40, 2, 1)
        # yl = f(xl, *popt)
        # yl = f(xl, 0.3725, 50.77, 0.00, 3)

        # print(popt)

        plt.scatter(p.T[0], p.T[1])

        # plt.plot(xl,yl)
    else:
        plt.xlim([0,20])
        # plt.yscale('log')
    plt.show()

if __name__ == '__main__':
    main()
