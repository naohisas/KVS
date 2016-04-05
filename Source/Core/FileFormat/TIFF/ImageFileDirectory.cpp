/****************************************************************************/
/**
 *  @file   ImageFileDirectory.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageFileDirectory.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "ImageFileDirectory.h"


namespace kvs
{

namespace tiff
{

ImageFileDirectory::ImageFileDirectory():
    m_offset( 0 )
{
}

ImageFileDirectory::ImageFileDirectory( std::ifstream& ifs )
{
    this->read( ifs );
}

const std::list<tiff::Entry>& ImageFileDirectory::entryList() const
{
    return m_entry_list;
}

kvs::UInt32 ImageFileDirectory::offset() const
{
    return m_offset;
}

void ImageFileDirectory::print( std::ostream& os, const kvs::Indent& indent ) const
{
    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry = m_entry_list.begin();
    kvs::tiff::ImageFileDirectory::EntryList::const_iterator last = m_entry_list.end();
    while ( entry != last )
    {
        entry->print( os, indent );
        ++entry;
    }
}

bool ImageFileDirectory::read( std::ifstream& ifs )
{
    kvs::UInt16 nentries = 0;
    ifs.read( reinterpret_cast<char*>( &nentries ), 2 );
    if ( ifs.gcount() != 2 ) return false;

    for( size_t i = 0; i < nentries; i++ )
    {
        m_entry_list.push_back( kvs::tiff::Entry( ifs ) );
    }

    ifs.read( reinterpret_cast<char*>( &m_offset ), 4 );
    if ( ifs.gcount() != 4 ) return false;

    if ( m_offset > 0 )
    {
        ifs.seekg( m_offset, std::ios::beg );
    }

    return true;
}

} // end of namesapce tiff

} // end of namespace kvs
