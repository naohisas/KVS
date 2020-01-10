/*****************************************************************************/
/**
 *  @file   OpenGLChecker.h
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
#pragma once
#include <string>
#include <kvs/Program>
#include <kvs/OpenGL>
#include <kvs/Message>
#if defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/GLUT>
#endif


namespace kvscheck
{

class OpenGL : public kvs::Program
{
public:
    int exec( int argc, char** argv );
};

inline int OpenGL::exec( int argc, char** argv )
{
#if defined( KVS_SUPPORT_GLUT )
    /* Using OS-dependent Window System API, such as aglCreateContext, glXCreateContext,
     * or wglCreateContext, instead of glutInit and glutCreateWindow, GLUT is not required ???
     */
    glutInit( &argc, argv );
    glutCreateWindow("");

    std::cout << "Vendor       : " << kvs::OpenGL::Vendor() << std::endl;
    std::cout << "Renderer     : " << kvs::OpenGL::Renderer() << std::endl;
    std::cout << "GL Version   : " << kvs::OpenGL::Version() << std::endl;
    std::cout << "GLSL Version : " << kvs::OpenGL::GLSLVersion() << std::endl;
    std::cout << "GLU Version  : " << kvs::OpenGL::GLUVersion() << std::endl;
    std::cout << "GLEW Version : " << kvs::OpenGL::GLEWVersion() << std::endl;

    return 0;
#else
    kvsMessageError(
        "KVS_SUPPORT_GLUT option is disabled in the installed KVS. "
        "GLUT is required to check OpenGL information using kvscheck.");
    return 1;
#endif

}



/*===========================================================================*/
/**
 *  @brief  OpenGL information checker class.
 */
/*===========================================================================*/
class OpenGLChecker
{
private:
    std::string m_vendor; ///< vendor name
    std::string m_renderer; ///< renderer (GPU) name
    std::string m_gl_version; ///< OpenGL version
    std::string m_glsl_version; ///< GLSL version
    std::string m_glu_version; ///< GLU version
    std::string m_glew_version; ///< GLEW version

public:
    OpenGLChecker( int argc, char** argv );
    const std::string& vendor() const { return m_vendor; }
    const std::string& renderer() const { return m_renderer; }
    const std::string& GLVersion() const { return m_gl_version; }
    const std::string& GLSLVersion() const { return m_glsl_version; }
    const std::string& GLUVersion() const { return m_glu_version; }
    const std::string& GLEWVersion() const { return m_glew_version; }
    friend std::ostream& operator << ( std::ostream& os, const OpenGLChecker& checker );
};

} // end of namespace kvscheck
