Kyoto Visualization System
===
[![Join the chat at https://gitter.im/naohisas/KVS](https://badges.gitter.im/naohisas/KVS.svg)](https://gitter.im/naohisas/KVS?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Kyoto Visualization System (KVS) is a multi-platform, open-source C++ library for developing scientific visualization applications. KVS provides various classes and interfaces, such as isosurace extraction, streamlines and volume rendering, to visualize medical data obtained by computerized tomography or magnetic resonance imaging, numerical data from computational fluid dynamics and so on.

# Installation

## Getting KVS
The source code of KVS can be downloaded via GitHub as follows:
```
$ git clone https://github.com/naohisas/KVS.git
```

## Requirements
Some of software are required for compiling KVS. Please install them as needed before building KVS.
* GLUT
* GLEW (optional but recommended)
* Qt (optional)
* CUDA (optional)
* OpenCV (optional)

## Configuration
The environment variable ```KVS_DIR``` should be set to an install directory of KVS, and the binary directory ```KVS_DIR/bin``` which includes several KVS commands should be add to the PATH variable.

+ Windows

|Variable|Value|
|:-------|:----|
|KVS_DIR |C:\Program Files\kvs|
|PATH|%PATH%;%KVS_DIR%\bin|

+ Linux and Mac OS X
```
export KVS_DIR=/usr/local/kvs
export PATH=$KVS_DIR/bin:$PATH
```

## Build and install
By using make command, KVS can be easily build and install to ```KVS_DIR```.

+ Windows
```
$ cd
$ nmake
$ nmake install
```

+ Linux and Mac OS X
```
$ cd
$ make
$ make install
```
