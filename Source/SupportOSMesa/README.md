# SupportOSMesa
SupportOSMesa is a support class library with [OSMesa](https://docs.mesa3d.org/osmesa.html) (Off-screen Mesa) for KVS. By checking the flag `KVS_SUPPORT_OSMESA` in the kvs.conf, OSMesa supported classes are compiled and availabl.

## Prerequisite
The OSMesa is required to compile the SupportOSMesa. Although the OSMesa can be easily installed with the package manager, such as apt-get on Ubuntu, we recommend the compile and install it by downloading the source codes of the OSMesa on your developping environment in order to use high-perfrmance rendering functionalities. 

### Mac
An environment in which to compile OSMesa using Homebrew can be set up on macOS.

1. Install the required packages using homebrew.
```sh
$ brew install meson ninja pkg-config llvm@20 python@3.14
```
* To safely compile OSMesa 25.0.7, llvm@20 and python@3.14 need to be install.

2. Install the Python modules (mako, packaging, pyyaml) required for building Mesa into the build venv (~/.venvs/mesa-build).
```sh
$ PY="$(brew --prefix python@3.14)/bin/python3.14"
$ $PY -m venv ~/.venvs/mesa-build
$source ~/.venvs/mesa-build/bin/activate
$ python -m pip install -U pip
$ python -m pip install mako packaging pyyaml
```

3. Move to your working directory (e.g. ~/Work/Temp), in which the OSMesa will be downloaded.
```sh
$ cd ~/Work/Temp
```

4. Download the OSMesa version 25.0.7.
```sh
$ curl -LO https://archive.mesa3d.org/mesa-25.0.7.tar.xz
$ tar xf mesa-25.0.7.tar.xz
$ cd mesa-25.0.7
```

5. Set the environment variables required for compilation. Specify the installation directory in the OSMESA_PREFIX environment variable.
```sh
$ export OSMESA_PREFIX="$HOME/local/osmesa"
$ export LLVM20_PREFIX="$(brew --prefix llvm@20)"
$ export PATH="$LLVM20_PREFIX/bin:$(brew --prefix bison)/bin:$PATH"
$ export PKG_CONFIG_PATH="$LLVM20_PREFIX/lib/pkgconfig:${PKG_CONFIG_PATH:-}"
$ export LLVM_CONFIG="$LLVM20_PREFIX/bin/llvm-config"
```

6. Compile and install OSMesa.
```sh
$ meson setup build/ \
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
$ ninja -C build/ install
```

6. Set environment variables in your shell setting file (e.g. ~/.bashrc) for using the SupportOSMesa.
```
export PKG_CONFIG_PATH=$KVS_OSMESA_DIR/lib/pkgconfig/:$PKG_CONFIG_PATH
export KVS_OSMESA_DIR=${HOME}/local/osmesa
export KVS_OSMESA_LINK_LIBRARY="$(pkg-config --cflags --libs osmesa)"
```

### Linux/Mac with osmesa-install.sh
The OSMesa can also be easily compiled and installed by using [osmesa-install](https://github.com/devernay/osmesa-install.git) as follows.

1. Create install target directories for OSMesa and LLVM. In the following example, the target directries; osmesa and llvm are created under "~/local".

```sh
$ cd
$ mkdir local
$ cd local
$ mkdir osmesa
$ mkdir llvm
```

2. Move to your working directory (e.g. ~/Work/GitHub), in which the osmesa-install will be cloned.

```sh
$ cd ~/Work/GitHub
```

3. Download the osmesa install script.

```sh
$ git clone https://github.com/devernay/osmesa-install.git
$ cd osmesa-install
```

4. Modify the install-script 'osmesa-install.sh' as follows. Note that these options can also be specified directly via environmental variables at compile time.

```
a) Specify the install target directory of OSMesa.
osmesaprefix="${OSMESA_PREFIX:-${HOME}/local/osmesa}

b) Specify the driver of 4. (1:swrast, 2:softpipe, 3:llvmpipe, 4:swr)
osmesadriver=${OSMESA_DRIVER:-4}

c) Disable 'mangled'. (1:compile mangled OSMesa or 0:not)
mangled=0

d) Specify the install target directory for LLVM.
llvmprefix="${LLVM_PREFIX:-${HOME}/local/llvm}

e) Enable 'buildllvm'. (1:download and build LLVM or 0:not)
buildllvm="${LLVM_BUILD:-1}
```

Rendering driver
- swrast: OSMesa original resterizer. very slow and not supported GLSL shader. Not recommended.
- softpipe: Gallium driver. Supported GLSL shader but very slow. Not recommended.
- llvmpipe: LLVM optimized Gallium driver. Very fast but LLVM is required. Recommended.
- swr: Intel CPU optimized Gallium driver (known as OpenSWR). Very fast. Recommended if you use Intel CPUs.

For Mac, several packages need to be installed with homebrew before cmpiling OSMesa as follows:
```sh
$ brew install gsed pkg-config cmake libtool automake autoconf
```

5. Compile and install OSMesa.

```sh
$ mkdir build
$ cd build
$ ../osmesa-install.sh
```

To specify the options directly via environmental variables, compile and install as follows:
```
$ OSMESA_PREFIX=${HOME}/local/osmesa OSMESA_DRIVER=3 MANGLED=0 LLVM_PREFIX=${HOME}/local/llvm LLVM_BUILD=1 ../osmesa-install.sh
```

6. Set environment variables for using the SupportOSMesa.

```
export KVS_OSMESA_DIR=${HOME}/local/osmesa
export KVS_OSMESA_LINK_LIBRARY="-lOSMesa32 -lz `~/local/llvm/bin/llvm-config --libs` `~/local/llvm/bin/llvm-config --ldflags` -lrt -ldl -lpthread -lm"
```

## Example Program
An example program for the SupportOSMesa is available in the Example/SupportOSMesa directory. You can easily compiple the example program with the kvsmake as follows:

```
$ cd Example/SupportOSMesa
$ cd Hello
$ kvsmake -G
$ kvsmake
```

The offscreen rendering results (12 images) will be generated by executing the compiled program as follows:

```
e.g)
$ ./Hello
OSMesa version: 11.2.0
rendering to ... output_000.bmp
rendering to ... output_001.bmp
rendering to ... output_002.bmp
rendering to ... output_003.bmp
rendering to ... output_004.bmp
rendering to ... output_005.bmp
rendering to ... output_006.bmp
rendering to ... output_007.bmp
rendering to ... output_008.bmp
rendering to ... output_009.bmp
rendering to ... output_010.bmp
rendering to ... output_011.bmp
Total rendering time:   5.92246 [sec]
Average rendering time: 0.493539 [sec]
```
