#!/usr/bin/env python
import argparse
from math import sin, pi


parser = argparse.ArgumentParser(description='Write n samples of sin wave to a file')
parser.add_argument('length', type=int, default=1000, help='The number of samples')
parser.add_argument('filename', type=str, default='sin.txt', help='The output file name')
parser.add_argument('--freq', type=float, default=0.01, help='Normalised Frequency, 1=Fs')
parser.add_argument('--scale', type=float, default=65536/2)
args = parser.parse_args()


with open(args.filename,"w") as f:
    for i in xrange(0, args.length):
        f.write("%d \n" % (args.scale *sin(2*pi*args.freq*i)) )