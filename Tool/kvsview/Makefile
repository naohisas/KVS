#*****************************************************************************
#  $Id: Makefile 1500 2013-04-04 07:56:40Z naohisa.sakamoto@gmail.com $
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

INCLUDE_PATH += $(OPENMP_INCLUDE_PATH)
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

LIBRARY_PATH += $(OPENMP_LIBRARY_PATH)
LIBRARY_PATH += $(GLEW_LIBRARY_PATH)
LIBRARY_PATH += $(GL_LIBRARY_PATH)


#=============================================================================
#  Link library.
#=============================================================================
ifeq "$(KVS_SUPPORT_CUDA)" "1"
LINK_LIBRARY += $(CUDA_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_GLUT)" "1"
LINK_LIBRARY += -lkvsSupportGLUT $(GLUT_LINK_LIBRARY)
endif

ifeq "$(KVS_SUPPORT_OPENCV)" "1"
LINK_LIBRARY += -lkvsSupportOpenCV $(OPENCV_LINK_LIBRARY)
endif

LINK_LIBRARY += -lkvsCore
LINK_LIBRARY += $(OPENMP_LINK_LIBRARY)
LINK_LIBRARY += $(GLEW_LINK_LIBRARY)
LINK_LIBRARY += $(GL_LINK_LIBRARY)


#=============================================================================
#  Project name.
#=============================================================================
PROJECT_NAME := kvsview

ifeq "$(findstring CYGWIN,$(shell uname -s))" "CYGWIN"
TARGET_EXE := $(OUTDIR)/$(PROJECT_NAME).exe
else
TARGET_EXE := $(OUTDIR)/$(PROJECT_NAME)
endif


#=============================================================================
#  Object.
#=============================================================================
OBJECTS := \
$(OUTDIR)/Argument.o \
$(OUTDIR)/Default.o \
$(OUTDIR)/Isosurface.o \
$(OUTDIR)/SlicePlane.o \
$(OUTDIR)/OrthoSlice.o \
$(OUTDIR)/PointRenderer.o \
$(OUTDIR)/LineRenderer.o \
$(OUTDIR)/PolygonRenderer.o \
$(OUTDIR)/TransferFunction.o \
$(OUTDIR)/ExtractEdges.o \
$(OUTDIR)/ExtractVertices.o \
$(OUTDIR)/ExternalFaces.o \
$(OUTDIR)/RayCastingRenderer.o \
$(OUTDIR)/ParticleBasedRenderer.o \
$(OUTDIR)/Histogram.o \
$(OUTDIR)/Widget.o \
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
