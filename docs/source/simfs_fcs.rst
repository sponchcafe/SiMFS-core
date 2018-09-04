Focus
-----

Signature
^^^^^^^^^

.. code-block:: bash

   $ simfs_fcs [list] [exi|det] [gauss|alpha|efield] < params.json > log.json

- ``list`` : don't run, only parse parameters
- ``exi|det`` : choose excitation or detection mode (default: excitation)
- ``gauss|alpha|efield`` : choose focus funcition (default: gauss)

Parameters
^^^^^^^^^^

.. json:object:: fcs.json

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
^^^^^^^^^^^^^^^^^^^

Gaussian excitation: ``simfs_fcs gauss exi < params.json``

.. code-block:: json

   {
       "focus": {
           "waist_x": 2e-07,
           "waist_y": 2e-07,
           "waist_z": 8e-07
       },
       "input": "__coordinates__",
       "output": "__focus__",
       "power": 1e-10,
       "wavelength": 4.88e-07
   }

Gaussian detection: ``simfs_fcs gauss det < params.json``
   
.. code-block:: json
   
   {
       "focus": {
           "waist_x": 2e-07,
           "waist_y": 2e-07,
           "waist_z": 8e-07
       },
       "input": "__coordinates__",
       "output": "__focus__"
   }

Alpha excitation: ``simfs_fcs alpha exi < params.json``
   
.. code-block:: json
   
   {
       "focus": {
           "waist_xy": 2e-07,
           "waist_z": 8e-07
       },
       "input": "__coordinates__",
       "output": "__focus__",
       "power": 1e-10,
       "wavelength": 4.88e-07
   }

   
Alpha detection: ``simfs_fcs alpha det < params.json``

.. code-block:: json
   
   {
       "focus": {
           "waist_xy": 2e-07,
           "waist_z": 8e-07
       },
       "input": "__coordinates__",
       "output": "__focus__"
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

