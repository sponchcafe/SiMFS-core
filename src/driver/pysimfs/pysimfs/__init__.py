import numpy as np
import os
from collections import namedtuple

here = os.path.dirname(__file__)
basepath = os.path.abspath(os.path.join(here, '/opt/SiMFS-Tk/build/src/components')) # SiMFS-Tk installation path

for comp in ['simfs_dif', 'simfs_sft', 'simfs_cnf']:
    assert os.path.exists(os.path.join(basepath, f'mol/{comp}')), f'mol/{comp} not found in {basepath}'
for comp in ['simfs_fcs', 'simfs_efi', 'simfs_pls']:
    assert os.path.exists(os.path.join(basepath, f'fcs/{comp}')), f'fcs/{comp} not found in {basepath}'
for comp in ['simfs_ph2']:
    assert os.path.exists(os.path.join(basepath, f'ph2/{comp}')), f'ph2/{comp} not found in {basepath}'
for comp in ['simfs_buf', 'simfs_spl', 'simfs_mix']:
    assert os.path.exists(os.path.join(basepath, f'utl/{comp}')), f'utl/{comp} not found in {basepath}'

print(f'All simfs components found in {basepath}.')

IO = namedtuple('IO', ['name', 'dtype'])
ComponentLog = namedtuple('ComponentLog', ['params', 'error'])

coordinate_t = np.dtype([('x', 'f8'), ('y', 'f8'),('z', 'f8'), ('t', 'f8')])
timed_value_t = np.dtype([('v', 'f8'), ('t', 'f8')])
timetag_t = np.dtype('f8')

from . component import *
from . presets import *
from . import mocks

ip = get_ipython()
ip.register_magics(SimfsDefaultMagics)
