import numpy as np
from . import coordinate_t
from . utils import new_filename

from typing import Tuple

Gridspec = Tuple[float, float, int]

###############################################################################
def single_coordinate( 
        x: float, y: float, z: float, t1: float, t0: float=0
        ) -> str:
    ''' Create a single molecular coordinate (x,y,z,t)

    Arguments
    x, y, z : Molecular position in m
    t1 : End time, equals dwell time if t0=0
    t0 : Start time (optional)

    Returns 
    filename of file with a start and an end coordinate
    '''

    c = np.array([(x,y,z,t0),(x,y,z,t1)], dtype=coordinate_t)
    fn = new_filename('single_coordinate')
    c.tofile(fn)
    return fn

###############################################################################
def coordinate_grid(xspec: Gridspec, yspec: Gridspec, zspec: Gridspec) -> str:
    ''' Create a grid of coordinates

    Arguments
    xspec : (min, max, n) range and number of points in x
    yspec : (min, max, n) range and number of points in y
    zspec : (min, max, n) range and number of points in z

    Returns
    filename to coordinate grid file
    '''

    raise NotImplementedError('Not yet implemented')
    return fn
