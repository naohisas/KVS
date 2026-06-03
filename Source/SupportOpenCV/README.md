# SupportOpenCV

SupportOpenCV provides OpenCV-based image and video support for KVS.

## Enable this support

Edit `kvs.conf`:

```make
KVS_SUPPORT_OPENCV = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_OPENCV=ON
```

## Requirements

SupportOpenCV requires OpenCV headers and libraries. OpenCV 4 is recommended.

## Install dependencies

### Linux

```sh
sudo apt-get install libopencv-dev pkg-config
```

### macOS

```sh
brew install opencv
```

For Intel Mac:

```sh
export KVS_OPENCV_DIR=/usr/local/opt/opencv
export PKG_CONFIG_PATH=$KVS_OPENCV_DIR/lib/pkgconfig:$PKG_CONFIG_PATH
```

For Apple Silicon Mac:

```sh
export KVS_OPENCV_DIR=/opt/homebrew/opt/opencv
export PKG_CONFIG_PATH=$KVS_OPENCV_DIR/lib/pkgconfig:$PKG_CONFIG_PATH
```

### Windows

Not tested.

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export KVS_OPENCV_DIR=/opt/homebrew/opt/opencv
export PKG_CONFIG_PATH=$KVS_OPENCV_DIR/lib/pkgconfig:$PKG_CONFIG_PATH
make
make install
```

If OpenCV is installed in a non-standard location:

```sh
export KVS_OPENCV_DIR=/path/to/opencv
```

The make-based build can also use `pkg-config` for `opencv4` or `opencv`.

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_OPENCV=ON \
  -DKVS_OPENCV_DIR=/opt/homebrew/opt/opencv
cmake --build build -j
cmake --install build
```

CMake first tries to use OpenCV's CMake package information and can also use `pkg-config`.

## Build examples

After installing KVS:

```sh
cd Example/SupportOpenCV
kvsmake -G
kvsmake
```

Use the actual example subdirectory if the example is organized into subfolders.

## Troubleshooting

### OpenCV is not found

Set `KVS_OPENCV_DIR` and `PKG_CONFIG_PATH`. Confirm that one of the following commands succeeds:

```sh
pkg-config --exists opencv4
pkg-config --exists opencv
```

### Header or ABI mismatch

Make sure the OpenCV package was built for the same architecture and deployment target as KVS.
