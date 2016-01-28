import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("..\\..\\output\\activity.dat")

plt.plot(data)
plt.show()
