#! /Users/tzickmantel/System/anaconda3/bin/python

import sys
from collections import deque
import matplotlib.pyplot as plt
import numpy as np
import time

window = 100
bin_width = 1e-3

class Trace():

    def __init__(self, path, ax):
        self.stream = open(path, 'rb')
        self.data = np.zeros(window)#deque(np.zeros(window), maxlen=window)
        self.ax = ax
        self.line, = self.ax.plot(self.data, label=path, lw=1)

    def read(self):
        raw = self.stream.read(4*window)
        if not raw:
            return False
        self.data = np.frombuffer(raw, dtype=np.int32)
        if self.data.size < window:
            self.data = np.pad(self.data, (0, window-self.data.size), mode='constant')
        self.line.set_ydata(self.data)
        return True

fig, ax = plt.subplots(1, figsize=(10, 4))
fig.show()
ax.set_ylim(-1, 1000)
ax.set_xlim(0, window)
ax.grid()
ax.set_xlabel('{}x 1ms'.format(window))
ax.set_ylabel('counts')
plt.tight_layout()

exp_time = ax.annotate(xy=(0.01, 0.95), s='', xycoords='axes fraction')
real_time = ax.annotate(xy=(0.01, 0.9), s='', xycoords='axes fraction')
factor = ax.annotate(xy=(0.01,0.85), s='', xycoords='axes fraction')
cps = ax.annotate(xy=(0.01,0.8), s='', xycoords='axes fraction')

traces = []
for f in sys.argv[1:]:
    traces.append(Trace(f, ax))

ax.legend(loc='upper right')

bins = 0
start = time.time()

while any([t.read() for t in traces]):
    
    bins+=window

    elapsed = round(time.time() - start, 3)
    simulated = round(bins*bin_width, 3)
    
    exp_time.set_text('Exp. time: {:>3.1f} s'.format(simulated))
    real_time.set_text('Real time: {:>3.1f} s'.format(elapsed))
    factor.set_text('Factor: {:>3.1f}'.format(round(elapsed/simulated,6)))
    cps.set_text('Mean cps: {:>6d}'.format(np.int(np.sum(traces[0].data)*1/(bin_width*window))))
    
    top = round(max([max(t.data) for t in traces]), -3)+1000
    if top > ax.get_ylim()[1]:
        ax.set_ylim(-0.1*top, top)

    plt.pause(1/window)

plt.pause(-1)