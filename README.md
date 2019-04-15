# ECR
Execute Collect Report -  a tool for remote code execution and reporting

### Work in Progress

The idea is to have a set of small tools for:

* preparing jobs or *tasks*
* sending them to remote machines
* executing them there 
* and reporting results back

The underlying tech is based on [ØMQ](http://zeromq.org/), [Redis](https://redis.io/) and [cJSON](https://github.com/DaveGamble/cJSON).

### Building

This project uses [CMake](https://cmake.org/download/) so you will have to install it first.

Create a `build` folder and enter it `mkdir build && cd build`.

Then let CMake generate Makefiles: `cmake ..`

Build the binaries with a simple `make` call.

For verbose output use `make VERBOSE=1`.

For quick testing of available functions use the binary based on `ecrprog.c` which is located in `build/bin`.

# LICENSE

[MIT](https://github.com/brakmic/ECR/blob/master/LICENSE)
