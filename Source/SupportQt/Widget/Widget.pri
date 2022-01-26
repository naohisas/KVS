#=============================================================================
#  sources and headers
#=============================================================================
WIDGET_HEADERS += \
Widget/TransferFunctionEditor.h \


WIDGET_SOURCES += \
Widget/TransferFunctionEditor.cpp \


HEADERS += $$WIDGET_HEADERS
SOURCES += $$WIDGET_SOURCES


#=============================================================================
#  install
#=============================================================================
WIDGET_INSTALL_HEADERS.files = $$WIDGET_HEADERS
WIDGET_INSTALL_HEADERS.path  = $$KVS_DIR/include/SupportQt/Widget

INSTALLS += WIDGET_INSTALL_HEADERS
