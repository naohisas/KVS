/****************************************************************************/
/**
 *  @file   FileHeader.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: FileHeader.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "FileHeader.h"
#include <kvs/Endian>
#include <string>
#include <cstring>


namespace kvs
{

namespace bmp
{

FileHeader::FileHeader( std::ifstream& ifs )
{
    this->read( ifs );
}

bool FileHeader::isBM()
{
    return !strncmp( reinterpret_cast<char*>(&m_type), "BM", 2 );
}

void FileHeader::print( std::ostream& os, const kvs::Indent& indent ) const
{
    const char* tmp = reinterpret_cast<const char*>(&m_type);
    char* num = const_cast<char*>(tmp); num[2] = '\0';
    os << indent << "Magic number : " << (std::string)num << std::endl;
    os << indent << "Size : " << m_size << std::endl;
    os << indent << "Reserved 1 : " << m_reserved1 << std::endl;
    os << indent << "Reserved 2 : " << m_reserved2 << std::endl;
    os << indent << "Offset : " << m_offset << std::endl;
}

void FileHeader::read( std::ifstream& ifs )
{
    BaseClass::ReadValue( ifs, &m_type );
    BaseClass::ReadValue( ifs, &m_size );
    BaseClass::ReadValue( ifs, &m_reserved1 );
    BaseClass::ReadValue( ifs, &m_reserved2 );
    BaseClass::ReadValue( ifs, &m_offset );
    BMP_HEADER_SWAP_BYTES;
}

void FileHeader::write( std::ofstream& ofs )
{
    BMP_HEADER_SWAP_BYTES;
    BaseClass::WriteValue( ofs, m_type );
    BaseClass::WriteValue( ofs, m_size );
    BaseClass::WriteValue( ofs, m_reserved1 );
    BaseClass::WriteValue( ofs, m_reserved2 );
    BaseClass::WriteValue( ofs, m_offset );
    BMP_HEADER_SWAP_BYTES;
}

void FileHeader::swapBytes()
{
    kvs::Endian::Swap( &m_size );
    kvs::Endian::Swap( &m_reserved1 );
    kvs::Endian::Swap( &m_reserved2 );
    kvs::Endian::Swap( &m_offset );
}

} // end of namespace bmp

} // end of namespace kvs
