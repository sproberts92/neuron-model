import numpy as np

def main():
	data = np.loadtxt('visualisation/adjacency/celegans_adj.dat')

	d_sum = np.sum(data)
	d_dim = data.shape[0]

	print(d_sum, d_dim, d_sum/d_dim)

if __name__ == '__main__':
	main()
