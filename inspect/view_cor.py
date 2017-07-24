#! /cygdrive/c/Users/zickmantel/AppData/Local/Continuum/Miniconda3/python.exe

import sys
import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':

    data = np.loadtxt(sys.argv[1], skiprows = 1)
    c, t, e = data[:,0], data[:,1], data[:,2]

    fig, ax = plt.subplots(1, figsize=(10,5))
    ax.semilogx(t, c)
    ax.grid(which='both', alpha=0.5, ls='--')
    plt.show()

    



