# SupportEGL

SupportEGL provides EGL-based off-screen rendering classes for KVS.

Enable this support when you want to render with OpenGL on a GPU without creating an X11/desktop window. This is typically useful on Linux servers and remote GPU nodes.

## Enable this support

Edit `kvs.conf`:

```make
KVS_ENABLE_OPENGL = 1
KVS_SUPPORT_EGL  = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_EGL=ON
```

## Requirements

SupportEGL requires:

- an OpenGL-capable GPU and driver
- EGL headers and libraries
- a working EGL vendor setup, such as GLVND on Linux

The current instructions have mainly been tested on Ubuntu with NVIDIA GPUs.

## Install dependencies

### Linux

Install a GPU driver suitable for your system. On Ubuntu with NVIDIA GPUs:

```sh
sudo ubuntu-drivers autoinstall
```

Install EGL development files:

```sh
sudo apt-get install libegl1-mesa-dev
```

If you use the NVIDIA EGL implementation through GLVND, check that the vendor file exists:

```sh
cat /etc/glvnd/egl_vendor.d/10_nvidia.json
```

The file usually contains an ICD entry similar to:

```json
{
    "file_format_version": "1.0.0",
    "ICD": {
        "library_path": "libEGL_nvidia.so.0"
    }
}
```

### macOS

EGL is not a common rendering backend on macOS. Use SupportGLUT, SupportGLFW, SupportQt, or SupportOSMesa instead.

### Windows

Not tested.

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
make
make install
```

If EGL is installed in a non-standard location, specify it explicitly:

```sh
export KVS_EGL_DIR=$HOME/local/egl
```

Alternatively:

```sh
export KVS_EGL_INCLUDE_PATH=$HOME/local/egl/include
export KVS_EGL_LIBRARY_PATH=$HOME/local/egl/lib
export KVS_EGL_LINK_LIBRARY="-lEGL"
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_EGL=ON
cmake --build build -j
cmake --install build
```

For a non-standard EGL installation:

```sh
cmake -S . -B build \
  -DKVS_SUPPORT_EGL=ON \
  -DKVS_EGL_DIR=$HOME/local/egl
```

## Build examples

After installing KVS:

```sh
cd Example/SupportEGL/Hello
kvsmake -G
kvsmake
./Hello
```

The example writes off-screen rendering results such as `output_000.bmp`.

## Troubleshooting

### Running without X Window System

If an X display is not available, unset `DISPLAY` before running EGL examples:

```sh
unset DISPLAY
```

### EGL library not found

Set `KVS_EGL_DIR`, or specify `KVS_EGL_INCLUDE_PATH`, `KVS_EGL_LIBRARY_PATH`, and `KVS_EGL_LINK_LIBRARY` manually.
