#*****************************************************************************
#  $Id: Viewer.pri 1319 2012-10-02 10:32:10Z naohisa.sakamoto@gmail.com $
#*****************************************************************************

#=============================================================================
#  sources and headers
#=============================================================================
VIEWER_HEADERS += \
Viewer/Application.h \
Viewer/Screen.h \
Viewer/ScreenBase.h \
Viewer/Timer.h \
Viewer/QtScreenSupport.h \
Viewer/KVSKey.h \
Viewer/KVSMouseButton.h \


VIEWER_SOURCES += \
Viewer/Application.cpp \
Viewer/Screen.cpp \
Viewer/ScreenBase.cpp \
Viewer/Timer.cpp \


HEADERS += $$VIEWER_HEADERS
SOURCES += $$VIEWER_SOURCES


#=============================================================================
#  install
#=============================================================================
VIEWER_INSTALL_HEADERS.files = $$VIEWER_HEADERS
VIEWER_INSTALL_HEADERS.path  = $$KVS_DIR/include/SupportQt/Viewer

INSTALLS += VIEWER_INSTALL_HEADERS
