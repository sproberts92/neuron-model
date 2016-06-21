import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as spst
from joblib import Parallel, delayed
from tqdm import trange
import warnings as warn

u = 000
v = 500
s = 10
a = 10
b = 0
n = 300

def main():
    length = np.array(Parallel(n_jobs=4)(delayed(process)(k) for k in trange(u, v, s))).T
    np.save('length_nan_{0}'.format(n), length)

def check_matrix_diag(m):
    if 0 in np.diag(m):
        return 0
    else:
        return 1

def process(k):

    length = np.zeros((a,2))

    for r in range(a):
        # path = "./output/adjacency/good_plots/neur_adj_{0}_{1}_{2}.dat".format(n, k, r)
        path = "./output/adjacency/neur_adj_{0}_{1}_{2}.dat".format(n, k, r)
        adj = np.loadtxt(path)
        # adj = np.random.choice([0, 1], size=(50,50), p=[90./100, 10./100])

        # Save connectivity parameter
        length[r, 0] = np.sum(adj) / float(n*n)

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
        # print(np.mean(length,0))
        # return length
        return np.mean(length,0)

if __name__ == '__main__':
    main()
