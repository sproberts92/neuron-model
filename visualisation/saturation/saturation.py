import numpy as np
import matplotlib.pyplot as plt

def main():

	f_name = './output/activity/activity__100_{0}_{1}_{2}.dat'

	data_pts = 30

	frac = np.zeros(data_pts)
	for i in range(data_pts):

		repeats = 50
		count = 0
		for j in range(repeats):
			data = np.loadtxt(f_name.format(i, i*10, j))

			if(data[-1]):
				count += 1

		frac[i] = count/repeats

	frac = np.stack((np.linspace(0, data_pts-1, data_pts), frac))

	np.savetxt('./output/activity/saturation.dat ', frac.T)

	plt.scatter(frac[0], frac[1])
	plt.show()

if __name__ == '__main__':
	main()
