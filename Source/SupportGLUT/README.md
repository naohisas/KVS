# SupportGLUT
SupportGLUT is a support class library with The OpenGL Utility Toolkit (GLUT) for KVS. By checking the flag KVS_SUPPORT_GLUT in the kvs.cong, GLUT supported classes are compiled and available. The SupportGLUT is required for developping viewer application based on KVS.
```Makefile
KVS_SUPPORT_GLUT = 1
```

## Prerequisite
To compile the SupportGLUT, the GLUT needs to be installed.

### Linux
If you want to install GLUT from a terminal, you can install it as follows.
```
$ sudo apt-get install freeglut3-dev libglut3-dev
```
- The above example shows an example of installing freeglut version 3.

### Mac
On Mac OS X, the GLUT is pre-installed and does not need to be installed.

### Windows
For Windows, The GLUT binary can be downloaded from the following site.

[Nate Robins - OpenGL - GLUT for Win32](http://www.xmission.com/~nate/glut.html)

Click on "[glut-3.7.6-bin.zip (117 KB)](https://user.xmission.com/~nate/glut/glut-3.7.6-bin.zip)" to download the zip file. After extracting the file, copy the extracted files into the following folders.

|Files|Folders|
|:--|:--|
|glut32.dll|<ul><li>**Windows 32bit**<br>C:\Windows\System32</li><li>**Windows 64bit**<br>C:\Windows\SysWOW64</li></ul>|
|glut32.lib|<ul><li>**Visual Studio 2010 (Windows XP 32bit)**<br>C:\Program Files\Microsoft SDKs\Windows\v7.0A\Lib</li><li>**Visual Studio 2010 (Windows 7 64bit)**<br>C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Lib</li><li>**Visual Studio 2012 (Windows 7/8 64bit)**<br>C:\Program Files (x86)\Windows Kits\8.0\Lib\win8\um\x86</li><li>**Visual Studio 2013 (Windows 7/8/8.1 64bit)**<br>C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86</li><li>**Visual Studio 2015 (Windows 7/8/8.1 64bit)**<br>C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x86</li><li>**Visual Studio 2017/2019 (Windows 10 64bit)**<br>C:\Program Files (x86)\Windows Kits\10\Lib\10.x.xxxxx.x\um\x86</li></ul>|
|glut.h|<ul><li>**Visual Studio 2010 (Windows XP 32bit)**<br>C:\Program Files\Microsoft SDKs\Windows\v7.0A\Include\gl</li><li>**Visual Studio 2010 (Windows 7 64bit)**<br>C:\Program Files (x86)\Microsoft SDKs\Windows\v7.0A\Include\gl</li><li>**Visual Studio 2012 (Windows 7/8 64bit)**<br>C:\Program Files (x86)\Windows Kits\8.0\Include\um\gl</li><li>**Visual Studio 2013 (Windows 7/8/8.1 64bit)**<br>C:\Program Files (x86)\Windows Kits\8.1\Include\um\gl</li><li>**Visual Studio 2015 (Windows 7/8/8.1 64bit)**<br>C:\Program Files (x86)\Windows Kits\8.1\Include\um\gl</li><li>**Visual Studio 2017/2019 (Windows 10 64bit)**<br>C:\Program Files (x86)\Windows Kits\10\Include\10.x.xxxxx.x\um\gl</li></ul>|
- The location of the files depends on the version of Visual Studio. For Windows environments not listed above, copy the files to the appropriate folder based on the destination of previous versions.
