import numpy as np
from collections import namedtuple

basepath = '/Users/tzickmantel/Entwicklung/simfs-tk/build/src/components/'

IO = namedtuple('IO', ['name', 'dtype'])

coordinate_t = np.dtype([('x', 'f8'), ('y', 'f8'),('z', 'f8'), ('t', 'f8')])
timed_value_t = np.dtype([('v', 'f8'), ('t', 'f8')])
timetag_t = np.dtype('f8')

from . component import *
from . presets import *

ip = get_ipython()
ip.register_magics(SimfsDefaultMagics)
