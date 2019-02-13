# Testing notes

## Library unit tests

### Component

Component contains 4 units:

1. Buffer

    - [x] Buffers are templates with an template argument for the datatype
    - [x] I can initizalize an Output buffer with a unique name id string
    - [x] I can write data item by item to an output
    - [x] I can write a chunk of data as a vector of the approriate data type
    - [x] An output does not block unless an out-of-memory-error occurs
    - [x] I can initizalize an Input buffer with a unique name id string
    - [x] I can read data item by item from an input into a variable
    - [x] I can read a chunk of data as a vector of the approriate data type into a variable
    - [x] I can peek at the next value in the input buffer
    - [x] Inputs can be sorted based on the next pending value
    - [.] An Input does block when there is no data in the buffer
    - [ ] I can optionally read an input in non-blocking mode, the return value indicates if data was available
    - [x] Data written to an Output with id and type can be read only from an Input of the same id an type
    - [x] An Input overrides the bool method and evaluates to false when its corresponding output is done and is destroyed 
    - [x] Threads can push data over the Input-Output interface in parallel
    - [x] There are helper methods to stream data from a file to an Outputs and from a file to an Input
    - [x] By default buffers are written to their id named files
    - [x] There are helper methods that embed the file io into their own threads
    - [.] The size of a chunk can be controlled via an environment variable

2. Component 

    - [x] Component is an abstract base class that can be inherited from
    - [x] I can set parameters via a json object
    - [x] I can get parameters as a json object
    - [x] I can set partial parameters via a json object, with defaults being preserved
    - [x] I can initialize and run a component manually
    - [x] I can initialize and run a component in a thread with optional auto intialization in-place

3. CLI 
    - [ ] NOT YET FINAL

4. Logging
    - [ ] NOT YET IMPLEMENTED


