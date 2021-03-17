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
#if defined( KVS_SUPPORT_GLFW )
#elif defined( KVS_SUPPORT_GLUT )
    int m_argc;
    char** m_argv;
#else
#endif

public:
#if defined( KVS_SUPPORT_GLFW )
    OpenGL( int, char** ) {}
#elif defined( KVS_SUPPORT_GLUT )
    OpenGL( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}
#else
    OpenGL( int, char** ) {}
#endif

    int exec()
    {
#if defined( KVS_SUPPORT_GLFW )
        glfwInit();
        glfwMakeContextCurrent( glfwCreateWindow( 1, 1, "", nullptr, nullptr ) );
#elif defined( KVS_SUPPORT_GLUT )
        /* Using OS-dependent Window System API, such as aglCreateContext, glXCreateContext,
         * or wglCreateContext, instead of glutInit and glutCreateWindow, GLUT is not required ???
         */
        glutInit( &m_argc, m_argv );
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
};

} // end of namespace kvscheck
