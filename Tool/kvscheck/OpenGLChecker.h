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


namespace kvscheck
{

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
