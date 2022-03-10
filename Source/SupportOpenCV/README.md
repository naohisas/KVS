# SupportOpenCV
SupportOpenCV is a support class library with the OpenCV (Open Source Computer Vision Library) for KVS. By checking the flag KVS_SUPPORT_OPENCV in the kvs.conf, OpenCV supported classes are compiled and available. The SupportOpenCV is required for developping image processing and computer vision application based on KVS.
```Makefile
KVS_SUPPORT_OPENCV = 1
```

## Prerequisite
To compile the SupportGLUT, the GLUT needs to be installed.

### Linux
The OpenCV can be installed from a terminal as follows.
```
$ sudo apt-get install libopencv-dev
```

### Mac
In case of Mac, the OpenCV can be installed by using Homebrew on a terminal as follows.
```
$ brew install oepncv
```
For pkg-config to find opencv you may need to set:
```
export PKG_CONFIG_PATH="/usr/local/opt/opencv/lib/pkgconfig"     (intel mac)
export PKG_CONFIG_PATH="/opt/homebrew/opt/opencv/lib/pkgconfig"  (m1 mac)
```

### Windows
Not tested.
