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
export KVS_FFMPEG_DIR=/usr/local/Cellar/ffmpeg/4.4.1_2      (intel mac)
export KVS_FFMPEG_DIR=/opt/homebrew/Cellar/ffmpeg/4.4.1_2   (m1 mac)
```

### Windows
Not tested.
