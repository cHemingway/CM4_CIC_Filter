#!/usr/bin/env python

import numpy as np
import matplotlib.pyplot as plt
import subprocess

length = 100000
A = 1000
M = 1000
square_freq = 0.00005
sin_freq = 0.005

#Names of data and results folders, see makefile
test_temp_folder = "./test_temp/"
test_result_folder = "./test_results/"

def sin_wave(length, freq, A):
	'''Function to generate a sin wave of given length, freq, amplitude'''
	x1 = np.arange(length)
	data = A*np.sin(freq*x1)
	return data


def square_wave(length, freq, A):
	'''Function to generate a square wave of given length, freq, amplitude'''
	x1 = np.arange(length)
	data = np.zeros_like(x1)
	t = 1/(freq)
	for i in range (0,len(x1)):
		data[i] = (A if (i%t) > (t-1)/2 else -A)
	data = data + (A/3)*np.sin(0.005*x1)
	return data

def plot_wave(data, name_prefix, M):
	'''Function to plot a wave before and after the CIC filter'''
	infile = test_temp_folder + name_prefix + ".txt"
	outfile = test_temp_folder + name_prefix + "_out.txt"
	#Name of output image
	plotfile = test_result_folder + name_prefix + "_wave.png"
	#Save the data
	np.savetxt(infile, data, "%d")

	#Call the thing
	subprocess.call("./test_cic %s %d 32 2 2 %s" % (infile,M,outfile), shell=True)

	#Read the output data
	dataout = np.loadtxt(outfile)

	#Plot input
	plt.subplot(211)
	x1 = np.arange(data.size)
	plt.plot(x1, data)
	plt.title("Input")
	#Plot Output
	plt.subplot(212)
	x2 = np.arange(length/M)
	plt.plot(x2, dataout)
	plt.title("Output")
	#Save the file
	plt.savefig(plotfile)
	plt.close()


if __name__ == '__main__':
	plot_wave(sin_wave(length,sin_freq,A), "sin", M)
	plot_wave(square_wave(length,square_freq,A), "square", M)



