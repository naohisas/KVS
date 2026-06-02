#*****************************************************************************
#  $Id: Release.pro 62 2009-03-19 06:58:44Z naohisa.sakamoto $
#*****************************************************************************

CONFIG  += release
DEFINES += NDEBUG

MOC_DIR     = Release
OBJECTS_DIR = Release
DESTDIR     = Release

KVS_CPP = $$(KVS_CPP)
!isEmpty( KVS_CPP ) {
QMAKE_CXX = $(KVS_CPP)
}

isEmpty( KVS_CPP_STANDARD ) {
KVS_CPP_STANDARD = 17
}

CONFIG += c++$${KVS_CPP_STANDARD}

KVS_MACOSX_DEPLOYMENT_TARGET = $$(KVS_MACOSX_DEPLOYMENT_TARGET)

macx {
    isEmpty( KVS_MACOSX_DEPLOYMENT_TARGET ) {
        QMAKE_MACOSX_DEPLOYMENT_TARGET =
    } else {
        QMAKE_MACOSX_DEPLOYMENT_TARGET = $$KVS_MACOSX_DEPLOYMENT_TARGET
    }
}

include( SupportQt.pri )
