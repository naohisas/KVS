Kyoto Visualization System
===
[![Join the chat at https://gitter.im/naohisas/KVS](https://badges.gitter.im/naohisas/KVS.svg)](https://gitter.im/naohisas/KVS?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

Kyoto Visualization System (KVS) is a multi-platform, open-source C++ library for developing scientific visualization applications. KVS provides classes and interfaces for isosurface extraction, streamlines, volume rendering, image processing, and related visualization tasks.

# Installation

## Getting KVS

```
git clone https://github.com/naohisas/KVS.git
cd KVS
```

## Installation directory

KVS is installed into the directory specified by `KVS_DIR`.

### Linux / macOS

```
export KVS_DIR=$HOME/local/kvs
export PATH=$KVS_DIR/bin:$PATH
```

### Windows

| Variable | Value |
|:--|:--|
| `KVS_DIR` | `C:\Program Files\kvs` |
| `PATH` | `%PATH%;%KVS_DIR%\bin` |

# Configuration

KVS build options are mainly controlled by `kvs.conf` and environment variables.

`kvs.conf` contains feature flags such as `KVS_ENABLE_OPENGL` and `KVS_SUPPORT_QT`.

```
KVS_ENABLE_OPENGL     = 1
KVS_ENABLE_GLU        = 1
KVS_ENABLE_GLEW       = 0
KVS_ENABLE_OPENMP     = 0
KVS_ENABLE_DEPRECATED = 0

KVS_SUPPORT_GLUT      = 1
KVS_SUPPORT_GLFW      = 0
KVS_SUPPORT_FFMPEG    = 0
KVS_SUPPORT_OPENCV    = 0
KVS_SUPPORT_QT        = 0
KVS_SUPPORT_PYTHON    = 0
KVS_SUPPORT_MPI       = 0
KVS_SUPPORT_EGL       = 0
KVS_SUPPORT_OSMESA    = 0
```

Set a flag to `1` to enable the corresponding feature or support library.

# Build with make

## Linux / macOS

```
make
make install
```

## Windows

```
nmake
nmake install
```

## Common make targets

| Target | Description |
|:--|:--|
| `make` or `make build` | Build KVS |
| `make install` | Install KVS into `KVS_DIR` |
| `make clean` | Remove build files |
| `make rebuild` | Clean and build again |
| `make uninstall` | Remove the installed `KVS_DIR` directory |
| `make reinstall` | Uninstall and install again |

## make build options

The following variables can be passed as environment variables or command-line variables.

```
KVS_CPP_STANDARD=17 make
KVS_CPP_STANDARD=17 make install
```

```
KVS_CPP=clang++ KVS_CC=clang make
```

On macOS, `KVS_MACOSX_DEPLOYMENT_TARGET` can be used when a specific deployment target is required.

```
KVS_MACOSX_DEPLOYMENT_TARGET=14.0 make
KVS_MACOSX_DEPLOYMENT_TARGET=14.0 make install
```

If `KVS_MACOSX_DEPLOYMENT_TARGET` is not specified, KVS does not add `-mmacosx-version-min`, allowing the compiler, SDK, and package manager environment to choose the default deployment target.

# Build with CMake

KVS also supports CMake builds. The CMake build reads `kvs.conf` and `Makefile.def` so that it can use settings compatible with the make-based build.

```
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=$KVS_DIR
cmake --build build -j
cmake --install build
```

You can also specify `KVS_DIR` directly.

```
cmake -S . -B build -DKVS_DIR=$HOME/local/kvs
cmake --build build -j
cmake --install build
```

## CMake build options

CMake options can be specified with `-D`.

```
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_CPP_STANDARD=17 \
  -DKVS_SUPPORT_QT=ON \
  -DKVS_SUPPORT_OPENCV=ON
```

On macOS:

```
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_MACOSX_DEPLOYMENT_TARGET=14.0
```

# Main build variables

| Variable | make | CMake | Description |
|:--|:--:|:--:|:--|
| `KVS_DIR` | yes | yes | KVS installation prefix |
| `KVS_CPP_STANDARD` | yes | yes | C++ standard, default is `17` |
| `KVS_MACOSX_DEPLOYMENT_TARGET` | yes | yes | macOS deployment target |
| `KVS_CPP` | yes | yes | C++ compiler |
| `KVS_CC` | yes | partial | C compiler used by make |
| `KVS_FC` | yes | partial | Fortran compiler used by make |
| `KVS_LD` | yes | yes | Linker command |
| `KVS_PY` | yes | yes | Python executable |
| `KVS_*_DIR` | yes | yes | Root directory of an optional dependency |
| `KVS_*_INCLUDE_PATH` | yes | yes | Include path of an optional dependency |
| `KVS_*_LIBRARY_PATH` | yes | yes | Library path of an optional dependency |
| `KVS_*_LINK_LIBRARY` | yes | yes | Explicit link libraries |

Examples:

```
KVS_GLFW_DIR=/opt/homebrew/opt/glfw make
```

```
cmake -S . -B build -DKVS_GLFW_DIR=/opt/homebrew/opt/glfw
```

# Support packages

KVS supports optional packages through `kvs.conf`.

| Package | Flag | Description |
|:--|:--|:--|
| OpenGL | `KVS_ENABLE_OPENGL` | Enable OpenGL-based rendering functionality |
| GLU | `KVS_ENABLE_GLU` | Enable GLU support |
| GLEW | `KVS_ENABLE_GLEW` | Enable GLEW support |
| OpenMP | `KVS_ENABLE_OPENMP` | Enable OpenMP parallelization |
| Deprecated APIs | `KVS_ENABLE_DEPRECATED` | Enable deprecated functions and classes |
| CUDA | `KVS_SUPPORT_CUDA` | Enable CUDA support |
| GLUT | `KVS_SUPPORT_GLUT` | Build GLUT-based screen and widgets. See [SupportGLUT README](Source/SupportGLUT/README.md). |
| GLFW | `KVS_SUPPORT_GLFW` | Build GLFW-based screen and widgets. See [SupportGLFW README](Source/SupportGLFW/README.md). |
| FFmpeg | `KVS_SUPPORT_FFMPEG` | Build movie input/output support. See [SupportFFmpeg README](Source/SupportFFmpeg/README.md). |
| OpenCV | `KVS_SUPPORT_OPENCV` | Build OpenCV-based image/video support. See [SupportOpenCV README](Source/SupportOpenCV/README.md). |
| Qt | `KVS_SUPPORT_QT` | Build Qt-based screen and widgets. See [SupportQt README](Source/SupportQt/README.md). |
| Python | `KVS_SUPPORT_PYTHON` | Build Python interaction support. See [SupportPython README](Source/SupportPython/README.md). |
| MPI | `KVS_SUPPORT_MPI` | Build MPI support. See [SupportMPI README](Source/SupportMPI/README.md). |
| EGL | `KVS_SUPPORT_EGL` | Build GPU off-screen rendering support. See [SupportEGL README](Source/SupportEGL/README.md). |
| OSMesa | `KVS_SUPPORT_OSMESA` | Build CPU off-screen rendering support. See [SupportOSMesa README](Source/SupportOSMesa/README.md). |

For package-specific settings, see the corresponding README files under `Source/Support*`.

# After installation

The following files and directories are installed under `KVS_DIR`.

| Path | Description |
|:--|:--|
| `bin/` | KVS tools such as `kvsmake`, `kvscheck`, `kvsconv`, and `kvsview` |
| `include/` | Header files |
| `lib/` | Static libraries |
| `lib/cmake/` | CMake package configuration files |
| `kvs.conf` | Installed KVS configuration |
| `Makefile.def` | Installed make configuration |

After installation, `kvsmake` can be used to create example or application build files.

```
kvsmake -G
make
```

For Qt applications:

```
kvsmake -Q
qmake
make
```

# Copyright

KVS is released under the BSD 3-Clause License. See [LICENSE](LICENSE) for details.

KVS/Source/Core/NanoVG uses [nanovg](https://github.com/memononen/nanovg) and [stb](https://github.com/nothings/stb).

KVS/Source/Core/FileFormat/XML uses [tinyxml](http://www.grinninglizard.com/tinyxml/).

KVS/Source/SupportMPI/Renderer/ImageCompositor uses [234Compositor](https://github.com/avr-aics-riken/234Compositor).

KVS/Source/SupportFFmpeg/AvCpp uses [AvCpp](https://github.com/h4tr3d/avcpp).

# Citation

When using KVS for your research and publishing the results in papers or other works, please cite this webpage or the following paper as a reference:

Naohisa Sakamoto and Koji Koyamada, [KVS: A simple and effective framework for scientific visualization](https://www.jstage.jst.go.jp/article/jasse/2/1/2_76/_article/-char/en), Journal of Advanced Simulation in Science and Engineering (JASSE), Vol. 2, Issue 1, pp. 76-95, 2015.

```
@article{Naohisa Sakamoto2015,
  title={KVS: A simple and effective framework for scientific visualization},
  author={Naohisa Sakamoto and Koji Koyamada},
  journal={Journal of Advanced Simulation in Science and Engineering},
  volume={2},
  number={1},
  pages={76-95},
  year={2015},
  doi={10.15748/jasse.2.76}
}
```
