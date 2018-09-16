.. SphinxTest documentation master file, created by
   sphinx-quickstart on Mon May  7 23:00:23 2018.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to SiMFS-Tk's documentation!
====================================

You found the documentation of **SiMFS-Tk** - the Single Molecule Flourescence
Simulation Toolkit. It is still under active development, so some pats are not
yet complete. For now, check out the installation section to get started.

.. Warning::

   SiMFS-Tk is still under heavy development. Although the core implementation
   is pretty robust, this documentation will still grow during the next weeks.


What is it?
------------

**SiMFS-Tk** is a collection of simple tools to simulate single molecule
fluorescence experiments. Each tool is a commandline program that is configured
by a single json file and reads and writes a set of plain binary input and
output files. By chaining together multiple tools (components), complex
experiments can be simulated. The modular architecture allows easy and quick
iteration of parameters, parallel execution of simulations as well as
inspection of intermediate results and interfacing to third party software.

The project is developed in portable C++11 and built with CMake. Although fully
functional on Linux, MacOS and Windows platforms, some features are not
available under Windows. For Windows users we recommend using Cygwin_.

.. _Cygwin: https://www.cygwin.org

Features
^^^^^^^^
- Free isotropic diffusion and immobilized settings
- Three different focus functions
    - 3D-Gaussian
    - Normalized flux pseudo-Gaussian
    - Ideal lens E-field function
- Independent excitation and detection foci
- Arbitrary pulse definition to model µs and ns ALEX, PIE, POE and other pulse shemes
- State based photophysics for flexible modeling of fluorophores
    - Lifetime interval simulation to bridge short (ps) and long (minutes) timescales
    - Complete output of events in the rate graph (e.g. ISC and RISC events)
    - Event system to react to external factors (molecule movement, laser power, FRET photons, collision with simulation box, ...) 
- Utilities for splitting, mixing and buffering photon timetags
- Highly parallel execution and low memroy usage due to pipeline design
   

Who is it for?
--------------

This toolkit is for everyone who wants to generate simulated single molecule
data to complement experimental data, test and benchmark analysis procedures or
for demonstrative purposes.

How to get started?
-------------------

Clone the repository and run the installation commands. Detailed tutorials will
be available soon.


.. toctree::
   :maxdepth: 2

   installation
   quickstart
   tutorial
   architecture
   components
   api
   contributing




Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`
