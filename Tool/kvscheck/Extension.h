/****************************************************************************/
/**
 *  @file   Extension.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/Program>
#include <kvs/StringList>
#include <kvs/OpenGL>
#include <kvs/Message>
#if defined( KVS_SUPPORT_GLUT )
#include <kvs/glut/GLUT>
#endif


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  OpenGL extension checker class.
 */
/*===========================================================================*/
class Extension : public kvs::Program
{
    int m_argc;
    char** m_argv;

public:
    Extension( int argc, char** argv ): m_argc( argc ), m_argv( argv ) {}

    int exec()
    {
#if defined( KVS_SUPPORT_GLUT )
        /* Using OS-dependent Window System API, such as aglCreateContext, glXCreateContext,
         * or wglCreateContext, instead of glutInit and glutCreateWindow, GLUT is not required ???
         */
        glutInit( &m_argc, m_argv );
        glutCreateWindow("");

        kvs::StringList gl_extensions = kvs::OpenGL::ExtensionList();
#if defined( KVS_ENABLE_GLU ) && defined( GLU_VERSION_1_1 )
        /* The gluGetString function is not used in GLU version 1.0.
         */
        std::stringstream glu_extension_list( (char*)gluGetString( GLU_EXTENSIONS ) );
        kvs::StringList glu_extensions;
        {
            std::string glu_extension;
            while ( glu_extension_list >> glu_extension )
            {
                glu_extensions.push_back( glu_extension );
            }
        }

        std::cout << "GL Extensions (" << gl_extensions.size() << ") :" << std::endl;
        kvs::StringList::const_iterator gl_extension = gl_extensions.begin();
        while ( gl_extension != gl_extensions.end() )
        {
            std::cout << "\t" << *gl_extension << std::endl;
            ++gl_extension;
        }

        std::cout << "GLU Extensions (" << glu_extensions.size() << ") :" << std::endl;
        kvs::StringList::const_iterator glu_extension = glu_extensions.begin();
        while ( glu_extension != glu_extensions.end() )
        {
            std::cout << "\t" << *glu_extension << std::endl;
            ++glu_extension;
        }

        return 0;
#endif
#else
        kvsMessageError(
            "KVS_SUPPORT_GLUT option is disabled in the installed KVS. "
            "GLUT is required to check OpenGL information using kvscheck.");
        return 1;
#endif
        return 1;
    }
};

} // end of namespace kvscheck
