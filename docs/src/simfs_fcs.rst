Focus
-----

:code:`simfs_fcs` evaluates a foucs function on a stream of molecular coordinates.

Signature
^^^^^^^^^

.. code-block:: bash

   $ simfs_fcs [list] [exi|det] [gauss|alpha|efield] < params.json > log.json

- ``list`` : don't run, only parse parameters
- ``exi|det`` : choose excitation or detection mode (default: excitation)
- ``gauss|alpha|efield`` : choose focus funcition (default: gauss)

Parameters
^^^^^^^^^^

**Mode:**

.. json:object:: excitation.json

   Parameters of simfs_fcs exi

   :property string input: Coordinate input filename
   :property string output: Flux density output filename 
   :property float power: Excitation power in Watt
   :property float wavelength: Excitation wavelength in m
   :property object focus: Focus function parameters

.. json:object:: detection.json

   Parameters of simfs_fcs det

   :property string input: Coordinate input filename
   :property string output: Detection efficiency output filename 
   :property object focus: Focus function parameters

**Kind:**

.. json:object:: gauss.json

   Additional foucus parameters of simfs_fcs gauss 

   :property float waist_x: :math:`\frac{1}{e^2}` waist in x of gauss focus.
   :property float waist_y: :math:`\frac{1}{e^2}` waist in y of gauss focus.
   :property float waist_z: :math:`\frac{1}{e^2}` waist in z of gauss focus.

.. json:object:: alpha.json

   Additional foucus parameters of simfs_fcs alpha 

   :property float waist_xy: :math:`\frac{1}{e^2}` waist in xy of alpha focus.
   :property float waist_z: :math:`\frac{1}{e^2}` waist in z of alpha ocus.

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

Excitation focus function evaluate a photon flux density, according to a total
power level. Detection functions are normalized to values between 0 and 1.

