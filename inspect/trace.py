#! /cygdrive/c/Users/zickmantel/AppData/Local/Continuum/Miniconda3/python.exe

import sys
import numpy as np
from ctypes import Structure, c_double
import matplotlib.pyplot as plt

class Double(Structure):
    _fields_ = [('v', c_double)]

timestep = 1e-3
clock = timestep

if __name__ == '__main__':
    print("STARTUP")
    d = Double()
    fig, ax = plt.subplots(1)
    plt.ion()
    ax.set_ylim(-1,1000)
    ax.grid()
    i = 0
    data = np.zeros(64)
    line, = ax.plot(data)
    mean = ax.axhline(data.mean(), color='r')
    intensity = 0
    labels = np.linspace(0, timestep*64, 17)
    ax.set_xticks(np.arange(0, 64, 4))
    while(sys.stdin.buffer.readinto(d)):
        intensity += 1
        if (d.v > clock):
            clock += timestep
            data[-1] = intensity
            intensity = 0
            data = np.roll(data, -1)
            labels+=timestep
            line.set_ydata(data)
            mean.set_ydata(data.mean())
            ax.set_xticklabels(labels)
            plt.pause(1e-3)
