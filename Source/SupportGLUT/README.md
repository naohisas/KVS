# SupportGLUT

SupportGLUT provides GLUT-based screen and widget classes for KVS viewer applications.

`KVS_SUPPORT_GLUT` is enabled by default in `kvs.conf`.

## Enable this support

Edit `kvs.conf`:

```make
KVS_ENABLE_OPENGL = 1
KVS_SUPPORT_GLUT = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_GLUT=ON
```

## Requirements

SupportGLUT requires GLUT or FreeGLUT headers and libraries.

## Install dependencies

### Linux

```sh
sudo apt-get install freeglut3-dev
```

Depending on the distribution, the package name may be `libglut-dev` or similar.

### macOS

GLUT is provided by the macOS SDK. Install Xcode or the Xcode Command Line Tools:

```sh
xcode-select --install
```

### Windows

Legacy GLUT binaries for Win32 have historically been used with KVS:

http://www.xmission.com/~nate/glut.html

For modern Windows environments, use headers and libraries compatible with your Visual C++ compiler and make sure the DLL is available in `PATH` or next to the executable.

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
make
make install
```

If GLUT is installed in a non-standard location:

```sh
export KVS_GLUT_DIR=/path/to/glut
```

Manual path settings:

```sh
export KVS_GLUT_INCLUDE_PATH=/path/to/glut/include
export KVS_GLUT_LIBRARY_PATH=/path/to/glut/lib
export KVS_GLUT_LINK_LIBRARY="-lglut"
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_GLUT=ON
cmake --build build -j
cmake --install build
```

For a non-standard GLUT installation:

```sh
cmake -S . -B build \
  -DKVS_SUPPORT_GLUT=ON \
  -DKVS_GLUT_DIR=/path/to/glut
```

## Build examples

After installing KVS:

```sh
cd Example/SupportGLUT
kvsmake -G
kvsmake
```

Use the actual example subdirectory if the example is organized into subfolders.
