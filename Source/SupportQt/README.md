# SupportQt

SupportQt provides Qt-based screen and widget classes for KVS GUI applications.

## Enable this support

Edit `kvs.conf`:

```make
KVS_ENABLE_OPENGL = 1
KVS_SUPPORT_QT   = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_QT=ON
```

## Requirements

SupportQt requires Qt development tools and libraries. Qt 6 or Qt 5 is recommended.

For qmake-based examples, `qmake` must be available in `PATH`.

## Install dependencies

### Linux

For Qt 6 on Ubuntu:

```sh
sudo apt-get install qt6-base-dev qt6-base-dev-tools
```

For Qt 5:

```sh
sudo apt-get install qtbase5-dev qtbase5-dev-tools
```

### macOS

```sh
brew install qt
```

For Intel Mac:

```sh
export PATH=/usr/local/opt/qt/bin:$PATH
```

For Apple Silicon Mac:

```sh
export PATH=/opt/homebrew/opt/qt/bin:$PATH
```

### Windows

Not tested.

## macOS deployment target

Do not edit Qt's installed mkspec files for normal KVS builds. Use `KVS_MACOSX_DEPLOYMENT_TARGET` only when you need to force a specific deployment target:

```sh
export KVS_MACOSX_DEPLOYMENT_TARGET=14.0
```

If `KVS_MACOSX_DEPLOYMENT_TARGET` is empty, KVS leaves the deployment target unset so that the compiler, SDK, Qt, and package manager environment can choose the default.

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export PATH=/opt/homebrew/opt/qt/bin:$PATH
make
make install
```

If Qt is installed in a non-standard location:

```sh
export KVS_QT_DIR=/path/to/qt
export PATH=$KVS_QT_DIR/bin:$PATH
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_QT=ON
cmake --build build -j
cmake --install build
```

If Qt is not found automatically, add the Qt prefix to `CMAKE_PREFIX_PATH`:

```sh
cmake -S . -B build \
  -DKVS_SUPPORT_QT=ON \
  -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/qt
```

## Build examples

After installing KVS:

```sh
cd Example/SupportQt/MainWindow
kvsmake -Q
qmake
make
```

## Troubleshooting

### `qmake` is not found

Add Qt's `bin` directory to `PATH`.

```sh
export PATH=/opt/homebrew/opt/qt/bin:$PATH
```

### Linker warns about different macOS versions

Use a consistent deployment target for KVS, Qt, and other dependencies. If you need to set one explicitly:

```sh
export KVS_MACOSX_DEPLOYMENT_TARGET=14.0
```

Then rebuild and reinstall KVS before rebuilding Qt examples.

### Fatal error: `iostream` file not found

After upgrading macOS or Xcode, reinstall the Xcode Command Line Tools:

```sh
sudo rm -rf /Library/Developer/CommandLineTools
xcode-select --install
```
