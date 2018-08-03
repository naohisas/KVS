/****************************************************************************/
/**
 *  @file   Tag.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tag.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
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
