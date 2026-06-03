# SupportOSMesa

SupportOSMesa provides OSMesa-based CPU off-screen rendering classes for KVS.

Use SupportOSMesa when you need OpenGL rendering without a window system or GPU-backed EGL context.

## Enable this support

Edit `kvs.conf`:

```make
KVS_ENABLE_OPENGL  = 1
KVS_SUPPORT_OSMESA = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_OSMESA=ON
```

## Requirements

SupportOSMesa requires OSMesa headers and libraries.

For simple builds, an OS package such as `libosmesa6-dev` may be enough. For faster rendering and better shader support, build Mesa with the `llvmpipe` Gallium driver.

## Quick install

### Linux

```sh
sudo apt-get install libosmesa6-dev
```

### macOS

Homebrew does not always provide an OSMesa configuration suitable for KVS off-screen rendering. Building Mesa from source with `llvmpipe` is recommended.

### Windows

Not tested.

## Build Mesa/OSMesa from source on macOS

The following example builds Mesa 25.0.7 with OSMesa and `llvmpipe`.

Install build tools:

```sh
brew install meson ninja pkg-config llvm@20 python@3.14 bison
```

Create a Python build environment:

```sh
PY="$(brew --prefix python@3.14)/bin/python3.14"
$PY -m venv ~/.venvs/mesa-build
source ~/.venvs/mesa-build/bin/activate
python -m pip install -U pip
python -m pip install mako packaging pyyaml
```

Download Mesa:

```sh
cd ~/Work/Temp
curl -LO https://archive.mesa3d.org/mesa-25.0.7.tar.xz
tar xf mesa-25.0.7.tar.xz
cd mesa-25.0.7
```

Set build variables:

```sh
export OSMESA_PREFIX="$HOME/local/osmesa"
export LLVM20_PREFIX="$(brew --prefix llvm@20)"
export PATH="$LLVM20_PREFIX/bin:$(brew --prefix bison)/bin:$PATH"
export PKG_CONFIG_PATH="$LLVM20_PREFIX/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
export LLVM_CONFIG="$LLVM20_PREFIX/bin/llvm-config"
```

Configure and install:

```sh
meson setup build/ \
  --prefix=$OSMESA_PREFIX \
  --libdir=lib \
  -Dbuildtype=release \
  -Db_ndebug=true \
  -Dllvm=enabled \
  -Dshared-llvm=enabled \
  -Ddraw-use-llvm=true \
  -Dopengl=true \
  -Dosmesa=true \
  -Dgallium-drivers=llvmpipe \
  -Dshared-glapi=enabled \
  -Dplatforms= \
  -Dglx=disabled \
  -Degl=disabled \
  -Dgbm=disabled \
  -Dgles1=disabled \
  -Dgles2=disabled \
  -Dglvnd=false \
  -Dvulkan-drivers= \
  -Dtools= \
  -Dgallium-opencl=disabled \
  -Dgallium-rusticl=false
ninja -C build/ install
```

Set variables for KVS:

```sh
export KVS_OSMESA_DIR=$HOME/local/osmesa
export PKG_CONFIG_PATH=$KVS_OSMESA_DIR/lib/pkgconfig:$PKG_CONFIG_PATH
export KVS_OSMESA_LINK_LIBRARY="$(pkg-config --cflags --libs osmesa)"
```

## Alternative source build with osmesa-install.sh

The external `osmesa-install.sh` script can also be used:

https://github.com/devernay/osmesa-install

Example:

```sh
git clone https://github.com/devernay/osmesa-install.git
cd osmesa-install
OSMESA_PREFIX=$HOME/local/osmesa \
OSMESA_DRIVER=3 \
MANGLED=0 \
LLVM_PREFIX=$HOME/local/llvm \
LLVM_BUILD=1 \
./osmesa-install.sh
```

Driver notes:

| Driver | Notes |
|:--|:--|
| `swrast` | Very slow; GLSL support is limited |
| `softpipe` | GLSL capable but slow |
| `llvmpipe` | Recommended; LLVM-based software rasterizer |
| `swr` | Fast on suitable Intel CPUs |

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export KVS_OSMESA_DIR=$HOME/local/osmesa
export PKG_CONFIG_PATH=$KVS_OSMESA_DIR/lib/pkgconfig:$PKG_CONFIG_PATH
export KVS_OSMESA_LINK_LIBRARY="$(pkg-config --cflags --libs osmesa)"
make
make install
```

If `pkg-config` is not available, specify the link libraries manually:

```sh
export KVS_OSMESA_LINK_LIBRARY="-lOSMesa"
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_OSMESA=ON \
  -DKVS_OSMESA_DIR=$HOME/local/osmesa
cmake --build build -j
cmake --install build
```

If needed:

```sh
cmake -S . -B build \
  -DKVS_SUPPORT_OSMESA=ON \
  -DKVS_OSMESA_LINK_LIBRARY="$(pkg-config --cflags --libs osmesa)"
```

## Build examples

After installing KVS:

```sh
cd Example/SupportOSMesa/Hello
kvsmake -G
kvsmake
./Hello
```

The example writes off-screen rendering results such as `output_000.bmp`.

## Troubleshooting

### OSMesa headers or libraries are not found

Set `KVS_OSMESA_DIR`. If the installation layout is not `include` and `lib` under one prefix, set `KVS_OSMESA_INCLUDE_PATH`, `KVS_OSMESA_LIBRARY_PATH`, and `KVS_OSMESA_LINK_LIBRARY`.

### Runtime library not found

Add the OSMesa library directory to the runtime library path for your platform, such as `DYLD_LIBRARY_PATH` on macOS or `LD_LIBRARY_PATH` on Linux.
