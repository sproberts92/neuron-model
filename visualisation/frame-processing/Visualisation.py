import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d import proj3d
from multiprocessing import Pool
import numpy as np
from joblib import Parallel, delayed
from tqdm import trange
import warnings as warn

signal_path = 'output\\signal\\signal_{0}.dat'
network_path = 'output\\Complete_Network.dat'
neuron_path = 'output\\Complete_Network_Neurons.dat'

def f(i, fig, ax, signals=0):
    ax.view_init(30, float(i) / float(545)* 360)

    if signals:
        lines = plot_scatt(signal_path.format(i), ax, c='r')
        if lines:
            fig.canvas.draw()
            save_fig(i)
            lines.remove()
        else:
            save_fig(i)
    else:
        save_fig(i)

def save_fig(i):
    plt.savefig('output\\frames\\frame' + str(i) + ".png", format='png')

def load_points(path):
    with warn.catch_warnings():
        # To catch "Empty input file" warning
        warn.simplefilter('ignore', category=UserWarning)
        return np.loadtxt(path)

def plot_scatt(path, ax, **kwargs):

    points = load_points(path).T

    if points.size:
        x, y, z = points
        return ax.scatter(x, y, z, **kwargs)
    else:
        return None

def main():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection ='3d')
    ax.set_axis_off()

    plot_scatt(network_path, ax, c='b', marker='.', s=5, alpha=0.02)
    plot_scatt(neuron_path, ax, c='b', marker='.', s=50, alpha=0.5)

    limits = 40
    ax.set_xlim3d(-limits,limits)
    ax.set_ylim3d(-limits,limits)
    ax.set_zlim3d(-limits,limits)

    ax.view_init(30,0)

    Parallel(n_jobs=4)(delayed(f)(i, fig, ax, True) for i in trange(545))

if __name__ == '__main__':
    main()
