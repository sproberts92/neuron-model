import matplotlib.pyplot as plt
import numpy as np

class Plot:
	def __init__(self, averages, file_name):
		self.averages = averages
		self.file_name = file_name

	def process(self, num):
		data = []
		peaks = np.empty(self.averages)

		for av in range(self.averages):
			file_name = '{0}_{1}_{2}.dat'.format(self.file_name, num, av)
			data = np.loadtxt(file_name)

			if data[-1] < 10.0:
				peaks[av] = np.NaN
				continue

			fft_window_min = 1000
			fft_window_max = 5000

			fft_data = np.fft.rfft(data[fft_window_min:fft_window_max])
			fft_data = np.array(list(map(abs, fft_data)))

			fft_freq = np.fft.rfftfreq(fft_window_max - fft_window_min)

			f = np.vectorize(lambda x: 1/x if x != 0 else np.NaN)
			period_lspc = f(fft_freq)

			# Reverse arrays as array must be ascending for interp
			period_lspc = period_lspc[::-1]
			fft_data = fft_data[::-1]

			# Could better resolution in the result be obtained without interp?
			# Or by using some better/different/higher resolution method of interp?
			# Or by fitting a sin function to the original data one the frequency has
			# been determined?

			interp_lspc = np.linspace(0, np.nanmax(period_lspc),  5*np.nanmax(period_lspc))

			print(len(period_lspc), len(fft_data))

			interp_data = np.interp(interp_lspc, period_lspc, fft_data)

			peaks[av] = interp_lspc[np.nanargmax(interp_data)]

			# Save some data for plotting
			if av == 0:
				self.data = data
				self.interp_lspc = interp_lspc
				self.interp_data = interp_data

		self.peaks = peaks
		self.mean = np.nanmean(peaks)

		return self.mean

	def plot_means(self):

		nums = np.array(range(40, 55, 2))

		data = np.empty(nums.size)
		for index, val in zip(range(nums.size), nums):
			data[index] = self.process(val)

		idx = np.isfinite(data) & np.isfinite(nums)

		print(idx)

		poly = np.polyfit(nums[idx], data[idx], 1)

		f = np.vectorize(np.poly1d(poly))

		print(poly)
		print(np.corrcoef(nums, data))

		file_out = open('Output/long_range_period_means.dat', 'w')
		for i in range(len(nums)):
			print("{0} {1}".format(nums[i], data[i]), file=file_out)
		file_out.close()

		plt.scatter(nums[idx], data[idx])
		# plt.errorbar(nums[idx], data[idx], yerr=25)
		plt.plot(nums[idx], (f(nums[idx])))
		plt.show()


	def plot_one(self):

		ax = []
		fig = plt.figure(figsize=plt.figaspect(0.45))
		ax.append(fig.add_subplot(131))
		ax.append(fig.add_subplot(132))
		ax.append(fig.add_subplot(133))

		lspc = np.linspace(0,4999,5000)
		file_out = open('Output/long_range_period.dat', 'w')
		for i in range(len(lspc)):
			print("{0} {1}".format(lspc[i], self.data[i]), file=file_out)
		file_out.close()

		file_out = open('Output/long_range_period_fft.dat', 'w')
		for i in range(len(self.interp_lspc)):
			print("{0} {1}".format(self.interp_lspc[i], self.interp_data[i]), file=file_out)
		file_out.close()

		ax[0].plot(self.data)
		ax[1].plot(self.interp_lspc, self.interp_data)

		ax[0].set_title('Long range activity level')
		ax[1].set_title('Long range activity level FFT')

		ax[0].set_xlabel('Time')
		ax[0].set_ylabel('Activity')

		ax[1].set_xlabel('Period')
		# ax[1].set_ylabel('Magnitude')


		ax[2].scatter(np.linspace(0,self.averages, self.averages), self.peaks)
		ax[2].set_title('Peak period')
		ax[2].set_xlabel('Run')
		ax[2].set_ylabel('Period')
		ax[2].set_ylim([0,1000])


		ax[2].text(0,50,'Mean: {0}'.format(self.mean))

		fig.tight_layout()

		# d = np.array([np.reshape(xv, -1), np.reshape(yv, -1), np.reshape(smoothed, -1)])
		# np.savetxt(".\\output\\for_pgfplots.dat", np.transpose(d))

def main():
	# periodic_activity = Plot(
	# 	xmin = 0,
	# 	xmax = 50000,
	# 	xstep = 1,
	# 	averages = 2,
	# 	file_name = '.\\output\\activity\\50000_large_cycles\\activity__200_35_20',
	# 	title = 'Long range activity level',
	# 	xlabel = 'Time',
	# 	ylabel = 'Activity',
	# 	)

	# periodic_activity = Plot(
	# 	averages = 45,
	# 	file_name = '.\\output\\activity\\5000_80_large_cycles\\activity__200_35',
	# 	)

	periodic_activity = Plot(
		averages = 5,
		file_name = '.\\output\\activity\\activity__200_30',
		)

	# num_stim.plot()
	periodic_activity.process(58)
	periodic_activity.plot_one()
	# periodic_activity.plot_means()
	plt.show()

if __name__ == '__main__':
	main()
