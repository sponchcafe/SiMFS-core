Diffusion
---------

:code:`simfs_dif` produces a single molecule diffusion trajectory inside a
cylindrical box. In addition to the random walk coordinates, it outputs a
timetag stream containing timetags for every collision with the simulation box.


Signature
^^^^^^^^^

.. code-block:: bash

   $ simfs_dif [list] < params.json > log.json

- ``list`` : don't run, only parse parameters

Parameters
^^^^^^^^^^

.. json:object:: diffusion.json

   Parameters of simfs_dif

   :property string coordinate_output: Output filename for coordinates
   :property string collision_output: Output filename for collision times
   :property float experiment_time: Experiment time in seconds
   :property float increment: Time increment per diffusion step in seconds
   :property float diffusion_coefficient: Diffusion :math:`D` in :math:`\rm{m \cdot s}^{-2}`
   :property float half_height: Half height of diffusion cylinder in meter
   :property float radius: Radius of cylinder in meter
   :property int seed: Random seed

**Example:**

.. code-block:: json

   {
       "collision_output": "__collisions__",
       "coordinate_output": "__coordinates__",
       "diffusion_coefficient": 1e-10,
       "experiment_time": 1.0,
       "half_height": 1e-06,
       "increment": 1e-07,
       "radius": 1e-06,
       "seed": 1865456046
   }


Data streams
^^^^^^^^^^^^

================= ============ ========== =============== ====================
Name              Datatype     Size       Layout          Description
================= ============ ========== =============== ====================
coordinate_output Coordinate   4 x 8 byte (x, y, z, time) Molecular trajectory
collision_output  Timetag      1 x 8 byte (time)          Collision events
================= ============ ========== =============== ====================

Notes
^^^^^

The simulation cylinder is defined by a radius and a half height. The initial
position is chosen at random somewhere within the box and moved incrementally
for every time increment in x, y and z. The stepsize in all dimension follows
the same normal distrubution. Each step is recorded as an (x, y, z, t) record
in the coordinate output. 

When a coordinate is outside the box (i.e. the molecule would hit the wall of
the box), a collision event is written to the collision output and the
coordinate is discarded. A new attempt to generate a valid coordinate is
calculated until a valid step is acquired.  However, when a second collision
follows the first immediatly, no collision event is outputted.  The event times
in the collision output are thereby guaranteed to be unique.

The diffusion coefficient will be converted to the standard deviation of the
internal normal distribution according to :math:`\sigma^2 = 2\cdot D \Delta t`
with :math:`\Delta t` being the increment of the diffusion step.

