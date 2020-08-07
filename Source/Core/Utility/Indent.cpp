/*****************************************************************************/
/**
 *  @file   Indent.cpp
 *  @author Naohisa Sakamoto
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
