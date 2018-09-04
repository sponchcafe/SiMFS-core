Diffusion
---------

Signature
^^^^^^^^^

.. code-block:: bash

   $ simfs_dif [list] < params.json > log.json

- ``list`` : don't run, only parse parameters

Parameters
^^^^^^^^^^

.. json:object:: dif.json

   Parameters of simfs_dif

   :property string coordinate_output: Output filename for coordinates
   :property string collision_output: Output filename for collision times
   :property float experiment_time: Experiment time in seconds
   :property float increment: Time increment per diffusion step in seconds
   :property float diffusion_coefficient: Diffusion :math:`D` in :math:`\rm{m \cdot s}^{-2}`
   :property float half_height: Half height of diffusion cylinder in meter
   :property float radius: Radius of cylinder in meter
   :property int seed: Random seed


Example params.json
^^^^^^^^^^^^^^^^^^^^^^

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

Notes
^^^^^

Diffusion is simulated in a cylinder defined by a radius and a half height. The
"molecule" is placed somewhere random within the cylinder and moved
incrementally for every time increment in x, y and z. The stepsize in all
dimension follows the same normal distrubution. Each step is recorded as an (x,
y, z, t) record in the coordinate output. When the molecule hits the wall, the
time (t) is recorded in the collision output.

The diffusion coefficient will be converted to the standard deviation of the
internal normal distribution according to :math:`\sigma^2 = 2\cdot D \Delta t`
with :math:`\Delta t` being the increment of the diffusion step.

