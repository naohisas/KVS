/****************************************************************************/
/**
 *  @file   InfoHeader.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: InfoHeader.cpp 1312 2012-09-18 15:33:43Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "InfoHeader.h"
#include <kvs/Endian>


namespace kvs
{

namespace bmp
{

InfoHeader::InfoHeader( std::ifstream& ifs )
{
    this->read( ifs );
}

void InfoHeader::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Structure size : " << m_size << std::endl;
    os << indent << "Image width : " << m_width << std::endl;
    os << indent << "Image height : " << m_height << std::endl;
    os << indent << "Number of planes : " << m_nplanes << std::endl;
    os << indent << "Bits per pixel : " << m_bpp << std::endl;
    os << indent << "Compression : " << m_compression << std::endl;
    os << indent << "Size of bitmap : " << m_bitmapsize << std::endl;
    os << indent << "Horizontal resolution : " << m_hresolution << std::endl;
    os << indent << "Vertical resolution : " << m_vresolution << std::endl;
    os << indent << "Number of colors : " << m_colsused << std::endl;
    os << indent << "Number of important colors : " << m_colsimportant << std::endl;
}

void InfoHeader::read( std::ifstream& ifs )
{
    BaseClass::ReadValue( ifs, &m_size );
    BaseClass::ReadValue( ifs, &m_width );
    BaseClass::ReadValue( ifs, &m_height );
    BaseClass::ReadValue( ifs, &m_nplanes );
    BaseClass::ReadValue( ifs, &m_bpp );
    BaseClass::ReadValue( ifs, &m_compression );
    BaseClass::ReadValue( ifs, &m_bitmapsize );
    BaseClass::ReadValue( ifs, &m_hresolution );
    BaseClass::ReadValue( ifs, &m_vresolution );
    BaseClass::ReadValue( ifs, &m_colsused );
    BaseClass::ReadValue( ifs, &m_colsimportant );
    BMP_HEADER_SWAP_BYTES;
}

void InfoHeader::write( std::ofstream& ofs )
{
    BMP_HEADER_SWAP_BYTES;
    BaseClass::WriteValue( ofs, m_size );
    BaseClass::WriteValue( ofs, m_width );
    BaseClass::WriteValue( ofs, m_height );
    BaseClass::WriteValue( ofs, m_nplanes );
    BaseClass::WriteValue( ofs, m_bpp );
    BaseClass::WriteValue( ofs, m_compression );
    BaseClass::WriteValue( ofs, m_bitmapsize );
    BaseClass::WriteValue( ofs, m_hresolution );
    BaseClass::WriteValue( ofs, m_vresolution );
    BaseClass::WriteValue( ofs, m_colsused );
    BaseClass::WriteValue( ofs, m_colsimportant );
    BMP_HEADER_SWAP_BYTES;
};

void InfoHeader::swapBytes()
{
    kvs::Endian::Swap( &m_size );
    kvs::Endian::Swap( &m_width );
    kvs::Endian::Swap( &m_height );
    kvs::Endian::Swap( &m_nplanes );
    kvs::Endian::Swap( &m_bpp );
    kvs::Endian::Swap( &m_compression );
    kvs::Endian::Swap( &m_bitmapsize );
    kvs::Endian::Swap( &m_hresolution );
    kvs::Endian::Swap( &m_vresolution );
    kvs::Endian::Swap( &m_colsused );
    kvs::Endian::Swap( &m_colsimportant );
}

} // end of namesapce bmp

} // end of namespace kvs
