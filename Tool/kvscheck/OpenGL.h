/*****************************************************************************/
/**
 *  @file   OpenGL.h
 *  @author Naohisa Sakamoto
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
#if defined( KVS_SUPPORT_GLFW )
#include <kvs/glfw/GLFW>
#endif


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  OpenGL version checker class.
 */
/*===========================================================================*/
class OpenGL : public kvs::Program
{
    int exec( int argc, char** argv );
};

/*===========================================================================*/
/**
 *  @brief  Executes the checker program.
 *  @param  argc [in] argument count
 *  @param  argv [in] argument values
 *  @return 0 if the process is done sucessfully
 */
/*===========================================================================*/
inline int OpenGL::exec( int argc, char** argv )
{
#if defined( KVS_SUPPORT_GLFW )
    glfwInit();
    glfwMakeContextCurrent( glfwCreateWindow( 1, 1, "", nullptr, nullptr ) );
#elif defined( KVS_SUPPORT_GLUT )
    /* Using OS-dependent Window System API, such as aglCreateContext, glXCreateContext,
     * or wglCreateContext, instead of glutInit and glutCreateWindow, GLUT is not required ???
     */
    glutInit( &argc, argv );
    glutCreateWindow("");
#else
    kvsMessageError() << "Cannot create OpenGL context." << std::endl;
    return 1;
#endif

    std::cout << "Vendor       : " << kvs::OpenGL::Vendor() << std::endl;
    std::cout << "Renderer     : " << kvs::OpenGL::Renderer() << std::endl;
    std::cout << "GL Version   : " << kvs::OpenGL::Version() << std::endl;
    std::cout << "GLSL Version : " << kvs::OpenGL::GLSLVersion() << std::endl;
    std::cout << "GLU Version  : " << kvs::OpenGL::GLUVersion() << std::endl;
    std::cout << "GLEW Version : " << kvs::OpenGL::GLEWVersion() << std::endl;

    return 0;
}

} // end of namespace kvscheck
