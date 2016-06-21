import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as spst
from joblib import Parallel, delayed
from tqdm import trange
import warnings as warn

colour = ['red', 'green', 'blue', 'yellow', 'orange', 'purple']

def make_plot(i, c):
    length = np.load('length_nan_{0}.npy'.format(i))

    fp = open('pgf/adj_nan_{0}.dat'.format(i), 'w')

    for j in length.T:
        print('{0} {1}'.format(j[0], j[1]), file=fp)
    plt.scatter(length[0].flatten(), length[1].flatten(), color=colour[c])

def main():


    for i in range(10,51,10):
        make_plot(i, int(i/10-1))

    make_plot(300, 5)
    # plt.xlim([0,20])
    plt.show()

if __name__ == '__main__':
    main()

