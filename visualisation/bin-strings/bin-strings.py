import numpy as np
import matplotlib.pyplot as plt
from tqdm import trange


def choose(n, p_a):
	return np.random.choice(2, n, p=[p_a, 1-p_a])

def main():
	history = choose(1000, 0.5)
	# print(history)
	# return
	w_len = 5

	result = np.zeros(len(history))

	for t in trange(10000):
		
		window = history[:w_len]
		
		for i in range(w_len, len(history) - w_len):
			if (history[i:i+w_len]==window).all():
				result[i] += 1

		history = np.roll(history, 1)
		# history[0] = choose(1, .5)[0]
		history[0] = choose(1, np.sin(t/30)/2+.5)[0]


	# print(result.size)
	np.savetxt('sin_10000.dat', np.stack((np.linspace(0, result.size-1, result.size),result)).T)
	# print(np.mean(result))

	plt.plot(result)
	# plt.show()

if __name__ == '__main__':
	main()
