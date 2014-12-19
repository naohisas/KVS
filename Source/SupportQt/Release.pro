#*****************************************************************************
#  $Id: Release.pro 62 2009-03-19 06:58:44Z naohisa.sakamoto $
#*****************************************************************************

CONFIG  += release
DEFINES += NDEBUG

MOC_DIR     = Release
OBJECTS_DIR = Release
DESTDIR     = Release

include( SupportQt.pri )
