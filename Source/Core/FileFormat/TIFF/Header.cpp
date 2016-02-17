/****************************************************************************/
/**
 *  @file   Header.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Header.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Header.h"
#include <string>

namespace
{

const kvs::UInt16 BigEndian    = 0x4d4d;
const kvs::UInt16 LittleEndian = 0x4949;

} // end of namespace


namespace kvs
{

namespace tiff
{

Header::Header():
    m_magic( 0 ),
    m_version( 0 ),
    m_offset( 0 )
{
}

Header::Header( std::ifstream& ifs )
{
    this->read( ifs );
}

std::ostream& operator << ( std::ostream& os, const Header& header )
{
    std::string magic( "" );
    if ( header.magic() == ::BigEndian )         magic.append("Big endian");
    else if ( header.magic() == ::LittleEndian ) magic.append("Little endian");
    else                                         magic.append("Unknown");

    os << "Magic number: " << header.magic() << " (" << magic << ")" << std::endl;
    os << "Version:      " << header.version() << std::endl;
    os << "Offset:       " << header.offset();

    return os;
}

void Header::print( std::ostream& os, const kvs::Indent& indent ) const
{
    std::string magic( "" );
    if ( m_magic == ::BigEndian ) magic.append("Big endian");
    else if ( m_magic == ::LittleEndian ) magic.append("Little endian");
    else magic.append("Unknown");

    os << indent << "Magic number : " << m_magic << " (" << magic << ")" << std::endl;
    os << indent << "Version : " << m_version << std::endl;
    os << indent << "Offset : " << m_offset << std::endl;
}

bool Header::read( std::ifstream& ifs )
{
    ifs.clear();
    ifs.seekg( 0, std::ios::beg );

    ifs.read( reinterpret_cast<char*>(&m_magic),   2 );
    if ( ifs.gcount() != 2 ) return false;

    ifs.read( reinterpret_cast<char*>(&m_version), 2 );
    if ( ifs.gcount() != 2 ) return false;

    ifs.read( reinterpret_cast<char*>(&m_offset),  4 );
    if ( ifs.gcount() != 4 ) return false;

    ifs.seekg( m_offset, std::ios::beg );

    return true;
}

} // end of namespace tiff

} // end of namespace kvs
