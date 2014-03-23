#!/usr/bin/env python

import subprocess
import matplotlib.pyplot as plt
import numpy as np


M = 1000
length = 10000
n_steps = 400.0
A = 2000
A_rms = A / np.sqrt(2.0)

gains = []
freqs = np.linspace(0,0.5,n_steps)

#For each frequency
for freq in freqs:
    #Generate input data
    with open("sin.txt","w") as f:
        for i in xrange(0, length):
            f.write("%d \n" % (A *np.sin(2*np.pi*freq*i)) )

    #Call the thing
    subprocess.call("./test_cic %s %d" % ("sin.txt",M), shell=True)

    #Read the output data
    dataout = np.zeros((length,1))
    i = 0
    with open("out.txt") as f:
        for line in f:
            dataout[i] = int(line)
            i+=1

    gains.append(np.sqrt(np.mean(dataout**2))/A_rms) #Append RMS

plt.semilogy(freqs, gains)
plt.ylabel("Gain (dB)")
plt.xlabel("Normalised Frequency")
plt.savefig("freq_resp.png")
plt.show()
