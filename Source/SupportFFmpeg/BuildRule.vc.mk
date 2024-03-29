#  DON'T EDIT THIS FILE.
#  THIS IS GENERATED BY "Configuration/configure_BuildRule.py".


OBJECTS = \
$(OUTDIR)\.\AvCpp\audioresampler.obj \
$(OUTDIR)\.\AvCpp\averror.obj \
$(OUTDIR)\.\AvCpp\avtime.obj \
$(OUTDIR)\.\AvCpp\avutils.obj \
$(OUTDIR)\.\AvCpp\codec.obj \
$(OUTDIR)\.\AvCpp\codeccontext.obj \
$(OUTDIR)\.\AvCpp\dictionary.obj \
$(OUTDIR)\.\AvCpp\format.obj \
$(OUTDIR)\.\AvCpp\formatcontext.obj \
$(OUTDIR)\.\AvCpp\frame.obj \
$(OUTDIR)\.\AvCpp\packet.obj \
$(OUTDIR)\.\AvCpp\pixelformat.obj \
$(OUTDIR)\.\AvCpp\rational.obj \
$(OUTDIR)\.\AvCpp\rect.obj \
$(OUTDIR)\.\AvCpp\sampleformat.obj \
$(OUTDIR)\.\AvCpp\stream.obj \
$(OUTDIR)\.\AvCpp\timestamp.obj \
$(OUTDIR)\.\AvCpp\videorescaler.obj \
$(OUTDIR)\.\Demuxer.obj \
$(OUTDIR)\.\Object\MovieObject.obj \
$(OUTDIR)\.\Renderer\MovieRenderer.obj \
$(OUTDIR)\.\Renderer\SphericalMovieRenderer.obj \



{.\Renderer\}.cpp{$(OUTDIR)\.\Renderer\}.obj::
	IF NOT EXIST $(OUTDIR)\.\Renderer $(MKDIR) $(OUTDIR)\.\Renderer
	$(CPP) /c $(CPPFLAGS) $(DEFINITIONS) $(INCLUDE_PATH) /Fo$(OUTDIR)\.\Renderer\ @<<
$<
<<

{.\Object\}.cpp{$(OUTDIR)\.\Object\}.obj::
	IF NOT EXIST $(OUTDIR)\.\Object $(MKDIR) $(OUTDIR)\.\Object
	$(CPP) /c $(CPPFLAGS) $(DEFINITIONS) $(INCLUDE_PATH) /Fo$(OUTDIR)\.\Object\ @<<
$<
<<

{.\AvCpp\}.cpp{$(OUTDIR)\.\AvCpp\}.obj::
	IF NOT EXIST $(OUTDIR)\.\AvCpp $(MKDIR) $(OUTDIR)\.\AvCpp
	$(CPP) /c $(CPPFLAGS) $(DEFINITIONS) $(INCLUDE_PATH) /Fo$(OUTDIR)\.\AvCpp\ @<<
$<
<<

{.\}.cpp{$(OUTDIR)\.\}.obj::
	IF NOT EXIST $(OUTDIR)\. $(MKDIR) $(OUTDIR)\.
	$(CPP) /c $(CPPFLAGS) $(DEFINITIONS) $(INCLUDE_PATH) /Fo$(OUTDIR)\.\ @<<
$<
<<


install::
	IF NOT EXIST $(INSTALL_DIR)\include\SupportFFmpeg\. $(MKDIR) $(INSTALL_DIR)\include\SupportFFmpeg\.
	$(INSTALL) .\*.h $(INSTALL_DIR)\include\SupportFFmpeg\.
	IF NOT EXIST $(INSTALL_DIR)\include\SupportFFmpeg\.\AvCpp $(MKDIR) $(INSTALL_DIR)\include\SupportFFmpeg\.\AvCpp
	$(INSTALL) .\AvCpp\*.h $(INSTALL_DIR)\include\SupportFFmpeg\.\AvCpp
	IF NOT EXIST $(INSTALL_DIR)\include\SupportFFmpeg\.\Object $(MKDIR) $(INSTALL_DIR)\include\SupportFFmpeg\.\Object
	$(INSTALL) .\Object\*.h $(INSTALL_DIR)\include\SupportFFmpeg\.\Object
	IF NOT EXIST $(INSTALL_DIR)\include\SupportFFmpeg\.\Renderer $(MKDIR) $(INSTALL_DIR)\include\SupportFFmpeg\.\Renderer
	$(INSTALL) .\Renderer\*.h $(INSTALL_DIR)\include\SupportFFmpeg\.\Renderer
