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
|OpenGL|KVS_ENABLE_OPENGL|Flag for enabling OpenGL functionalities. If '0' (disable) is specified for this flag, the renderer classes implemented with OpenGL functions in the visualization modules will be unavailable.|
|GLU|KVS_ENABLE_GLU|Flag for enabling GLU (OpenGL Utility Library) functionalities. KVS can be complied even if this flag is zero.|
|GLEW|KVS_ENABLE_GLEW|Flag for enabling GLEW (OpenGL Extension Wrangler Library) functionalities. Note: GLEW needs to be installed to compile KVS on Windows environments.|
|OpenMP|KVS_ENABLE_OPENMP|Flag for enabling OpenMP functionalities. OpenMP supported compiler is required.|
|(Deprecated functions)|KVS_ENABLE_DEPRECATED|Flag for enabling the deprecated functions and classes in KVS. Note: Although the deprecated functions and classes can be available by checking this flag, but not recommended.|
|GLUT|KVS_SUPPORT_GLUT|Flag for supporting GLUT functions. The screen class based on the GLUT is provided. See [SupportGLUT](Source/SupportGLUT) for setting information.|
|GLFW|KVS_SUPPORT_GLFW|Flag for supporting GLFW functions. The screen class based on the GLFW is provided. Note: GLUT or GLFW is required for developing viewer application with KVS. See [SupportGLFW](Source/SupportGLFW) for setting information.|
|OpenCV|KVS_SUPPORT_OPENCV|Flag for supporting OpenCV functions. Note: OpenCV4 is not supported.|
|Qt|KVS_SUPPORT_QT|Flag for supporting Qt classes. The screen class based on the Qt.|
|Python|KVS_SUPPORT_PYTHON|Flag for supporting Python in KVS. By using these classes, python codes can be called in c++ codes.|
|MPI|KVS_SUPPORT_MPI|Flag for supporting MPI functions. Note: MPI compiler is required. See [SupportMPI](Source/SupportMPI) for setting information.|
|EGL|KVS_SUPPORT_EGL|Flag for supporting EGL (The Khronos Native Platform Graphics Interface) functions. By checking this flag, GPU-based off-screen class is provided. See [SupportEGL](Source/SupportEGL) for setting information.|
|OSMesa|KVS_SUPPORT_OSMESA|Flag for supporting OSMesa (Off-screen Mesa) functions. By checking this flag, CPU-based off-screen class is provided. See See [SupportOSMesa](Source/SupportOSMesa) for setting information.|

## Configuration
The environment variable ```KVS_DIR``` should be set to an install directory of KVS, and the binary directory ```KVS_DIR/bin``` which includes several KVS commands should be add to the PATH variable.

### Linux/Mac
```
export KVS_DIR=~/local/kvs
export PATH=$KVS_DIR/bin:$PATH
```

### Windows

|Variable|Value|
|:-------|:----|
|KVS_DIR |C:\Program Files\kvs|
|PATH|%PATH%;%KVS_DIR%\bin|

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

# Copyright
KVS is released under BSD 3-Clause License. See [LICENSE](LICENSE) for details.

# Citation
Naohisa Sakamoto and Koji Koyamada, [KVS: A simple and effective framework for scientific visualization](https://www.jstage.jst.go.jp/article/jasse/2/1/2_76/_article/-char/en), Journal of Advanced Simulation in Science and Engineering (JASSE), Vol. 2, Issue 1, pp. 76-95, 2015.
