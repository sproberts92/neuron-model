import matplotlib.pyplot as plt
import numpy as np

f_name = './output/three_loops/tl_{0}_{1}_{2}_{3}.dat'

n = 50

fracs = np.zeros(n)

for i in range(n):

	data = np.zeros(300)
	for j in range(300):
		data[j] = np.loadtxt(f_name.format(100, i, 10*i, j))

	fracs[i] = np.sum(data)/data.size

fracs = np.stack((np.linspace(0, 49, 50), fracs))
print(fracs.T)

np.savetxt('./output/three_loops/processed.dat', fracs.T)

plt.plot(fracs[0], fracs[1])
plt.show()
