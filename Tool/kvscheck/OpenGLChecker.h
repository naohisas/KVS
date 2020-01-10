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

} // end of namespace kvscheck
