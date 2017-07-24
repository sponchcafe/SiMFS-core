#! /cygdrive/c/Users/zickmantel/AppData/Local/Continuum/Miniconda3/python.exe

import sys
import numpy as np
from ctypes import Structure, c_double, c_int16
import matplotlib.pyplot as plt
import json
from matplotlib.patches import Ellipse

import argparse

class Coordinate(Structure):
    _fields_ = [('x', c_int16),
                ('y', c_int16),
                ('z', c_int16)]


if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Load simulator parameters.')
    parser.add_argument('-p', '--parameters', type=str)
    parser.add_argument('-w', '--window', type=int)
    args = parser.parse_args()
    parameters = json.load(open(args.parameters, 'r'))
    dif = parameters['./bin/dif']
    exi = parameters['./bin/exiAlpha']
    det = parameters['./bin/detGauss']

    c = Coordinate()
    fig, axes = plt.subplots(1, 2, figsize=(10,5))
    plt.ion()

    axes[0].set_xlim(-dif['radius']*1e+9,dif['radius']*1e+9)
    axes[0].set_ylim(-dif['radius']*1e+9,dif['radius']*1e+9)
    
    axes[1].set_xlim(-dif['radius']*1e+9,dif['radius']*1e+9)
    axes[1].set_ylim(-dif['half-height']*1e+9,dif['half-height']*1e+9)


    cylinder = Ellipse(xy=(0, 0), width=dif['radius']*2e+9, height=dif['radius']*2e+9, fc='None', ec='k')
    axes[0].add_artist(cylinder)

    # plot excitation
    ex_region_xy = Ellipse(xy=(0, 0), width=exi['waist-xy']*1e+9, height=exi['waist-xy']*1e+9, fc='g', alpha=0.25)
    axes[0].add_artist(ex_region_xy)
    ex_region_z = Ellipse(xy=(0, 0), width=exi['waist-xy']*1e+9, height=exi['waist-z']*1e+9, fc='g', alpha=0.25)
    axes[1].add_artist(ex_region_z)
    
    det_region_xy = Ellipse(xy=(det['offset-x']*1e+9, det['offset-y']*1e+9), width=det['waist-x']*1e+9, height=det['waist-y']*1e+9, lw=2, ec='k', fc='None')
    axes[0].add_artist(det_region_xy)
    det_region_z = Ellipse(xy=(det['offset-y']*1e+9, det['offset-z']*1e+9), width=det['waist-x']*1e+9, height=det['waist-z']*1e+9, lw=2, ec='k', fc='None')
    axes[1].add_artist(det_region_z)

    x = np.zeros(args.window)
    y = np.zeros(args.window)
    z = np.zeros(args.window)

    lxy, = axes[0].plot(x, y)
    lxz, = axes[1].plot(x, z)

    plt.tight_layout()
    i=0
    while(sys.stdin.buffer.readinto(c)):
        x[-1] = c.x
        y[-1] = c.y
        z[-1] = c.z
        x = np.roll(x, 1)
        y = np.roll(y, 1)
        z = np.roll(z, 1)
        i+=1
        if(i%(args.window/2) == 0):
            lxy.set_xdata(x)
            lxy.set_ydata(y)
            lxz.set_xdata(x)
            lxz.set_ydata(z)
            plt.pause(0.00001)
