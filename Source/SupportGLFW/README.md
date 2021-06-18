# SupportGLFW
SupportGLFW is a support class library with the Graphics Library Framework (GLFW) for KVS. By checking the flag `KVS_SUPPORT_GLFW` in the kvs.cong, GLFW supported classes are compiled and available. The SupportGLFW is required for developping viewer application based on KVS.

```Makefile
KVS_SUPPORT_GLFW = 1
```

## Prerequisite
To compile the SupportGLFW, the GLFW needs to be installed.

### Linux
On Linux, the GLFW can be installed from a terminal as follows.
```
$ sudo apt-get install libglfw3-dev
```

### Mac
In the case of Mac, the GLFW can be installed by using Homebrew on a terminal as follows.
```
$ brew install glfw
```

### Windows
For Windows, The GLFW binary can be downloaded from the following site.

[GLFW - An OpenGL Library](https://www.glfw.org/)

Click on "64-bit Windows binraries" or "32-bit Windows binaries" in the [download page](https://www.glfw.org/download.html) to download the zip file. After extracting the files, copy these files into the following folders.

|Files|Folders|
|:--|:--|
|glfw3.dll|Same folder as GLUT DLL folder, in which glut32.dll is included.|
|glfw3.lib, glfw3dll.lib|Same folder as GLUT static library folder, in which glut32.lib is included.|
|glfw3.h, glfw3native.h| Same folder as GLUT header folder, in which glut.h is included.|

- The library files (\*.dll, \*.lib) and header files (\*.h) are included in lib-xxxx and include/GLFW folders, respectively.
- "xxxx" of lib-xxxx means compiler name (version of MSVC) used for building the SupportGLFW.
- For GLUT folders, see [SupportGLUT/README](../SupportGLUT/README.md).
