#!/usr/bin/env python

import matplotlib.pyplot as plt

M = 50

datain = [];
dataout = [];

with open("sin.txt") as f:
	for line in f:
		datain.append(int(line))

with open("out.txt") as f:
	for line in f:
		dataout.append(int(line))

plt.plot(range(0,len(datain),M),dataout,color="r")
plt.plot(datain,color='g')
plt.show()
plt.savefig("out.png")