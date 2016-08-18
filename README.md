<h1 align="center">Batch Arithmetic Circuit Evaluation (BACE)</h1>

__BACE__ is a C++ library for evaluating arithmetic circuits on batches of inputs.

The protocol is an efficient implementation of the Merlin-Arthur proof system presented in the paper "_Strong ETH Breaks With Merlin and Arthur: Short Non-Interactive Proofs of Batch Evaluation_" (Williams, CCC 2016). The paper can be found [here](https://arxiv.org/pdf/1601.04743v1) and slides can be found [here](http://computationalcomplexity.org/Archive/2016/slides/williams-ccc2016.pdf). The proof system consists of a single message from the (deterministic) prover to the (probabilistic) verifier; the statement being proved is about the correctness of evaluating an arithmetic circuit on a batch of inputs.

The library is developed by [SCIPR Lab] and contributors (see [AUTHORS] file) and is released under the MIT License (see [LICENSE] file). Check out the [__Performance__](#performance) section for runtime data.

## Table of contents

- [Directory Structure](#directory-structure)
- [Build guide](#build-guide)
- [Testing](#testing)
- [Profiling](#profiling)
- [Performance](#performance)

## Directory structure

The directory structure is as follows:

* [__src__](src): C++ source code, containing the following modules:
  * [__arithmetic\_circuit__](src/arithmetic_circuit): interface for arithmetic circuit
  * [__proof\_system__](src/proof_system): prover, verifier, and naive evaluation
  * [__profiling__](src/profiling): profile and plot runtimes
  * [__tests__](src/tests): collection of tests

## Build guide

The library has the following dependencies:

* [CMake](http://cmake.org/)
* [GMP](http://gmplib.org/)
* [gnuplot](http://www.gnuplot.info/)
* [libprocps](http://packages.ubuntu.com/trusty/libprocps3-dev)
* [libff](https://github.com/scipr-lab/libff)
* [libfqfft](https://github.com/scipr-lab/libfqfft)

The library has been tested on Linux, but it is compatible with Windows and Mac OS X.

### Installation

On Ubuntu 14.04 LTS:

```
sudo apt-get install build-essential git cmake gnuplot-x11 libgmp3-dev libprocps3-dev
```

Then follow the build guide for [libff](https://github.com/scipr-lab/libff) and [libfqfft](https://github.com/scipr-lab/libfqfft).

### Compilation

To compile, starting at the project root directory and setup the `build` directory:

```
mkdir build && cd build && cmake ..
```

Then, to compile the library and profiler, run:

```
make
```

The above makes the `build` folder and compiles the profiler to the project root directory. To remove all executables, from the build folder, run `make clean`.

#### Options

The following flags change the behavior of the compiled code:

* `cmake .. -DMULTICORE=ON`
Enables parallelized execution using OpenMP. This will utilize all cores on the CPU for heavyweight parallelizable operations such as FFT.

* `cmake .. -DOPT_FLAGS={ FLAGS }`
Passes specified optimizations flags to compiler.

* `cmake .. -PROF_DOUBLE=ON`
Enables profiling with Double (default: ON). If the flag is turned off, profiling will use `Fr<alt_bn128_pp>`.

## Testing

This library includes unit tests that cover arithmetic circuit, prover, and verifier evaluation. The test suite is easily extensible to support a wide range of fields and domain sizes. To run the tests for this library, after [Compilation](#compilation), run:

```
make check
```

## Profiling

The library profiles runtimes with multi-threading support, and plots the resulting data using [gnuplot](http://www.gnuplot.info/). All profiling and plotting activity is logged under `src/profiling/logs`; logs are sorted into a directory hierarchy by timestamp.

After [Compilation](#compilation), start the profiler by running ```./profile``` from the project root directory. The profiler logs runtimes for the naive evaluation, prover, and verifier, across varying batch and input sizes. Then, it plots graphs comparing naive evaluation with the verifier, naive evaluation with the prover, and runtimes across threads for the naive evaluation, prover, and verifier. Profiling results and plots are saved under ```src/profiling/logs/{datetime}```.

## Performance

__Machine Specification:__ The following benchmark data was obtained on a 64-bit Intel i7 Quad-Core machine with 16GB RAM (2x8GB) running Ubuntu 14.04 LTS. The code is compiled using g++ 4.8.4.

```
Architecture:          x86_64
CPU op-mode(s):        32-bit, 64-bit
Byte Order:            Little Endian
CPU(s):                8
On-line CPU(s) list:   0-7
Thread(s) per core:    1
Core(s) per socket:    2
Socket(s):             4
NUMA node(s):          1
Vendor ID:             GenuineIntel
CPU family:            6
Model:                 94
Stepping:              3
CPU MHz:               4008.007
BogoMIPS:              8016.01
Virtualization:        VT-x
L1d cache:             32K
L1i cache:             32K
L2 cache:              256K
L3 cache:              8192K
NUMA node0 CPU(s):     0-7
```

### Runtime

These runtime benchmarks are determined by constructing an arithmetic circuit of degree 3 using quadratic inner product gates. For every batch size, the input size is varied by powers of two to determine the threshold values for which the verifier's evaluation becomes more cost effective than performing the naive evaluation.

<p align="center"><img src="https://cloud.githubusercontent.com/assets/9260812/17748214/24e61f30-646d-11e6-8c41-5dff1a858e04.png" width="50%"><img src="https://cloud.githubusercontent.com/assets/9260812/17748213/24c6bcbc-646d-11e6-8e52-3da658dda4ad.png" width="50%"></p>

The left graph compares the runtime cost of naive evaluation with the prover's cost of constructing a proof for the verifier. The right graph compares runtime cost of naive evaluation with the verifier's cost of performing a probabilistic check and evaluation from the proof.

[SCIPR Lab]: http://www.scipr-lab.org/ (Succinct Computational Integrity and Privacy Research Lab)

[LICENSE]: LICENSE (LICENSE file in top directory of bace distribution)

[AUTHORS]: AUTHORS (AUTHORS file in top directory of bace distribution)