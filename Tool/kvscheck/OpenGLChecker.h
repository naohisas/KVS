/****************************************************************************/
/**
 *  @file OpenGLChecker.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: OpenGLChecker.h 1437 2013-03-18 01:42:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVSCHECK__OPENGL_CHECKER_H_INCLUDE
#define KVSCHECK__OPENGL_CHECKER_H_INCLUDE

#include <string>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  OpenGL information checker class.
 */
/*===========================================================================*/
class OpenGLChecker
{
protected:

    std::string m_vendor; ///< vendor name
    std::string m_renderer; ///< renderer (GPU) name
    std::string m_gl_version; ///< OpenGL version
    std::string m_glsl_version; ///< GLSL version
    std::string m_glu_version; ///< GLU version
    std::string m_glew_version; ///< GLEW version

public:

    OpenGLChecker( int argc, char** argv );

    const std::string& vendor() const;
    const std::string& renderer() const;
    const std::string& GLVersion() const;
    const std::string& GLSLVersion() const;
    const std::string& GLUVersion() const;
    const std::string& GLEWVersion() const;
    friend std::ostream& operator << ( std::ostream& os, const OpenGLChecker& checker );
};

} // end of namespace kvscheck

#endif // KVSCHECK__OPENGL_CHECKER_H_INCLUDE
