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

Create a `build` folder and enter it: `mkdir build && cd build`.

Then let CMake generate Makefiles: `cmake ..`

Build the binaries: `make`.

For verbose output use `make VERBOSE=1`.

For a simple showcase of available functions use the binary based on `ecrprog.c` which is located in `build/bin`.

### Testing

Tests are based on [Criterion Testing Framework](https://github.com/Snaipe/Criterion)

Enter directory `test` and compile the suite with `clang -o test_ecr_all test_ecr_all.c -lcriterion -lhiredis` (*applies to GCC as well*)

```shell
$ ./test_ecr_all 
[====] Synthesis: Tested: 13 | Passing: 13 | Failing: 0 | Crashing: 0 
```

# LICENSE

[MIT](https://github.com/brakmic/ECR/blob/master/LICENSE)
