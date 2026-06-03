# SupportPython

SupportPython provides Python C API integration for KVS.

## Enable this support

Edit `kvs.conf`:

```make
KVS_SUPPORT_PYTHON = 1
```

Or enable it when configuring with CMake:

```sh
cmake -S . -B build -DKVS_SUPPORT_PYTHON=ON
```

## Requirements

SupportPython requires:

- Python 3 development headers and libraries
- NumPy

Python 3 is recommended. Python 2 is no longer recommended for new environments.

## Install dependencies

### Linux

```sh
sudo apt-get install python3-dev python3-numpy
```

### macOS

```sh
brew install python
python3 -m pip install numpy
```

If your Python installation is externally managed, prefer using a virtual environment:

```sh
python3 -m venv ~/.venvs/kvs-python
source ~/.venvs/kvs-python/bin/activate
python -m pip install -U pip numpy
```

### Windows

Not tested.

## Python variables

The make-based build calls `python` and `python-config` by default. Use `KVS_PY` to select a Python executable:

```sh
export KVS_PY=python3
```

For versioned Python:

```sh
export KVS_PY=python3.12
```

The build then calls `python3.12` and `python3.12-config`.

Additional path variables are available:

| Variable | Description |
|:--|:--|
| `KVS_PY` | Python executable used by make |
| `KVS_PYTHON_DIR` | Python installation prefix |
| `KVS_NUMPY_DIR` | NumPy installation prefix |

## Build KVS with make

```sh
export KVS_DIR=$HOME/local/kvs
export KVS_PY=python3
make
make install
```

If you use a virtual environment, activate it before building:

```sh
source ~/.venvs/kvs-python/bin/activate
export KVS_PY=python
make
make install
```

## Build KVS with CMake

```sh
cmake -S . -B build \
  -DKVS_DIR=$HOME/local/kvs \
  -DKVS_SUPPORT_PYTHON=ON \
  -DPython3_EXECUTABLE=$(which python3)
cmake --build build -j
cmake --install build
```

You can also use:

```sh
KVS_PY=python3 cmake -S . -B build -DKVS_SUPPORT_PYTHON=ON
```

## Build examples

After installing KVS:

```sh
cd Example/SupportPython
kvsmake -G
kvsmake
```

Use the actual example subdirectory if the example is organized into subfolders.

## Troubleshooting

### NumPy is not found

Check that NumPy is installed for the same Python executable used by KVS:

```sh
$KVS_PY -c "import numpy; print(numpy.get_include())"
```

### `python-config` is not found

Use a Python installation that provides the matching `python-config` command, or set `KVS_PY` to a versioned executable such as `python3.12`.
