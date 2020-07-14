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

## Support software package
KVS supports the following software packages. By checking (changing 0:disable to 1:enable) the enable/support flags corresponding to each package in [kvs.conf](https://github.com/naohisas/KVS/blob/develop/kvs.conf), the functionalities provided from them can be available in KVS.

|Package|Flag|Description|
|:--|:--|:--|
|OpenGL|KVS_ENABLE_OPENGL|Flag for enabling OpenGL functionalities.|
|GLU|KVS_ENABLE_GLU|Flag for enabling GLU (OpenGL Utility Library) functionalities.|
|GLEW|KVS_ENABLE_GLEW|Flag for enabling GLEW (OpenGL Extension Wrangler Library) functionalities. Note: GLEW needs to be installed to compile KVS on Windows environments.|
|OpenMP|KVS_ENABLE_OPENMP|Flag for enabling OpenMP functionalities. OpenMP supported compiler is required.|
|(Deprecated functions)|KVS_ENABLE_DEPRECATED|Flag for enabling the deprecated functions and classes in KVS. Note: Although the deprecated functions and classes can be available by checking this flag, but not recommended.|
|GLUT|KVS_SUPPORT_GLUT|Flag for adding GLUT supported classes.|
|GLUT|KVS_SUPPORT_GLFW|Flag for adding GLFW supported classes. Note: GLUT or GLFW is required for developing viewer application with KVS.|
|OpenCV|KVS_SUPPORT_OPENCV|Flag for adding OpenCV supported classes. Note: OpenCV4 is not supported.|
|Qt|KVS_SUPPORT_QT|Flag for adding Qt supported classes.|
|Python|KVS_SUPPORT_PYTHON|Flag for adding Python supported classes. By using these classes, python codes can be called in c++ codes.|
|MPI|KVS_SUPPORT_MPI|Flag for adding MPI supported classes. Note: MPI compiler is required.|
|EGL|KVS_ENABLE_EGL|Flag for adding EGL (OpenGL ES core functions) supported classes. By checking this flag, GPU-based off-screen classes are available.|
|OSMesa|KVS_ENABLE_OSMESA|Flag for adding OSMesa (Software OpenGL implementation) supported classes. By checking this flag, CPU-based off-screen classes are available.|

## Configuration
The environment variable ```KVS_DIR``` should be set to an install directory of KVS, and the binary directory ```KVS_DIR/bin``` which includes several KVS commands should be add to the PATH variable.

+ Windows

|Variable|Value|
|:-------|:----|
|KVS_DIR |C:\Program Files\kvs|
|PATH|%PATH%;%KVS_DIR%\bin|

+ Linux and Mac OS X
```
export KVS_DIR=~/local/kvs
export PATH=$KVS_DIR/bin:$PATH
```

## Build and install
By using the make command, KVS can be easily build and install to ```KVS_DIR```.

+ Windows
```
$ cd KVS
$ nmake
$ nmake install
```

+ Linux and Mac OS X
```
$ cd KVS
$ make
$ make install
```
