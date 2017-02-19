import numpy as np

def main():
	f_name = './output/adjacency/neur_adj_100_400_{0}.dat'

	c = np.zeros(30)	
	for i in range(0, len(c)):
		data = np.loadtxt(f_name.format(i))

		d_sum = np.sum(data)
		d_dim = data.shape[0]

		c[i]=(d_sum/d_dim)

	print(c, np.mean(c))

if __name__ == '__main__':
	main()
