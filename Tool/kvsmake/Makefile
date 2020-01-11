#*****************************************************************************
#  $Id: Makefile 1409 2012-12-21 15:19:08Z naohisa.sakamoto@gmail.com $
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
#  Project name.
#=============================================================================
PROJECT_NAME := kvsmake

ifeq "$(findstring CYGWIN,$(shell uname -s))" "CYGWIN"
TARGET_EXE := $(OUTDIR)/$(PROJECT_NAME).exe
else
TARGET_EXE := $(OUTDIR)/$(PROJECT_NAME)
endif


#=============================================================================
#  Object.
#=============================================================================
OBJECTS := \
$(OUTDIR)/Makefile.o \
$(OUTDIR)/QtProject.o \
$(OUTDIR)/VCProject.o \
$(OUTDIR)/VCXProject.o \
$(OUTDIR)/VCProjectCUDA.o \
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
	$(INSTALL) Makefile.kvs.template $(INSTALL_DIR)/bin
	$(INSTALL) QtProject.template $(INSTALL_DIR)/bin
	$(INSTALL) VCProject.template $(INSTALL_DIR)/bin
	$(INSTALL) VCXProject.template $(INSTALL_DIR)/bin
	$(INSTALL) VCProjectCUDA.template $(INSTALL_DIR)/bin
	$(INSTALL_EXE) $(TARGET_EXE) $(INSTALL_DIR)/bin
