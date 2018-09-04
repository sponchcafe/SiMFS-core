Installation
============

*SiMFS-TK* runs on Linux, Mac OS and Windows, although a UNIX system that
supports named pipes is recommended.

.. Note::

   Cygwin_ is a robust UNIX environemnt for Windows that provides a bash
   terminal, named pipes, and the gnu compiler suite. Cygwin is the recommended
   way of using *SiMFS-Tk* on Windows.
   

Prerquisites
------------

Before using *SiMFS-Tk*, make sure you have the following tools:

- git
- a gnu c++ compiler (g++ or clang)
- cmake
- python3 (if you want to use the python driver)

Linux
^^^^^

Use your system package manager (e.g. ``apt``) to install the dependencies.

Mac OSX
^^^^^^^

We recommend homebrew_ to install software dependencies on OSX.

Windows
^^^^^^^

We recommend Cygwin_ to run *SiMFS-TK* on Windows. Make sure you select the
required packages during the installation Cygwin_.


Standard procedure
------------------

The standard procedure after installing all dependencies is cloning the
SiMFS-Tk repository and invoking cmake:

.. code-block:: bash

   $ git clone <url> simfs
   $ mkdir simfs/build
   $ cd simfs/build
   $ cmake ..
   $ make
   $ ctest


.. _Cygwin: https://www.cygwin.org
.. _homebrew: https://www.brew.sh

