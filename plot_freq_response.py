#!/usr/bin/env python

import subprocess
import matplotlib.pyplot as plt
import numpy as np


M = 1000
length = 10000
n_steps = 400.0
A = 1000

gains = []

#For each frequency
for freq in np.linspace(0,0.5,n_steps):
    #Generate input data
    with open("sin.txt","w") as f:
        for i in xrange(0, length):
            f.write("%d \n" % (A *np.sin(2*np.pi*freq*i)) )

    #Call the thing
    subprocess.call("./test_cic %s %d" % ("sin.txt",M), shell=True)

    #Read the output data
    dataout = []
    with open("out.txt") as f:
        for line in f:
            dataout.append(int(line))

    dataout = np.array(dataout)
    gains.append(np.sqrt(np.mean(dataout**2))) #Append RMS

plt.semilogy(gains)
plt.savefig("freq_resp.png")
plt.show()
