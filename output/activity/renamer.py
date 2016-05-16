import os, sys

i = 10;

for a in range(30,65,5):
	for b in range(5):
		print("activity__200_35_{0}_{1}.dat".format(a,b))
		print("activity__200_35_20_{0}.dat".format(i))
		os.rename("activity__200_35_{0}_{1}.dat".format(a,b), "activity__200_35_20_{0}.dat".format(i))
		i += 1