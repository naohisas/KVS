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
#ifndef KVSCHECK__EXTENSION_CHECKER_H_INCLUDE
#define KVSCHECK__EXTENSION_CHECKER_H_INCLUDE

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
protected:

    kvs::StringList m_gl_extensions; ///< OpenGL extensions
    kvs::StringList m_glu_extensions; ///< GLU extensions

public:

    ExtensionChecker( int argc, char** argv );

    const kvs::StringList& GLExtensions() const;
    const kvs::StringList& GLUExtensions() const;
    friend std::ostream& operator << ( std::ostream& os, const ExtensionChecker& checker );
};

} // end of namespace kvscheck

#endif // KVSCHECK__EXTENSION_CHECKER_H_INCLUDE
