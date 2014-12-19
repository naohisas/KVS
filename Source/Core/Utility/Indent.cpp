/*****************************************************************************/
/**
 *  @file   Indent.cpp
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
#include "Indent.h"
#include <string>


namespace kvs
{

Indent::Indent( const size_t width ):
    m_width( width )
{
}

Indent Indent::nextIndent() const
{
    const size_t width = ( m_width == 0 ) ? 4 : m_width * 2;
    return Indent( width );
}

std::ostream& operator << ( std::ostream& os, const kvs::Indent& indent )
{
    const std::string blanks( indent.m_width, ' ' );
    os << blanks;
    return os;
}

} // end of namespace kvs
