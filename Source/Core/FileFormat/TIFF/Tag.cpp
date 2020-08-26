/****************************************************************************/
/**
 *  @file   Tag.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "Tag.h"
#include <kvs/Type>
#include <string>


namespace kvs
{

namespace tiff
{

Tag::Tag():
    m_id( 0 ),
    m_name( "" )
{
}

Tag::Tag( const kvs::UInt16 id, const std::string& name ):
    m_id( id ),
    m_name( name )
{
}

Tag::Tag( const Tag& tag ):
    m_id( tag.m_id ),
    m_name( tag.m_name )
{
}

Tag& Tag::operator = ( const Tag& tag )
{
    m_id   = tag.m_id;
    m_name = tag.m_name;
    return *this;
}

} // end of namespace tiff

} // end of namespace kvs
