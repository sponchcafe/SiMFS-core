<style>
	h2 {
		page-break-before: always;
	}
</style>

# SiMFS-Tk: Single Molecule Fluorescence Simulator Toolkit

[![Version Badge](https://img.shields.io/badge/Version-0.0.1-yellow.svg)](https://shields.io/)


## Introduction

**TODO**

## Installation guide

```bash
git clone <repo>
cd <repo>
mkdir build
cd build
cmake ..
make
ctest
```

## Architecture

SiMFSTk is organized into components that solve individual tasks in the simulation process, for example generation of a diffusion trajectory or evaluation of a focus function. Each component is defined as a `C++` class and has a UNIX-shell command line interface for testing and building simulation scripts. Components can be parametrized via a parameter json document. Data flows into and out of components via the input and output interface. In case of the CLI, in- and outputs map directly to filestreams. To tackle complex simulation problems, components are chained together and form complex pipelines using UNIX named pipes.  

### Data types

SiMFSTk data types rely on double precision floating point numbers to represent most simulation data. As a general rule physical quantities should be represented in SI units without prefix. 

```cpp
#ifndef SIMFS_TYPES_H
#define SIMFS_TYPES_H

namespace simfs{

    //-Meters------------------------------------------------------------//
    typedef double coordinate_t;

    //-Seconds-----------------------------------------------------------//
    typedef double realtime_t;

    //-Timed-double-value------------------------------------------------//
    typedef struct{
        double value;
        realtime_t time;
    } TimedValue;

    //-SI-Coordinate-----------------------------------------------------//
    typedef struct{
        coord_t x;
        coord_t y;
        coord_t z;
        realtime_t t;
    } Coordinate;
    
}

#endif
```
    
The CLI component binaries read and write these types in binary format. Compressed storage can be achieved by zipping. Inspection of binary streams is readily done by the standard UNIX `od` utility.


### CLI binaries

CLI binaries wrap a component's class into a executable program that can be configured via a parameter json file. The binaries are prefixed `simfs_` followed by a three letter name. Example: `simfs_dif` - the diffusion component. Data is read from input files or streams (pipes) and written to output files or streams.  

#### General options

Proposal:  
`simfs_xxx < params.json --list | jq .param.path=new_value > simfs_xxx`. 

- read parameters froms stdin
- pass to `jq` to modify
- pass to component and run.

`--help -h` : Prints a short help text to stdout  
`--list -l` : Prints the currently accepted parameter specficiation as json to stdout  
`--set -s </path/to/parameter=value>` : Sets a json parameter accoring to a json pointer  
`--patch -p <json string>` : Patch the parameter settings with a partial or full parameter json patch document  


#### Parameter passing

Checkout [`jq`](https://stedolan.github.io/jq/) for parameter file manipulation from within the shell.


 

## Component: Diffusion

> Binary: `simfs_dif`  
> Class: `Diffusion` in `src/components/dif/include/dif.hpp`  

Generates a diffusion trajectory of a single molecule within a cylindrical experimental box. The trajectory is a series of $(x, y, z, t)$ coordinates with a fixed time increment. Diffusion steps are calculated according to the same normal distribution in all three dimensions (isotropic) and written to the primary output. On collision with the experimental box boundaries, a collision coordiante with time and location is generated and written to a secondary output. The collision is handled via a trial and error procedure: When a coorindate outside the box is generated, it is discarded and a collision coordinate with the currently valid molecule postition and time is written to the collision output. The diffusion step is repeated until a valid coordinate is generated. Only the first failed attempt will cause a collsion coordiante to be written. Failed attempts do not progress the experiment time.


#### CLI parameters:  

###### JSON specfication (defaults)

```json
{
	"diffusion_coefficient": 1e+10,
	"experiment_time": 1,
	"increment": 1e-7,
	"half_height": 1e-6,
	"radius": 1e-6,
	"seed": 0,
	"trajectory_output": "./trajectory",
	"collision_output": "/dev/null"`
}
```

###### diffusion_coefficient

The molecules diffusion coefficient $D$ in $\rm{m\cdot s}^{-2}$. Will be converted to the standard deviation of the internal normal distribution according to $\sigma^2 = 2\cdot D \Delta t$ with $\Delta t$ being the time increment of the diffusion step.

###### experiment_time

Experimental time $t_\rm{max}$ in $\rm{s}$. For all coordinates $(x, y, z, t)_\rm{n}$:  $t_{\rm{n}} \leq t_\rm{max}$.

###### increment
  
Time increment $\Delta t$ in $\rm{s}$. For all coordinates $t_{\rm{n}} - t_{\rm{n}-1} = \Delta t$.

###### half_height

z-Dimension of the simulation box $z_{\rm{max}}$ in $\rm{m}$. For all coordinates $|z_{\rm{n}}| \leq z_{\rm{max}}$

###### radius

xy-Dimension of the simulation box $r_{\rm{max}}$ in $\rm{m}$. For all coordinates $r_{\rm{n}} \leq r_{\rm{max}}$ with $r_{\rm{n}}(x, y, z, t) = \sqrt{x^2+y^2}$

###### seed

Random seed for the random number generator as unsignd integer. The default special value 0 instructs the component to generate a new seed itself.

###### trajectory_output
  
Output identifier for the trajectory coordinate data. In the CLI context, this is a filepath. Coordinates $(x, y, z, t)$ will be written to the filestream as binary stream data.

###### collision_output

Output identifier for the collision coordinate data. In the CLI context, this is a filepath. Coordinates $(x, y, z, t)$ will be written to the filestream as binary stream data. By default data is written to `/dev/null` and will be discarded.


## Component: Excitation

> Binary: `simfs_exi`  
> Class: `Excitation` in `src/components/exi.cpp`  

Evaluates an excitation focus function at a coordinate $(x, y, z, t)$ and produces a stream of timed photon flux density values $(F, t)$. 


#### CLI parameters:  

```json
{
	"focus_function": {
		"type": "alpha | gauss",
		<...> // shape pararmeters
	},
	"power": 10e-6,
	"wavelength": 488e-9,
	"pulse": {
 	  "offset": [], 
 	  "amplitude": []
 	},
 	"trajectory_input": "./trajectory",
 	"flux_output": "./flux"
	
}
```

###### focus_function
*available:* `"alpha"`, `"gauss"`

Type specifier for the focus function to be used for evaluation. Each focus function can require arbitrary shape parameters. See the documentation of available excitation focus funcitons below.

###### power
  
Excitation power in Watt. This value is equivalent to the expreimentally determined laser power effectively reaching the sample volume.

###### wavelength $(\lambda)$

Excitation laser wavelength in nm.

###### pulse

Pulse shape specificaiton. `offset` and `amplitude` are the $x$ and $y$ values for the pulse shape function. Empty lists signify continuous wave excitation. The `offset` values start at 0 and determine time points relative to the pulse window. Each `offset` value needs to map to an `amplitude` value, that scales the excitation power at that given offset time. The pulse window is reperated indefinetly on top of the processed coordinate trajectory. The pulse `amplitude` scalings are normalized to preserved the total power level specified by `power`.  

*Example:*  

```json
"pulse": {
	"offset": [0, 1e-9, 100e-9],
	"amplitude": [1, 0, 0]
}
```

specifies a one nanosecond rectangular pulse at the beginning of a 100 nanosecond pulse window, i.e. a repetition rate of 10 MHz.

###### trajectory_input

Input identifier for the trajectory coordinate data. In the CLI context, this is a filepath. Coordinates $(x, y, z, t)$ will be read from the filestream as binary stream data.
 
###### flux_output
  
Output identifier for the photon flux density data. In the CLI context, this is a filepath. Photon flux values $(F, t)$ will be written to the filestream as binary stream data.


## Component: Detection

> Binary: `simfs_det`  
> Class: `Detection` in `src/components/det.cpp`  

Evaluates a detection focus function at a coordinate $(x, y, z, t)$ and produces a stream of timed detection efficiency values $(E, t)$. Efficiency values are effectively detection probabilities $E(t) = P(t) \in [0, 1]$.


#### CLI parameters:  

```json
{
	"focus_function": "{
		"type": "alpha | gauss",
		<...> // shape pararmeters
	},
	"max_efficiency": 1,
	"trajectory_input": "./trajectory",
	"efficiency_output": "./efficiency"
}
```

###### foucs_function

*available:* `"alpha"`, `"gauss"`

Type specifier for the focus function to be used for evaluation. Each focus function can require arbitrary shape parameters. See the documentation of available detection funcitons below.

###### max_efficiency

Maximum efficiency of the detection function. $E_\textrm{max} = P_\textrm{max} \in [0, 1]$. 

###### trajectory_input
    
Input identifier for the trajectory coordinate data. In the CLI context, this is a filepath. Coordinates $(x, y, z, t)$ will be read 
from the filestream as binary stream data.

###### efficiency_output
  
Output identifier for the photon flux density data. In the CLI context, this is a filepath. Detection efficiency values $(E, t)$ will be written to the filestream as binary stream data.


## Component: Photophysics

> Binary: `simfs_ph2`  
> Class: `Photophysics` in `src/components/ph2.cpp`  

Core simulation component for fluorophore photophysics simulations modeled by a generalized Jablonsky diagram graph.

#### Model

The electronic states and transition rates that constitute the Jablonsky diagram, are mapped to the internal graph model as nodes and edges respectively. Each edge has a transition rate, that is used to generate transition lifetimes. Starting at the initial node, all outgoing edges generate a lifetime based on its current rate and the path of the shortest lifetime is traversed. This static traversal is extended to dynamic simulations by actions. Actions are small tasks that can be linked to both nodes and edges and are executed when their parent is traveresed by the simulation. In addition, events can be issued by the simulator that trigger further actions at a specific time in the simulation. In this way, dynamic updates can be read from input streams and applied to the graph simulation as necessary. Each update issues the next update as an event in the simulation timeline.  
Actions provide a powerful interface to extend and customize the photophysics simulation. Available actions include:

- dynamic updating of an edges transition rate depending on an external stream of excitation photon flux density
- emission of timetags on traversal of a desired path in the graph (e.g. an photon emitting transition)
- handling of virtual FRET photons, i.e. putting the graph into an excitated state if excitation is possible at the time of the transferred photon and rejecting it otherwise

**Note:** Inputs and outputs are exclusively handled (and owned) by graph actions. In this way additional inputs and outputs can be attached and removed as necessary, just by registering new actions with the graph.

#### CLI parameters


```json
{
	"seed": 0,
	"start_at": "S0",
	"jablonsky": {  
		"exi": {
			"from": "S0", 
			"to": "S1", 
			"rate": 1e+8
		},
		"emi": {
			"from": "S1", 
			"to": "S0", 
			"rate": 1e+8
		},
		"isc": {
			"from": "S1", 
			"to": "T1", 
			"rate": 1e+8 
		},
		"risc": {
			"from": "T1", 
			"to": "S0", 
			"rate": 1e+8
		}
	},
	"actions": {
		"excitation": {
			"type": "excitation",
			"target_edges": {
				"exi": {
					"epsilon": 73000 // absorption coefficient in M-1 cm-1
				}
			},
			"input": "./excitation"
		},
		"emssion": {
			"type": "emission",
			"target_edge": "emi",
			"output": "emi"
		},
		"transfer": {
			"type": "transfer",
			"target_edge": "exi",
			"source_node": "S0",
			"input": "./transfer",
			"output": "./rejected"
		}
	}
}
```


###### seed

Random seed for the random lifetime generator as unsignd integer. The default special value 0 instructs the component to generate a new seed itself.

###### start_at

Specifies the initial state of the graph, i.e. the node to start the simulation from. `<nodename>` has to exist in the `jablonsky` graph parameters.

###### jablonsky
 
Spceification of the jablonsky diagram. Each entry defines a unique transition in the Jablonsky diagram. The `from` and `to` parameters define the connected states. The full set of existing nodes is inferred from the edge specification, i.e. a node without any connection cannot exist (which makes sense). `rate` specifies a fixed transtion rate $r$ in $s^{-1}$. 

###### actions
 
Actions are triggered when their associated edge is traversed. The `target_edge` has to exist in `jablonsky`. The action type defines the desired action. Available actions are:

- `"excitation"`: Read excitation photon flux density from an input `input` and update all target edges rates according to the excitation and their respective `epsilon` parameter as absorption coefficient in $M^{-1} cm^{-1}$).
- `"emission"`: Write timetags to an output `output`.
- `"transfer"`: Reads transfer photons from its `input` and traverses the `target_edge` at the photon time if the graph is currently in the `source_node`.  Otherwise the photon is rejected and passed back to the `output`.


## Component: Conformation

> Binary: `simfs_cnf`  
> Class: `Conformation` in `src/components/cnf.cpp`  

Produces transfer efficiencies for a double labeled molecule (FRET). The component uses the same graph model as the photophysics. States are molecular conformations with an associated static trasnfer efficiency. Arbitrary state transitions can be specified as nodes in the transition graph. The transition rates model dynamic conformational changes of the molecule.


#### CLI parameters

```json
{
	"efficiencies": {
		"OPEN": 0.3,
		"CLOSED": 0.7
	},
	"transitions": {
		"open": {
			"from": "CLOSED",
			"to": "OPEN",
			"rate": 1e+2
		},
		"close": {
			"from": "CLOSED",
			"to": "OPEN",
			"rate": 1e+2
		}
	},
	"output": "./transfer_efficiency"
}
```

###### efficiencies

Pairs of nodenames and transfer efficiency $E \in [0, 1]$.

###### transitions

Edge specification for the transition graph. Each entry is a transtion `from` nodename `to` nodename. The `rate` specifies the conformational transition rate in $s^{-1}$. The connected nodenames have to be existent in `efficiencies`.


## Component: Immobilization

> Binary: `simfs_put`  
> Class: `Immobilization` in `src/components/put.cpp`  

Produces two coordinates $c_0 = (x, y, z, t_0)$, $c_1 = (x, y, z, t_1)$ at the same position separated by a time delta $\Delta t = t_1-t_0$. It is used to put a molecule to a specific position for a specific time interval.

### CLI parameters

```json
{
	"x": 0,
	"y": 0,
	"z": 0,
	"t0": 0,
	"t1": 1
}
``` 

###### x, y, z

$x$, $y$ and $z$ coordinate in m.

###### t0, t1

Start and end time in s.


## Component: Photon splitter

> Binary: `simfs_spl`  
> Class: `Splitter` in `src/components/put.cpp`  
 
Splits a photon input stream into two output streams based on a stochastic cointoss experiment. The success probability is updated dynamically (as timed values $(P, t)$) from a secondary input. 

### CLI parameters

```json
{
	"photon_input": "emission",
	"efficiency_input": "efficiency",
	"accepted_output": "./accepted",
	"rejected_output": "/dev/null"
}
```

###### photon_input

Input identifier for the photon stream to be split. In the CLI context, this is a filepath. Timetags will be read from the filestream as binary stream data.

###### efficiency_input

Input identifier for the efficiency values to use on the photon stream. In the CLI context, this is a filepath. Timed probability values $(P, t)$  will be read from the filestream as binary stream data.

###### accepted_output

Output identifier for accepted timetags. In the CLI context, this is a filepath. Accepted timetags will be written to the filestream as binary stream data.

###### rejected_output

Output identifier for rejected timetags. In the CLI context, this is a filepath. Rejected timetags will be written to the filestream as binary stream data. Defaults to "/dev/null" and discards rejected photons.

### Use cases

- *Detection function*: Reads detection values from `simfs_det` and detects the primary photon stream from `simfs_ph2` according to the detection efficiency stream.
- *FRET*: Reads in transfer efficiencies from `simfs_cnf`, extracts FRET photon candidates from a primary emission stream of `simfs_ph2` and passes them to a second instance (acceptor) of `simfs_ph2`.
- *Data reduction*: Reduce the amount of data in a photon stream without changing the underlying distribution. 


## Component: Photon mixer

> Binary: `simfs_mix`  
> Class: `Mixer` in `src/components/put.cpp`  

Mixes together a list of timetag input streams to one output stream. Requires the inputs to be strictly ascending. If all $t^\text{in}_\text{n} \leq t^\text{in}_\text{n+1}$ for all individual inputs, then the output contains all photons of the inputs together and  $t^\text{out}_\text{n} \leq t^\text{out}_\text{n+1}$ for all timetags in the output.

### CLI parameters

```json
{
	"inputs": [],
	"output": "./mixed"
}
```

###### inputs

List of input identifiers for the photon streams to be mixed. In the CLI context, these are filepaths. Timetags will be read from the filestream as binary stream data.

###### output

Output identifier for mixed timetags. In the CLI context, this is a filepath. Mixed timetags will be written to the filestream as binary stream data.

### Use cases

- *Mix molecules*: When simulating multiple molecules, mix the individual molecules to one output timetrace.
- *FRET*: Remix rejected FRET photons passed back from the acceptor `simfs_ph2` back into the donor emission stream.

---

### Tee (*)

*Use UNIX standard `tee`*

### Buffer (*)

*Use [`mbuffer`](http://www.maier-komor.de/mbuffer.html)*

### Correlator (*)

### Anisotropic diffusion (*)

### Liquid flow (*)

---

## Focus functions

**TODO**

$$F_\text{alpha}(x, y, z) = \frac{1}{\frac{\pi}{2} w_{xy}^2} \cdot \exp{\left(-2\cdot\left(\frac{z^2}{w_z^2} + \frac{(x^2+y^2)\cdot \exp{\left(-2\frac{z^2}{w_z^2}\right)}}{w_{xy}^2}\right)\right)}$$


## Extend and Integrate

While the CLI components provide an easy way to write custom simulation scripts, inspect and store intermediate data streams and extend the simulation pipeline with new (foreign) components, the component classes can be integrated into systems different from the UNIX shell as well. The component interface uses the json document for arbitrary paramerization of the component logic. Json is easy to read and produce from many software systems and can be passed as a serialized string. A small wrapper that takes a json string and parses it via the included `nlohmann::json` is enough to set component defaults and patch parameters,  
The inputs and outputs are passed to the component class via template arguments:
`template <template<typename T> Input<T>, template<typename T> Output<T>>`


### Component interface

```cpp
<template
	<template class InputT> class InputT,
	<template class OutputT> class OutputT
> Component;

virtual void Component<InputT, OutputT>::run() = 0;
```


### IO interface

#### Vector IO 
(serial)

#### Queue IO 
(thread parallel)

#### File IO 
(serial, process parallel)

### Parameter interface
    


## Library documentation

### Structure

    src
   	├── lib
   	│	├── component
	│	├── focus
	│	├── graph
	│	├── correlator*
	│	└── random
	└── components
		├── dif
		├── exi
		├── det
		├── ph2
		├── mix
		├── spl
		├── cnf
		├── tee*	
		├── buf*
		├── put
		└── bkg
		
	
### Graph

### Random

### Focus

### Component

### Correlate


## Roadmap