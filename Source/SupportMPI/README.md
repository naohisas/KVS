# SupportMPI

SupportMPI provides MPI-based parallel processing classes for KVS.

## Enable this support

Edit `kvs.conf`:

```make
KVS_SUPPORT_MPI = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_MPI=ON
```

## Requirements

SupportMPI requires an MPI implementation such as Open MPI, MPICH, Intel MPI, or Fujitsu MPI.

## Install dependencies

### Linux

Open MPI can be installed on Ubuntu as follows:

```sh
sudo apt-get install openmpi-bin libopenmpi-dev
```

MPICH can be installed as follows:

```sh
sudo apt-get install mpich libmpich-dev
```

### macOS

```sh
brew install open-mpi
```

or:

```sh
brew install mpich
```

### Windows

Use an MPI implementation compatible with your compiler environment.

## MPI compiler variables

The make-based build uses MPI compiler wrapper variables when SupportMPI is enabled.

| Variable | Default | Description |
|:--|:--|:--|
| `KVS_MPI_CPP` | `mpicxx` | MPI C++ compiler wrapper |
| `KVS_MPI_CC` | `mpicc` | MPI C compiler wrapper |
| `KVS_MPI_FC` | `mpif90` | MPI Fortran compiler wrapper |
| `KVS_MPI_LD` | `$(KVS_MPI_CPP)` | MPI linker command |

Example:

```sh
export KVS_MPI_CPP=mpiicpc
```

Manual path settings are also available:

```sh
export KVS_MPI_INCLUDE_PATH=/path/to/mpi/include
export KVS_MPI_LIBRARY_PATH=/path/to/mpi/lib
export KVS_MPI_LINK_LIBRARY="-lmpi"
```

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export KVS_MPI_CPP=mpicxx
make
make install
```

If the MPI wrapper uses a different underlying C++ compiler from the compiler used to build KVS, set the wrapper-specific variable.

For Open MPI:

```sh
OMPI_CXX=clang++ kvsmake
```

For MPICH:

```sh
MPICH_CXX=clang++ kvsmake
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_MPI=ON \
  -DMPI_CXX_COMPILER=mpicxx
cmake --build build -j
cmake --install build
```

You can also use `KVS_MPI_CPP`:

```sh
KVS_MPI_CPP=mpicxx cmake -S . -B build -DKVS_SUPPORT_MPI=ON
```

## Build examples

After installing KVS, generate an MPI-enabled Makefile with `-use_mpi`:

```sh
cd Example/SupportMPI/HelloWorld
kvsmake -G -use_mpi
kvsmake
```

Run the program with `mpirun` or the provided script:

```sh
mpirun -np 4 ./HelloWorld
```

or:

```sh
./run.sh
```

Example output:

```text
Size = 4, Rank = 0
Size = 4, Rank = 1
Size = 4, Rank = 2
Size = 4, Rank = 3
```

## HPC environment examples

```sh
# Fujitsu MPI
export KVS_MPI_CPP=mpiFCCpx
```

```sh
# Open MPI or MPICH
export KVS_MPI_CPP=mpicxx
```
