import matplotlib.pyplot as plt
import numpy as np

data = []

for i in range(3):
	data.append(np.loadtxt("..\\..\\output\\activity\\activity__100_10_{0}.dat".format(i)))

av = [sum(e)/len(data) for e in zip(*data)]
plt.plot(av)
plt.show()
