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

include( SupportQt.pri )
