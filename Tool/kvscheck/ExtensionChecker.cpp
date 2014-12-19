/****************************************************************************/
/**
 *  @file   ExtensionChecker.cpp
 *  @author Naohisa Sakamtoo
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id$
 */
/****************************************************************************/
#include "ExtensionChecker.h"
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
ExtensionChecker::ExtensionChecker( int argc, char** argv )
{
#if defined( KVS_SUPPORT_GLUT )
    /* Using OS-dependent Window System API, such as aglCreateContext, glXCreateContext,
     * or wglCreateContext, instead of glutInit and glutCreateWindow, GLUT is not required ???
     */
    glutInit( &argc, argv );
    glutCreateWindow("");

    m_gl_extensions = kvs::OpenGL::ExtensionList();
#if defined( GLU_VERSION_1_1 )
    /* The gluGetString function is not used in GLU version 1.0.
     */
    std::stringstream glu_extension_list( (char*)gluGetString( GLU_EXTENSIONS ) );
    std::string glu_extension;
    while ( glu_extension_list >> glu_extension )
    {
        m_glu_extensions.push_back( glu_extension );
    }
#endif
#else
    kvsMessageError(
        "It seems that KVS_SUPPORT_GLUT option is disabled in the installed KVS. "
        "GLUT is required to check OpenGL information using kvscheck command.");
#endif
}

/*===========================================================================*/
/**
 *  @brief  Returns OpenGL extension list.
 *  @return GL extension list
 */
/*===========================================================================*/
const kvs::StringList& ExtensionChecker::GLExtensions() const
{
    return m_gl_extensions;
}

/*===========================================================================*/
/**
 *  @brief  Returns GLU extension list.
 *  @return GLU extension list
 */
/*===========================================================================*/
const kvs::StringList& ExtensionChecker::GLUExtensions() const
{
    return m_glu_extensions;
}

/*==========================================================================*/
/**
 *  @brief Output platform information.
 *  @param os      [in] output stream
 *  @param checker [in] OpenGL extension information checker
 */
/*==========================================================================*/
std::ostream& operator << ( std::ostream& os, const ExtensionChecker& checker )
{
    os << "GL Extensions (" << checker.GLExtensions().size() << ") :" << std::endl;
    kvs::StringList::const_iterator gl_extension = checker.GLExtensions().begin();
    while ( gl_extension != checker.GLExtensions().end() )
    {
        os << "\t" << *gl_extension << std::endl;
        ++gl_extension;
    }

    os << "GLU Extensions (" << checker.GLUExtensions().size() << ") :" << std::endl;
    kvs::StringList::const_iterator glu_extension = checker.GLUExtensions().begin();
    while ( glu_extension != checker.GLUExtensions().end() )
    {
        os << "\t" << *glu_extension << std::endl;
        ++glu_extension;
    }

    return os;
}

} // end of namespace kvscheck
