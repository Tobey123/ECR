# ECR

*Execute*, *Collect*, and *Report* -  a tool set for remote code execution, data collection and reporting

### Work in Progress

The idea is to develop a set of networking tools for:

* preparing jobs which can be simple one-liners or complex scripts
* sending them to remote machines for execution
* collecting data and reporting it back

The underlying tech is based on [ØMQ](http://zeromq.org/), [Redis](https://redis.io/) and [cJSON](https://github.com/DaveGamble/cJSON).

The code is based on [C99](https://en.wikipedia.org/wiki/C99) and can be compiled under **Windows**, **macOS** and **Linux**.

![msvc_ide](https://raw.githubusercontent.com/brakmic/bazaar/master/images/ecr/ecr_msvc.png)

![macos_ide](https://raw.githubusercontent.com/brakmic/bazaar/master/images/ecr/ecr_macos.png)

### Building

This project uses [CMake](https://cmake.org/download/) to generate project files for your preferred build environment.

Create a `build` folder and enter it: `mkdir build && cd build`.

Then let CMake generate Makefiles: `cmake ..`

Build the binaries: `make`.

For verbose output use `make VERBOSE=1`.

For a simple showcase of available functions use the binary based on [ecrprog.c](https://github.com/brakmic/ECR/blob/master/ecrprog.c) which is located in `build/bin`.

### Testing

Tests are based on [Criterion Testing Framework](https://github.com/Snaipe/Criterion)

Enter directory `test` and compile the suite with `clang -o test_ecr_all test_ecr_all.c -lcriterion -lhiredis` (*applies to GCC as well*)

```shell
$ ./test_ecr_all 
[====] Synthesis: Tested: 13 | Passing: 13 | Failing: 0 | Crashing: 0 
```

### Use-Cases

* Managing server farms via script-execution
* Automatized reporting
* Software delivery and updates
* Health-checks & heartbeats 

# LICENSE

[MIT](https://github.com/brakmic/ECR/blob/master/LICENSE)
