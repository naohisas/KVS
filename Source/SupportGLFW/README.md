# SupportGLFW

SupportGLFW provides GLFW-based screen and widget classes for KVS viewer applications.

## Enable this support

Edit `kvs.conf`:

```make
KVS_ENABLE_OPENGL = 1
KVS_SUPPORT_GLFW = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_GLFW=ON
```

## Requirements

SupportGLFW requires GLFW headers and libraries.

## Install dependencies

### Linux

```sh
sudo apt-get install libglfw3-dev
```

### macOS

```sh
brew install glfw
```

For Intel Mac:

```sh
export KVS_GLFW_DIR=/usr/local/opt/glfw
```

For Apple Silicon Mac:

```sh
export KVS_GLFW_DIR=/opt/homebrew/opt/glfw
```

### Windows

Download GLFW from:

https://www.glfw.org/download.html

Use binaries compatible with your Visual C++ compiler. If you install GLFW outside the default search paths, set `KVS_GLFW_DIR`, or set the include/library/link variables manually.

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export KVS_GLFW_DIR=/opt/homebrew/opt/glfw
make
make install
```

Manual path settings:

```sh
export KVS_GLFW_INCLUDE_PATH=/path/to/glfw/include
export KVS_GLFW_LIBRARY_PATH=/path/to/glfw/lib
export KVS_GLFW_LINK_LIBRARY="-lglfw"
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_GLFW=ON \
  -DKVS_GLFW_DIR=/opt/homebrew/opt/glfw
cmake --build build -j
cmake --install build
```

CMake also searches for `glfw` or `glfw3` packages when available.

## Build examples

After installing KVS:

```sh
cd Example/SupportGLFW
kvsmake -G
kvsmake
```

Use the actual example subdirectory if the example is organized into subfolders.

## Troubleshooting

### GLFW headers or libraries are not found

Set `KVS_GLFW_DIR`. If the installation layout is not `include` and `lib` under one prefix, use `KVS_GLFW_INCLUDE_PATH`, `KVS_GLFW_LIBRARY_PATH`, and `KVS_GLFW_LINK_LIBRARY`.
