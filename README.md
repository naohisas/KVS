Kyoto Visualization System
===
Kyoto Visualization System (KVS) is a multi-platform, open-source C++ library for developing scientific visualization applications. KVS provides various classes and interfaces, such as isosurace extraction, streamlines and volume rendering, to visualize medical data obtained by computerized tomography or magnetic resonance imaging, numerical data from computational fluid dynamics and so on.

# Installation

## Requirements
* GLEW
* GLUT
* Qt (optional)
* CUDA (optional)
* OpenCV (optional)

## Settings

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

+ Windows
```
nmake
nmake install
```

+ Linux and Mac OS X
```
make
make install
```
