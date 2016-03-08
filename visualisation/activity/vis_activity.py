from mpl_toolkits.mplot3d import Axes3D
import matplotlib.pyplot as plt
import numpy as np
from scipy import ndimage as nd
from matplotlib import cm
import time

class Plot:
	def __init__(self, xmin, xmax, xstep, ymin, ymax, ystep, averages, zlim, file_name, title, xlabel, ylabel, zlabel, xsmoothing, ysmoothing):
		self.xmin = xmin
		self.xmax = xmax
		self.xstep = xstep
		self.ymin = ymin
		self.ymax = ymax
		self.ystep = ystep
		self.averages = averages
		self.zlim = zlim
		self.file_name = file_name
		self.title = title
		self.xlabel = xlabel
		self.ylabel = ylabel
		self.zlabel = zlabel
		self.xsmoothing = xsmoothing
		self.ysmoothing = ysmoothing

	def plot(self):
		xnum = (self.xmax - self.xmin) / self.xstep
		ynum = (self.ymax - self.ymin) / self.ystep
		data = np.array([]).reshape(0, self.xmax)

		for k in range(self.ymin, self.ymax + 1, self.ystep):
			data_t = []
			for i in range(self.averages):
				file_name = '{0}_{1}_{2}.dat'.format(self.file_name, i, k)
				data_t1 = np.loadtxt(file_name)
				data_t1[data_t1 > self.zlim] = self.zlim
				data_t.append(data_t1)

			av = [sum(e)/len(data_t) for e in zip(*data_t)][0:self.xmax]
			data = np.vstack([data, av])

		ax = []
		f = plt.figure(figsize=plt.figaspect(0.45))
		ax.append(f.add_subplot(121, projection='3d'))
		ax.append(f.add_subplot(122, projection='3d'))
		
		ax[0].set_title(self.title)
		ax[1].set_title(self.title + ' - smoothed')
		for x in [0,1]:
			ax[x].set_xlabel(self.xlabel)
			ax[x].set_ylabel(self.ylabel)
			ax[x].set_zlabel(self.zlabel)
	
		smoothed = nd.gaussian_filter(data, [self.xsmoothing, self.ysmoothing])

		shape = np.shape(smoothed)

		x = np.linspace(self.xmin, self.xmax, shape[1])
		y = np.linspace(self.ymin, self.ymax, shape[0])
		xv, yv = np.meshgrid(x, y)

		ax[0].plot_surface(xv, yv, data,  cmap=cm.jet, cstride=5, rstride=1, linewidth=0)
		ax[1].plot_surface(xv, yv, smoothed,  cmap=cm.jet, cstride=5, rstride=1, linewidth=0)
		f.tight_layout()


def main():
	num_stim = Plot(
		xmin = 0,
		xmax = 500,
		xstep = 1,
		ymin = 0,
		ymax = 116,
		ystep = 5,
		averages = 20,
		zlim = 1000,
		file_name =  '.\\output\\activity\\NumStim_200\\activity__200',
		title = 'Synapse threshold fall rate',
		xlabel = 'Time',
		ylabel = 'No of neurons stimulated',
		zlabel = 'Active neurons',
		xsmoothing = 5,
		ysmoothing = 35
		)

	sym_dec_rate = Plot(
		xmin = 25,
		xmax = 500,
		xstep = 1,
		ymin = 0,
		ymax = 501,
		ystep = 25,
		averages = 20,
		zlim = 1000,
		file_name = '.\\output\\activity\\SynapseDecayRate\\activity__200_0',
		title = 'Synapse threshold fall rate',
		xlabel = 'Time',
		ylabel = 'Synapse threshold fall rate x 10^-3',
		zlabel = 'Active neurons',
		xsmoothing = 5,
		ysmoothing = 35
		)


	num_stim.plot()
	sym_dec_rate.plot()
	plt.show()

if __name__ == '__main__':
	main()
