from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np

xmin = 0
xmax = 300
xstep = 1
xnum = xmax / xstep

ymin = 0
ymax = 70
ystep = 10
ynum = ymax / ystep

averages = 50

data = np.array([]).reshape(0,xmax)

x = np.linspace(xmin + xstep, xmax, xnum)
y = np.linspace(ymin + ystep, ymax, ynum)

xv, yv = np.meshgrid(x, y)

for k in range(ystep, ymax + 1, ystep):
	data_t = []
	for i in range(averages):
		data_t.append(np.loadtxt(
			"..\\..\\output\\activity\\activity__100_{1}_{0}.dat"
			.format(i, k)))
	
	av = [sum(e)/len(data_t) for e in zip(*data_t)][0:xmax]
	data = np.vstack([data, av])

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

ax.set_xlabel('Time')
ax.set_ylabel('FWM for connectivity')
ax.set_zlabel('Active neurons')

ax.plot_surface(xv, yv, data)
plt.show()
