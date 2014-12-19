#*****************************************************************************
#  $Id: Debug.pro 62 2009-03-19 06:58:44Z naohisa.sakamoto $
#*****************************************************************************

CONFIG  += debug
DEFINES += _DEBUG KVS_ENABLE_DEBUG

MOC_DIR     = Debug
OBJECTS_DIR = Debug
DESTDIR     = Debug

include( SupportQt.pri )
