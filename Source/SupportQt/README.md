# SupportQt
SupportQt is a support class library with the Qt for KVS. By checking the flag KVS_SUPPORT_QT in the kvs.conf, Qt supported classes are compiled and available. The SupportQt is required for developping GUI application based on KVS.
```Makefile
KVS_SUPPORT_QT = 1
```

## Prerequisite
To compile the SupportQt, the Qt needs to be installed.

### Linux
Not tested.

### Mac
In case of Mac, the Qt can be installed by using Homebrew on a terminal as follows.
```
$ brew install qt
```
In order to use qmake command, you may need to set:
```
export PATH="/usr/local/opt/qt/bin:$PATH"     (intel mac)
export PATH="/opt/homebrew/opt/qt/bin:$PATH"  (m1 mac)
```
And check mac SDK version in the mkspecs
```
<Qt5>
$ vi /usr/local/opt/qt/mkspecs/common/macx.conf     (intel mac)
$ vi /opt/homebrew/opt/qt/mkspecs/common/macx.conf  (m1 mac)

<Qt6>
$ vi /usr/local/opt/qt/share/qt/mkspecs/qconfig.pri     (intel mac)
$ vi /opt/homebrew/opt/qt/share/qt/mkspecs/qconfig.pri  (m1 mac)

Modify 'QMAKE_MACOSX_DEPLOYMENT_TARGET' and 'QT_MAC_SDK_VERSION_MAX'
e.g.)
QMAKE_MACOSX_DEPLOYMENT_TARGET = 15.0
QT_MAC_SDK_VERSION_MAX = 15.0
```

If you get the fatal error "'iostream' file not found" after upgrading MacOSX, try reinstalling commandline tools as follows:
```
$ sudo rm -rf /Library/Developer/CommandLineTools
$ xcode-select --install
```

### Windows
Not tested.
