#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import subprocess

M = 10
length = 10000
n_steps = 400.0
A = 1000
freq = 0.1

#Names of data and results folders, see makefile
test_temp_folder = "./test_temp/"
test_result_folder = "./test_results/"
#Names of data in/out file
infile = test_temp_folder + "sin.txt"
outfile = test_temp_folder + "sin_out.txt"
#Name of output image
plotfile = test_result_folder + "sin_wave.png"

#Generate Input Data
x1 = np.arange(length)
datain = A*np.sin(2*np.pi*freq*x1)
#datain = A* np.ones(length)	
np.savetxt(infile, datain, "%d")

#Call the thing
subprocess.call("./test_cic %s %d 32 2 2 %s" % (infile,M,outfile), shell=True)

#Read the output data
dataout = np.loadtxt(outfile)

#Plot input
plt.subplot(211)
plt.plot(x1, datain)

#Plot Output
plt.subplot(212)
x2 = np.arange(length/M)
plt.plot(x2, dataout)

plt.savefig(plotfile)