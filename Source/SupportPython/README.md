# SupportPython
SupportPython is a support class library with the Python C API for KVS. By checking the flag KVS_SUPPORT_PYTHON in the kvs.conf, Python supported classes are compiled and available.
```Makefile
KVS_SUPPORT_PYTHON = 1
```

## Prerequisite
To compile the SupportPython, Python version 2 or 3 and Numpy need to be installed.

# Linux
Algthouhg Python2 and Python3 have been pre-installed on recent Linux environments, Numpy needs to be installed additionally as follows:
```
# Python2
$ sudo apt-get install python-numpy
# Python3
$ sudo apt-get install python3-numpy
```

# Mac
On Mac, Python2 and Numpy have been pre-installed. If you want to use Python3, you need to install them as follows:
```
$ brew install python3
$ pip3 install numpy
```

# Windows
