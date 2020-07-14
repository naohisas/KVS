# SupportMPI
SupportMPI is a support class library with message passing interface (MPI) for KVS. By checking the flag `KVS_SUPPORT_MPI` in kvs.conf, MPI supported classes are compiled and available.

## Prerequisite
The following software packages are needed to compile SupportMPI.

### MPI Compiler
MPI compiler, such as OpenMPI and MPICH, must be installed in your computing environment before compiling KVS.mpi.

#### Linux
For Linux, OpenMPI will be available by installing libopenmpi-dev. The following shows a command example with apt-get on Ubuntu Linux.

```
$ apt-get install openmpi-bin libopenmpi-dev
```

#### Mac OS X
In case of Mac OS X environment, OpenMPI can be installed with HomeBrew as follows:

```
$ brew install openmpi
```

## Compile
By default, `mpicxx` compiler is used for compiling the SupportMPI. If you want to change the MPI compiler, you can use the environmnet variable `KVS_MPI_CPP` to change it to your specified compiler as follows.

```
e.g.) Using Intel MPI compiler
$ export KVS_MPI_CPP=mpiicpc
```

## Example Program
Several example programs with the SupportMPI can be found in the Example/SupportMPI directory. Each of the programs can be easily compiled with kvsmake command. Note that the `-use_mpi` option need to be specified for the kvsmake to generate a Makefile with MPI comiple otions.

```
$ cd Example/SupportMPI
$ cd HelloWorld
$ kvsmake -G -use_mpi
$ kvsmake
```

The execution result of the "HelloWorld" program is shown as follows:
```
$ ./run.sh
Size = 4, Rank = 0
Size = 4, Rank = 1
Size = 4, Rank = 2
Size = 4, Rank = 3
```

If you use KVS compiled with the different compiler from the mpicxx wrapper compiler, you need to specify the compiler by using the environment variable ```OMPI_CXX``` for OpenMP or ```MPICH_CXX``` for MPICH as follows:

- OpenMP
```
e.g.)
$ OMPI_CXX=g++-6 kvsmake
```

- MPICH
```
e.g.)
$ MPICH_CXX=g++-6 kvsmake
```

### HPC Environment
The followings are setting examples for each HPC environment.

- K computer (RIKEN)
```
$ export KVS_MPI_MPI_CPP=mpiFCCpx
```

- Pi-computer (Kobe University)
```
$ export KVS_MPI_CPP=mpiFCCpx
```

- Pi-VizStudio (Kobe University)
```
$ export KVS_MPI_CPP=mpicxx
```
