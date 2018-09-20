Pulse
-----

:code:`simfs_pls` transforms a stream of cw focus values (excitation) into a
pulsed pattern.  The pulse shape is configured by a list of time and relative
power values as well as a repitition rate.

Signature
^^^^^^^^^

.. code-block:: bash

   $ simfs_pls [list] < params.json > log.json

- ``list`` : don't run, only parse parameters

Parameters
^^^^^^^^^^

.. json:object:: simfs_pls.json

   Parameters of simfs_pls

   :property string input: Input filename for focus values
   :property string output: Output filename for focus values
   :property float repetition_time: Length of the pulse window in seconds
   :property floats time: Time axis of the pulse shape in seconds
   :property floats power: Relative power axis of the pulse shape

**Example:**

.. code-block:: json

   {
       "input": "__flux_cw__",
       "output": "__flux_pulse__",
       "power": [
           1.0,
           0
       ],
       "repetition_time": 1e-07,
       "time": [
           0,
           1e-9
       ]
   }


Data streams
^^^^^^^^^^^^

==== ====== =========== ========== ============= ==========================
Type Name   Datatype    Size       Layout        Description
==== ====== =========== ========== ============= ==========================
in   input  Timed value 2 x 8 byte (value, time) Input (cw) focus values
out  output Timed value 2 x 8 byte (value, time) Output pulsed foucs values          
==== ====== =========== ========== ============= ==========================

Notes
^^^^^

- The power values are alway normalized such that the average output pulsed power
  is equal to the input cw power. In other words the pulse shape is normalized to
  an area of 1. 
- :code:`simfs_spl` prints out an error if the pulse pattern does not
  fit into the repetition window, i.e. if the last value of the pulse time is
  larger than the reptition rate.
- If the first pulse time point is greater than zero, it is subtracted from the
  whole pulse time axis.
