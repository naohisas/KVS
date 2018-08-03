/****************************************************************************/
/**
 *  @file   TagDictionary.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: TagDictionary.cpp 1303 2012-09-14 11:26:37Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "TagDictionary.h"
#include "TagTable.h"
#include <iostream>
#include <kvs/String>


namespace kvs
{

namespace tiff
{

TagDictionary::TagDictionary()
{
    this->create_dictionary();
}

kvs::tiff::Tag TagDictionary::find( const kvs::UInt16 tag_id ) const
{
    Container::const_iterator tag = m_container.find( tag_id );

    kvs::UInt16 id;
    std::string name;
    if ( tag == m_container.end() )
    {
        id   = tag_id;
        name = "Unknown tag (" + kvs::String::ToString( tag_id ) + ")";
    }
    else
    {
        id   = tag->first;
        name = tag->second;
    }

    return kvs::tiff::Tag( id, name );
}

void TagDictionary::create_dictionary()
{
    for ( size_t i = 0; i < kvs::tiff::TagTableSize; i++ )
    {
        kvs::UInt16 key   = kvs::tiff::TagTable[i].id();
        std::string value = kvs::tiff::TagTable[i].name();
        m_container[key] = value;
    }
}

} // end of namespace tiff

} // end of namespace kvs
