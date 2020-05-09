/*****************************************************************************/
/**
 *  @file   GL.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/*****************************************************************************/
#ifndef KVS__GL_H_INCLUDE
#define KVS__GL_H_INCLUDE

#include <kvs/Macro>
#include <kvs/Breakpoint>
#include <kvs/Platform>

#if !defined( KVS_PLATFORM_WINDOWS )
#define GL_GLEXT_PROTOTYPES
#endif

#if defined( KVS_PLATFORM_WINDOWS )
#include <cstdlib>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#define GLUT_DISABLE_ATEXIT_HACK
#endif

#if defined( KVS_PLATFORM_MACOSX )
#include <AvailabilityMacros.h>
// Silence deprecated OpenGL warnings.
#if MAC_OS_X_VERSION_MIN_REQUIRED >= 1090
#include <OpenGL/OpenGLAvailability.h>
#undef OPENGL_DEPRECATED
#undef OPENGL_DEPRECATED_MSG
#undef OPENGL_ENUM_DEPRECATED
#undef OPENGL_ENUM_DEPRECATED_MSG
#define OPENGL_DEPRECATED(from, to)
#define OPENGL_DEPRECATED_MSG(from, to, msg)
#define OPENGL_ENUM_DEPRECATED(from, to)
#define OPENGL_ENUM_DEPRECATED_MSG(from, to, msg)
#endif
#endif

// Includes OpenGL header files
#if defined( KVS_ENABLE_OPENGL )

// GLEW header file 'glew.h' must be included before the OpenGL header files.
#if defined( KVS_ENABLE_GLEW )
#include <GL/glew.h>
#endif

// OpenGL headers.
#if defined( KVS_PLATFORM_MACOSX )
  // Mac
  #if defined( KVS_SUPPORT_OSMESA )
    #include <GL/osmesa.h>
    #if defined( KVS_ENABLE_GLU )
      #include <GL/glu.h>
    #endif
//    #include <GL/glext.h>
  #else
    #include <OpenGL/gl.h>
    #if defined( KVS_ENABLE_GLU )
      #include <OpenGL/glu.h>
    #endif
  #endif
#else
  // Windows and Linux
  #if defined( KVS_SUPPORT_OSMESA )
    #include <GL/osmesa.h>
  #else
    #include <GL/gl.h>
  #endif
  #if defined( KVS_ENABLE_GLU )
    #include <GL/glu.h>
  #endif
//  #include <GL/glext.h>
#endif

#else
#include "GLdef.h"
#endif


namespace kvs
{

namespace GL
{

namespace detail
{

bool HasError( const char* file, const int line, const char* func, const char* command );

} // end of namespace detail

} // end of namespace GL

} // end of namespace kvs

/*===========================================================================*/
/**
 *  @def KVS_GL_CALL( command )
 *  A macro for OpenGL command safe calling. An error checking will be done
 *  before and after calling the command when the debug option (KVS_ENABLE_DEBUG)
 *  is available.
 */
/*===========================================================================*/

/*===========================================================================*/
/**
 *  @def KVS_GL_CALL_BEG( command )
 *  A macro for OpenGL command safe calling. An error checking will be done
 *  before calling the command when the debug option (KVS_ENABLE_DEBUG)
 *  is available. This is for calling glBegin function.
 */
/*===========================================================================*/

/*===========================================================================*/
/**
 *  @def KVS_GL_CALL_END( command )
 *  A macro for OpenGL command safe calling. An error checking will be done
 *  after calling the command when the debug option (KVS_ENABLE_DEBUG)
 *  is available. This is for calling glEnd function.
 */
/*===========================================================================*/

/*===========================================================================*/
/**
 *  @def KVS_GL_CALL_VER( command )
 *  A macro for OpenGL command calling without error checking. This is for
 *  calling glVertex, glNormal, glColor, and etc. which will be exectuted
 *  between glBegin and glEnd functions.
 */
/*===========================================================================*/

#if defined( KVS_ENABLE_OPENGL )

#if defined( KVS_ENABLE_DEBUG )
#define KVS_GL_CALL( command )                                          \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    if ( kvs::GL::detail::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, "Unknown" ) ) { KVS_BREAKPOINT; } \
    command;                                                            \
    if ( kvs::GL::detail::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, #command ) ) { KVS_BREAKPOINT; } \
    KVS_MACRO_MULTI_STATEMENT_END
#define KVS_GL_CALL_BEG( command )                                      \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    if ( kvs::GL::detail::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, "Unknown" ) ) { KVS_BREAKPOINT; } \
    command;                                                            \
    KVS_MACRO_MULTI_STATEMENT_END
#define KVS_GL_CALL_END( command )                                      \
    KVS_MACRO_MULTI_STATEMENT_BEGIN                                     \
    command;                                                            \
    if ( kvs::GL::detail::HasError( KVS_MACRO_FILE, KVS_MACRO_LINE, KVS_MACRO_FUNC, #command ) ) { KVS_BREAKPOINT; } \
    KVS_MACRO_MULTI_STATEMENT_END
#define KVS_GL_CALL_VER( command ) ( command )
#else
#define KVS_GL_CALL( command ) ( command )
#define KVS_GL_CALL_BEG( command ) ( command )
#define KVS_GL_CALL_END( command ) ( command )
#define KVS_GL_CALL_VER( command ) ( command )
#endif // KVS_ENABLE_DEBUG

#else
#define KVS_GL_CALL( command )
#define KVS_GL_CALL_BEG( command )
#define KVS_GL_CALL_END( command )
#define KVS_GL_CALL_VER( command )
#endif // KVS_ENABLE_OPENGL

#endif // KVS__GL_H_INCLUDE
