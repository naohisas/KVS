/****************************************************************************/
/**
 *  @file FileHeader.cpp
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

FileHeader::FileHeader()
{
}

FileHeader::FileHeader( std::ifstream& ifs )
{
    this->read( ifs );
}

kvs::UInt16 FileHeader::type() const
{
    return m_type;
}

kvs::UInt32 FileHeader::size() const
{
    return m_size;
}

kvs::UInt16 FileHeader::reserved1() const
{
    return m_reserved1;
}

kvs::UInt16 FileHeader::reserved2() const
{
    return m_reserved2;
}

kvs::UInt32 FileHeader::offset() const
{
    return m_offset;
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
    os << indent << "Offset : " << m_offset;
}

void FileHeader::read( std::ifstream& ifs )
{
    BaseClass::get_value( ifs, &m_type );
    BaseClass::get_value( ifs, &m_size );
    BaseClass::get_value( ifs, &m_reserved1 );
    BaseClass::get_value( ifs, &m_reserved2 );
    BaseClass::get_value( ifs, &m_offset );
    BMP_HEADER_SWAP_BYTES;
}

void FileHeader::write( std::ofstream& ofs )
{
    BMP_HEADER_SWAP_BYTES;
    BaseClass::put_value( ofs, m_type );
    BaseClass::put_value( ofs, m_size );
    BaseClass::put_value( ofs, m_reserved1 );
    BaseClass::put_value( ofs, m_reserved2 );
    BaseClass::put_value( ofs, m_offset );
    BMP_HEADER_SWAP_BYTES;
}

void FileHeader::swap_bytes()
{
    kvs::Endian::Swap( &m_size );
    kvs::Endian::Swap( &m_reserved1 );
    kvs::Endian::Swap( &m_reserved2 );
    kvs::Endian::Swap( &m_offset );
}

} // end of namespace bmp

} // end of namespace kvs
