#*****************************************************************************
#  $Id: Makefile 1497 2013-04-04 07:17:54Z naohisa.sakamoto@gmail.com $
#*****************************************************************************

#=============================================================================
#  Include.
#=============================================================================
include ../kvs.conf
include ../Makefile.def


#=============================================================================
#  INCLUDE_PATH, LIBRARY_PATH, LINK_LIBRARY, INSTALL_DIR.
#=============================================================================
INCLUDE_PATH :=
LIBRARY_PATH :=
LINK_LIBRARY :=
INSTALL_DIR  := $(KVS_DIR)


#=============================================================================
#  Sub directory.
#=============================================================================
SUBDIRS := Core

ifeq "$(KVS_SUPPORT_GLUT)" "1"
SUBDIRS += SupportGLUT
endif

ifeq "$(KVS_SUPPORT_QT)" "1"
SUBDIRS += SupportQt
endif

ifeq "$(KVS_SUPPORT_SAGE)" "1"
SUBDIRS += SupportSAGE
endif

ifeq "$(KVS_SUPPORT_OPENCV)" "1"
SUBDIRS += SupportOpenCV
endif

ifeq "$(KVS_SUPPORT_CUDA)" "1"
SUBDIRS += SupportCUDA
endif

ifeq "$(KVS_SUPPORT_OPENCABIN)" "1"
SUBDIRS += SupportOpenCABIN
endif


#=============================================================================
#  build.
#=============================================================================
build:
	for i in $(SUBDIRS); do \
	$(MAKE) -C $$i $(MFLAGS) $(MAKEOVERRIDES) build; done


#=============================================================================
#  clean.
#=============================================================================
clean:
	for i in $(SUBDIRS); do \
	$(MAKE) -C $$i $(MFLAGS) $(MAKEOVERRIDES) clean; done


#=============================================================================
#  install.
#=============================================================================
install:
	$(MKDIR) $(INSTALL_DIR)/include/kvs
	$(INSTALL) kvsCore $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs
ifeq "$(KVS_SUPPORT_GLUT)" "1"
	$(MKDIR) $(INSTALL_DIR)/include/kvs/glut
	$(INSTALL) kvsSupportGLUT $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs/glut -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs/glut
endif
ifeq "$(KVS_SUPPORT_QT)" "1"
	$(MKDIR) $(INSTALL_DIR)/include/kvs/qt
	$(INSTALL) kvsSupportQt $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs/qt -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs/qt
endif
ifeq "$(KVS_SUPPORT_OPENCV)" "1"
	$(MKDIR) $(INSTALL_DIR)/include/kvs/opencv
	$(INSTALL) kvsSupportOpenCV $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs/opencv -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs/opencv
endif
ifeq "$(KVS_SUPPORT_OPENCABIN)" "1"
	$(MKDIR) $(INSTALL_DIR)/include/kvs/opencabin
	$(INSTALL) kvsSupportOpenCABIN $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs/opencabin -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs/opencabin
endif
ifeq "$(KVS_SUPPORT_CUDA)" "1"
	$(MKDIR) $(INSTALL_DIR)/include/kvs/cuda
	$(INSTALL) kvsSupportCUDA $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs/cuda -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs/cuda
endif
ifeq "$(KVS_SUPPORT_SAGE)" "1"
	$(MKDIR) $(INSTALL_DIR)/include/kvs/sage
	$(INSTALL) kvsSupportSAGE $(INSTALL_DIR)/include
	$(INSTALL) $(shell find kvs/sage -maxdepth 1 -type f) $(INSTALL_DIR)/include/kvs/sage
endif
	for i in $(SUBDIRS); do \
	$(MAKE) -C $$i $(MFLAGS) $(MAKEOVERRIDES) install; done
