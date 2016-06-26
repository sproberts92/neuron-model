import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as spst
from joblib import Parallel, delayed
from tqdm import trange
import warnings as warn

def main():
    # name = 'length_mean_no_norm'
    # name = 'length_nanmean_no_norm'

    # name = 'length_mean_yes_norm'
    name = 'length_nanmean_yes_norm'


    for i in range(10,51,10):
        length = np.array(Parallel(n_jobs=4)(delayed(process)(k, i, 40) for k in trange(0, 500, 1))).T
        np.save('output/adjacency_processed/{0}_{1}'.format(name, i), length)

    length = np.array(Parallel(n_jobs=4)(delayed(process)(k, 300, 10) for k in trange(10, 500, 10))).T
    np.save('output/adjacency_processed/{0}_{1}'.format(name, 300), length)


def check_matrix_diag(m):
    if 0 in np.diag(m):
        return 0
    else:
        return 1

def process(k, size, av):

    length = np.zeros((av,2))

    for r in range(av):
        path = "./output/adjacency/good_plots/neur_adj_{0}_{1}_{2}.dat".format(size, k, r)
        adj = np.loadtxt(path)

        # Save connectivity parameter
        # length[r, 0] = np.sum(adj) / float(size)
        length[r, 0] = np.sum(adj) / float(size*size)

        cum_paths = adj

        adj_pow = np.dot(adj, adj)
        cum_paths += adj_pow

        for i in range(2,500):
            if check_matrix_diag(cum_paths):
                length[r, 1] = i
                break
            else:
                length[r, 1] = np.NaN
                adj_pow = np.dot(adj_pow, adj)
                cum_paths += adj_pow

    with warn.catch_warnings():
        # To catch "Mean of empty slice" warning
        warn.simplefilter("ignore", category=RuntimeWarning)

        # return np.mean(length,0)
        return np.nanmean(length,0)

if __name__ == '__main__':
    main()
