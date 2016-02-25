from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from scipy import ndimage as nd
from matplotlib import cm

def smooth(m, x_pts, y_pts):
    box = np.ones(((x_pts+1), (y_pts+1)))/((x_pts+1) * (y_pts+1))
    y_smooth = sg.convolve2d(m, box, mode='same')
    return y_smooth

xmin = 0
xmax = 500
xstep = 1
xnum = xmax / xstep

ymin = 0
ymax = 116
ystep = 5
ynum = ymax / ystep

averages = 20

data = np.array([]).reshape(0,xmax)

th = 1000

for k in range(ymin, ymax + 1, ystep):
	data_t = []
	for i in range(averages):
		file_name = "..\\..\\output\\activity\\activity__200_{0}_{1}.dat".format(k, i)
		data_t1 = np.loadtxt(file_name)
		data_t1[data_t1 > th] = th
		data_t.append(data_t1)

	av = [sum(e)/len(data_t) for e in zip(*data_t)][0:xmax]
	data = np.vstack([data, av])

ax = []
f = plt.figure(figsize=plt.figaspect(0.45))
ax.append(f.add_subplot(121, projection='3d'))
ax.append(f.add_subplot(122, projection='3d'))

ax[0].set_title('Number of neurons stimulated')
ax[1].set_title('Number of neurons stimulated - smoothed')
for x in [0,1]:
	ax[x].set_xlabel('Time')
	ax[x].set_ylabel('No of neurons stimulated')
	ax[x].set_zlabel('Active neurons')

smoothed = nd.gaussian_filter(data, [5, 35])

shape = np.shape(smoothed)

x = np.linspace(xmin, xmax, shape[1])
y = np.linspace(ymin, ymax, shape[0])
xv, yv = np.meshgrid(x, y)

ax[0].plot_surface(xv, yv, data,  cmap=cm.jet, cstride=5, rstride=1, linewidth=0)
ax[1].plot_surface(xv, yv, smoothed,  cmap=cm.jet, cstride=5, rstride=1, linewidth=0)
f.tight_layout()
plt.show()
