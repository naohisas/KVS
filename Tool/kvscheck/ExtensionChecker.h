/****************************************************************************/
/**
 *  @file   ExtensionChecker.h
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
/****************************************************************************/
#pragma once
#include <string>
#include <kvs/StringList>


namespace kvscheck
{

/*===========================================================================*/
/**
 *  @brief  OpenGL extension information checker class.
 */
/*===========================================================================*/
class ExtensionChecker
{
private:
    kvs::StringList m_gl_extensions; ///< OpenGL extensions
    kvs::StringList m_glu_extensions; ///< GLU extensions

public:
    ExtensionChecker( int argc, char** argv );
    const kvs::StringList& GLExtensions() const { return m_gl_extensions; }
    const kvs::StringList& GLUExtensions() const { return m_glu_extensions; }
    friend std::ostream& operator << ( std::ostream& os, const ExtensionChecker& checker );
};

} // end of namespace kvscheck
