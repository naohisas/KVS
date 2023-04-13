# SupportFFmpeg
SupportFFmpeg is a support class library with the FFmpeg for KVS. By checking the flag `KVS_SUPPORT_FFMPEG` in the kvs.cong, FFmpeg supported classes are compiled and available.

## Prerequisite
To compile the SupportFFmpeg, the FFmpeg needs to be installed.


### Linux
On Linux, the FFmpeg can be installed from a terminal as follows.
```
$ sudo apt-get install ffmpeg
```

### Mac
In the case of Mac, the FFmpeg can be installed by using Homebrew on a terminal as follows.
```
$ brew install ffmpeg
```

In order to link the libraries, you may need to set:
```
export KVS_FFMPEG_DIR=/usr/local/opt/ffmpeg      (intel mac)
export KVS_FFMPEG_DIR=/opt/homebrew/opt/ffmpeg   (m1 mac)
```

### Windows
For Windows, the FFMPEG binary can be downloaded from the following site.

[FFMPEG - FFmpeg-Builds-Win32](https://github.com/sudo-nautilus/FFmpeg-Builds-Win32/releases)

 - Donwload the FFMPEG of version 5 (ffmpeg-n5.1-latest-win32-gpl-shared-5.1.zip).
 - NOTE: Due to some limitations, a 32-bit binary is currently required; only the 64-bit binary can be downloaded from the FFMPEG home site, so please download the 32-bit binary from the above site.

Follow the steps below to install the FFMPEG.

1. Create an install target folder (e.g. C:\ffmpeg) and set an environment variable `KVS_FFMPEG_DIR` to the target folder as follows:

|Variable|Value|
|:-------|:----|
|KVS_FFMPEG_DIR |C:\ffmpeg|

2. Copy the folders of include and lib included in the downloaded file to the target folder.

3. Copy all of DLL files (*.dll) in the bin folder to the following folder.

|Files|Folder|
|:-------|:----|
|avcodec-59.dll<br>avdevice-59.dll<br>avfilter-8.dll<br>avformat-59.dll<br>avutil-57.dll<br>postproc-56.dll<br>swresample-4.dll<br>swscale-6.dll|<ul><li>**Windows 32bit**<br>C:\Windows\System32</li><li>**Windows 64bit**<br>C:\Windows\SysWOW64</li></ul>|
