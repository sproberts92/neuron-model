import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from multiprocessing import Pool

def read_points(path):
    f = open(path, 'r')

    points = []
    for line in f:
        pos = [float(x) for x in line.split()]
        points.append(pos)

    return points

def f(i, fig, ax):
    sigpath = "output\\signal\\signal_" + str(i) + ".dat"

    points = read_points(sigpath)

    ax.view_init(30, 0.3 * i)

    if points:
        lines = ax.scatter(*zip(*points), c='r')
        fig.canvas.draw()

        plt.savefig("output\\frames\\frame" + str(i) + ".png", format='png')
        lines.remove()
    else:
        plt.savefig("output\\frames\\frame" + str(i) + ".png", format='png')
    
    print("Finished processing frame {0}".format(i))

def main():
    fig = plt.figure()
    ax = fig.add_subplot(111, projection = '3d')

    netpath = "output\\Complete_Network.dat"
    points = read_points(netpath)
    net = ax.scatter(*zip(*points), c='b', marker='.', s=5, alpha=0.1)

    limits = 50
    ax.set_xlim3d(-limits,limits)
    ax.set_ylim3d(-limits,limits)
    ax.set_zlim3d(-limits,limits)

    ax.view_init(30,0)

    args = [(i, fig, ax) for i in range(500)]
    
    p = Pool(8)
    p.starmap(f, args, 1)

if __name__ == '__main__':
    main()