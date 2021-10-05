# SupportQt
SupportQt is a support class library with the Qt for KVS. By checking the flag KVS_SUPPORT_QT in the kvs.conf, Qt supported classes are compiled and available. The SupportQt is required for developping GUI application based on KVS.
```Makefile
KVS_SUPPORT_QT = 1
```

## Prerequisite
To compile the SupportQt, the Qt needs to be installed. NOTE: Currently, Qt6 is not supported. Therefore, Qt5 is required to compile the SupportQt.

### Linux
Not tested.

### Mac
In case of Mac, the Qt can be installed by using Homebrew on a terminal as follows.
```
$ brew install qt@5
```
In order to use qmake command, you may need to set:
```
export PATH="/opt/homebrew/opt/qt@5/bin:$PATH"
```
And mac SDK version in the mkspecs (/opt/homebrew/opt/qt@5/mkspecs/common/macx.conf)
```
$ vi /opt/homebrew/opt/qt@5/mkspecs/common/macx.conf

Modify 'QMAKE_MACOSX_DEPLOYMENT_TARGET' and 'QT_MAC_SDK_VERSION_MAX'
e.g.)
QMAKE_MACOSX_DEPLOYMENT_TARGET = 11.3
QT_MAC_SDK_VERSION_MAX = 11.3
```

### Windows
Not tested.
