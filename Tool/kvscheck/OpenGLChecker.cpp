/****************************************************************************/
/**
 *  @file OpenGLChecker.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpenGLChecker.cpp 1437 2013-03-18 01:42:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "OpenGLChecker.h"
#include <kvs/OpenGL>
#include <kvs/Message>

#if defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/GLUT>
#endif


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  Constructs.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 */
/*===========================================================================*/
OpenGLChecker::OpenGLChecker( int argc, char** argv )
{
#if defined( KVS_SUPPORT_GLUT )
    /* Using OS-dependent Window System API, such as aglCreateContext, glXCreateContext,
     * or wglCreateContext, instead of glutInit and glutCreateWindow, GLUT is not required ???
     */
    glutInit( &argc, argv );
    glutCreateWindow("");

    m_vendor = kvs::OpenGL::Vendor();
    m_renderer = kvs::OpenGL::Renderer();
    m_gl_version = kvs::OpenGL::Version();
    m_glsl_version = kvs::OpenGL::GLSLVersion();
    m_glu_version = kvs::OpenGL::GLUVersion();
    m_glew_version = kvs::OpenGL::GLEWVersion();
#else
    kvsMessageError(
        "It seems that KVS_SUPPORT_GLUT option is disabled in the installed KVS. "
        "GLUT is required to check OpenGL information using kvscheck command.");
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns vendor information.
 *  @return vender information
 */
/*===========================================================================*/
const std::string& OpenGLChecker::vendor() const
{
    return m_vendor;
}

/*===========================================================================*/
/**
 *  @brief  Returns renderer (GPU) information.
 *  @return render information
 */
/*===========================================================================*/
const std::string& OpenGLChecker::renderer() const
{
    return m_renderer;
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL version.
 *  @return GL version
 */
/*===========================================================================*/
const std::string& OpenGLChecker::GLVersion() const
{
    return m_gl_version;
}

/*===========================================================================*/
/**
 *  @brief  Returns GLSL version.
 *  @return GLSL version
 */
/*===========================================================================*/
const std::string& OpenGLChecker::GLSLVersion() const
{
    return m_glsl_version;
}

/*===========================================================================*/
/**
 *  @brief  Returns GLU version.
 *  @return GLU version
 */
/*===========================================================================*/
const std::string& OpenGLChecker::GLUVersion() const
{
    return m_glu_version;
}

/*===========================================================================*/
/**
 *  @brief  Returns GLEW version.
 *  @return GLEW version
 */
/*===========================================================================*/
const std::string& OpenGLChecker::GLEWVersion() const
{
    return m_glew_version;
}

/*==========================================================================*/
/**
 *  @brief Output platform information.
 *  @param os      [in] output stream
 *  @param checker [in] OpenGL information checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const OpenGLChecker& checker )
{
    os << "Vendor       : " << checker.vendor() << std::endl;
    os << "Renderer     : " << checker.renderer() << std::endl;
    os << "GL Version   : " << checker.GLVersion() << std::endl;
    os << "GLSL Version : " << checker.GLSLVersion() << std::endl;
    os << "GLU Version  : " << checker.GLUVersion() << std::endl;
    os << "GLEW Version : " << checker.GLEWVersion();
    return os;
}

} // end of namespace kvscheck
