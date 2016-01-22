import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d import proj3d
from multiprocessing import Pool
import numpy as np

def f(i, fig, ax):
    sigpath = "output\\signal\\signal_" + str(i) + ".dat"

    points = np.loadtxt(sigpath)
    ax.view_init(30, float(i) / float(545)* 360)
    
    if points.size:
        x, y, z = points.T
        lines = ax.scatter(x, y, z, c='r')
        fig.canvas.draw()

        plt.savefig("output\\frames\\frame" + str(i) + ".png", format='png')
        lines.remove()
    else:
        plt.savefig("output\\frames\\frame" + str(i) + ".png", format='png')
    
    print("Finished processing frame {0}".format(i))

def main():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection ='3d')
    ax.set_axis_off()

    netpath = "output\\Complete_Network.dat"
    points = np.loadtxt(netpath)
    x, y, z = points.T
    net = ax.scatter(x, y, z, c='b', marker='.', s=5, alpha=0.2)

    limits = 40
    ax.set_xlim3d(-limits,limits)
    ax.set_ylim3d(-limits,limits)
    ax.set_zlim3d(-limits,limits)

    ax.view_init(30,0)
    args = [(i, fig, ax) for i in range(545)]
    
    p = Pool(8)
    p.starmap(f, args, 1)

if __name__ == '__main__':
    main()