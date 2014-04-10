#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import subprocess

M = 10
length = 10000
n_steps = 400.0
A = 1000
freq = 0.1

#Generate Input Data
x1 = np.arange(length)
datain = A*np.sin(2*np.pi*freq*x1)
#datain = A* np.ones(length)	
np.savetxt("sin.txt", datain, "%d")

#Call the thing
subprocess.call("./test_cic %s %d 32 2 2" % ("sin.txt",M), shell=True)

#Read the output data
dataout = np.loadtxt("out.txt")

#Plot input
plt.subplot(211)
plt.plot(x1, datain)

#Plot Output
plt.subplot(212)
x2 = np.arange(length/M)
plt.plot(x2, dataout)

plt.savefig("sin_wave.png")