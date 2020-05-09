#*****************************************************************************
#  $Id: Makefile 1656 2013-11-26 14:03:49Z naohisa.sakamoto@gmail.com $
#*****************************************************************************

#=============================================================================
#  Include.
#=============================================================================
include ../../kvs.conf
include ../../Makefile.def


#=============================================================================
#  INCLUDE_PATH, LIBRARY_PATH, LINK_LIBRARY, INSTALL_DIR.
#=============================================================================
INCLUDE_PATH := -I../../Source
LIBRARY_PATH := -L../../Source/Core/$(OUTDIR)
LINK_LIBRARY := -lkvsCore
INSTALL_DIR  := $(KVS_DIR)


#=============================================================================
#  Include path.
#=============================================================================
ifeq "$(KVS_SUPPORT_CUDA)" "1"
INCLUDE_PATH += $(CUDA_INCLUDE_PATH)
endif

ifeq "$(KVS_SUPPORT_GLUT)" "1"
INCLUDE_PATH += $(GLUT_INCLUDE_PATH)
endif

ifeq "$(KVS_SUPPORT_OPENCV)" "1"
INCLUDE_PATH += $(OPENCV_INCLUDE_PATH)
endif

ifeq "$(KVS_SUPPORT_PYTHON)" "1"
INCLUDE_PATH += $(PYTHON_INCLUDE_PATH)
endif

ifeq "$(KVS_SUPPORT_EGL)" "1"
INCLUDE_PATH += $(EGL_INCLUDE_PATH)
endif

INCLUDE_PATH += $(GLEW_INCLUDE_PATH)
INCLUDE_PATH += $(GL_INCLUDE_PATH)


#=============================================================================
#  Library path.
#=============================================================================
ifeq "$(KVS_SUPPORT_CUDA)" "1"
LIBRARY_PATH += -L../../Source/SupportCUDA/$(OUTDIR) $(CUDA_LIBRARY_PATH)
endif

ifeq "$(KVS_SUPPORT_GLUT)" "1"
LIBRARY_PATH += -L../../Source/SupportGLUT/$(OUTDIR) $(GLUT_LIBRARY_PATH)
endif

ifeq "$(KVS_SUPPORT_OPENCV)" "1"
LIBRARY_PATH += -L../../Source/SupportOpenCV/$(OUTDIR) $(OPENCV_LIBRARY_PATH)
endif

ifeq "$(KVS_SUPPORT_PYTHON)" "1"
LIBRARY_PATH += -L../../Source/SupportPython/$(OUTDIR) $(PYTHON_LIBRARY_PATH)
endif

ifeq "$(KVS_SUPPORT_EGL)" "1"
LIBRARY_PATH += -L../../Source/SupportEGL/$(OUTDIR) $(EGL_LIBRARY_PATH)
endif

ifeq "$(KVS_SUPPORT_OSMESA)" "1"
LIBRARY_PATH += -L../../Source/SupportOSMesa/$(OUTDIR)
endif

LIBRARY_PATH += $(GLEW_LIBRARY_PATH)
LIBRARY_PATH += $(GL_LIBRARY_PATH)


#=============================================================================
#  Link library.
#=============================================================================
ifeq "$(KVS_SUPPORT_CUDA)" "1"
LINK_LIBRARY += -lkvsSupportCUDA $(CUDA_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_GLUT)" "1"
LINK_LIBRARY += -lkvsSupportGLUT $(GLUT_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_OPENCV)" "1"
LINK_LIBRARY += -lkvsSupportOpenCV $(OPENCV_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_PYTHON)" "1"
LINK_LIBRARY += -lkvsSupportPython $(PYTHON_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_EGL)" "1"
LINK_LIBRARY += -lkvsSupportEGL $(EGL_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_OSMESA)" "1"
LINK_LIBRARY += -lkvsSupportOSMesa
endif

LINK_LIBRARY += $(GLEW_LINK_LIBRARY)
LINK_LIBRARY += $(GL_LINK_LIBRARY)


#=============================================================================
#  Project name.
#=============================================================================
PROJECT_NAME := kvscheck

ifeq "$(findstring CYGWIN,$(shell uname -s))" "CYGWIN"
TARGET_EXE := $(OUTDIR)/$(PROJECT_NAME).exe
else
TARGET_EXE := $(OUTDIR)/$(PROJECT_NAME)
endif


#=============================================================================
#  Object.
#=============================================================================
OBJECTS := \
$(OUTDIR)/main.o \


#=============================================================================
#  Build rule.
#=============================================================================
$(TARGET_EXE): $(OBJECTS)
	$(LD) $(LDFLAGS) $(LIBRARY_PATH) -o $@ $^ $(LINK_LIBRARY)

$(OUTDIR)/%.o: %.cpp %.h
	$(MKDIR) $(OUTDIR)
	$(CPP) -c $(CPPFLAGS) $(DEFINITIONS) $(INCLUDE_PATH) -o $@ $<

$(OUTDIR)/%.o: %.cpp
	$(MKDIR) $(OUTDIR)
	$(CPP) -c $(CPPFLAGS) $(DEFINITIONS) $(INCLUDE_PATH) -o $@ $<


#=============================================================================
#  build.
#=============================================================================
build: $(TARGET_EXE)


#=============================================================================
#  clean.
#=============================================================================
clean:
	$(RMDIR) $(OUTDIR)


#=============================================================================
#  install.
#=============================================================================
install:
	$(MKDIR) $(INSTALL_DIR)/bin
	$(INSTALL_EXE) $(TARGET_EXE) $(INSTALL_DIR)/bin
