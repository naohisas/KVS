#*****************************************************************************
#  $Id: Debug.pro 62 2009-03-19 06:58:44Z naohisa.sakamoto $
#*****************************************************************************

CONFIG  += debug
DEFINES += _DEBUG KVS_ENABLE_DEBUG

MOC_DIR     = Debug
OBJECTS_DIR = Debug
DESTDIR     = Debug

KVS_CPP = $$(KVS_CPP)
!isEmpty( KVS_CPP ) {
QMAKE_CXX = $(KVS_CPP)
}

include( SupportQt.pri )
