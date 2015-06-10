/****************************************************************************/
/**
 *  @file   Bmp.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Bmp.cpp 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Bmp.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <kvs/Message>
#include <kvs/File>
#include <kvs/Math>


namespace
{

void Flip( const size_t width, const size_t height, kvs::UInt8* pixels )
{
    const size_t stride = width * 3;

    kvs::UInt8* pdata = pixels;
    const size_t end_line = height / 2;
    for ( size_t i = 0; i < end_line; i++ )
    {
        kvs::UInt8* src = pdata + ( i * stride );
        kvs::UInt8* dst = pdata + ( ( height - i - 1 ) * stride );
        for ( size_t j = 0; j < stride; j++ )
        {
            std::swap( *src, *dst );
            src++; dst++;
        }
    }
}

}


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Check the file extension.
 *  @param  filename [in] filename
 *  @return true, if the specified file is a Bitmap file format
 */
/*===========================================================================*/
bool Bmp::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "bmp" || file.extension() == "BMP" )
    {
        return true;
    }

    return false;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Bmp class.
 *  @param  width  [in] width
 *  @param  height [in] height
 *  @param  data   [in] pixel data
 */
/*==========================================================================*/
Bmp::Bmp( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels ):
    m_width( width ),
    m_height( height ),
    m_bpp( 3 ),
    m_pixels( pixels )
{
    this->set_header();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Bmp class.
 *  @param  filename [in] filename
 */
/*==========================================================================*/
Bmp::Bmp( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the bitmap image.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Bmp::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    m_file_header.print( os, indent );
    m_info_header.print( os, indent );
}

/*==========================================================================*/
/**
 *  @brief  Reads the bitmap format image.
 *  @param  filename [in] filename
 *  @return true, if the read process is done successfully
 */
/*==========================================================================*/
bool Bmp::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Open the file.
    std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );
    if( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    // Read header information.
    m_file_header.read( ifs );
    m_info_header.read( ifs );

    // Chech whether this file is supported or not.
    if ( !( m_info_header.bpp() == 24 || m_info_header.bpp() == 32 ) )
    {
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    this->skip_header_and_pallete( ifs );

    m_width = m_info_header.width();
    m_height = kvs::Math::Abs( m_info_header.height() );

    // NOTE: The pixel data is stored as 24-bit color data in kvs::Bmp
    // without depending on m_info_header.bpp().
    m_bpp = 24;

    // NOTE: 24-bit color data (R, G, and B channels) only supported.
    const size_t nchannels = 3;
    m_pixels.allocate( m_width * m_height * nchannels );

    kvs::UInt8* data = m_pixels.data();
    const size_t bpl = m_width * ( m_bpp / 8 );
    const size_t padding = ( m_width * ( m_info_header.bpp() / 8 ) ) % 4;
    const size_t upper_left = ( m_height - 1 ) * bpl;
    const size_t received = m_info_header.bpp() == 24 ? 0 : 1;
    for ( size_t j = 0; j < m_height; j++ )
    {
        // The origin of BMP is a lower-left point.
        const size_t line_index = upper_left - j * bpl;
        for ( size_t i = 0; i < m_width; i++ )
        {
            // BGR -> RGB
            const size_t index = line_index + 3 * i;
            ifs.read( (char*)( data + index + 2 ), 1 );
            ifs.read( (char*)( data + index + 1 ), 1 );
            ifs.read( (char*)( data + index + 0 ), 1 );
            ifs.seekg( received, std::ios::cur ); // ignored
        }

        // Padding. (4-byte alignment)
        ifs.seekg( padding, std::ios::cur );
    }

    if ( m_info_header.height() < 0 )
    {
        ::Flip( m_width, m_height, data );
    }

    ifs.close();

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Writes the bitmap format image.
 *  @param  filename [in] filename
 *  @return true, if the write process is done successfully
 */
/*==========================================================================*/
bool Bmp::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    std::ofstream ofs( filename.c_str(), std::ios::out | std::ios::trunc | std::ios::binary );
    if( !ofs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", BaseClass::filename().c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    this->set_header();

    m_file_header.write( ofs );
    m_info_header.write( ofs );

    kvs::UInt8* data = m_pixels.data();

    const size_t bpp = 3;
    const size_t bpl = m_width * bpp;
    const size_t padding = m_width % 4;
    const size_t lower_left = ( m_height - 1 ) * bpl;
    for ( size_t j = 0; j < m_height; j++ )
    {
        // The origin of BMP is a lower-left point.
        const size_t line_index = lower_left - j * bpl;
        for ( size_t i = 0; i < m_width; i++ )
        {
            // RGB -> BGR
            const size_t index = line_index + 3 * i;
            ofs.write( reinterpret_cast<char*>( data + index + 2 ), 1 );
            ofs.write( reinterpret_cast<char*>( data + index + 1 ), 1 );
            ofs.write( reinterpret_cast<char*>( data + index + 0 ), 1 );
        }

        // Padding.
        for ( size_t i = 0; i < padding; i++ ) ofs.write( "0", 1 );
    }

    ofs.close();

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Skips the header and pallete region.
 *  @param  ifs [in] input file stream
 */
/*==========================================================================*/
void Bmp::skip_header_and_pallete( std::ifstream& ifs )
{
    ifs.clear();
    ifs.seekg( m_file_header.offset() + m_info_header.colsused() * 4, std::ios::beg );
}

/*==========================================================================*/
/**
 *  @brief  Sets the bitmap image header information.
 */
/*==========================================================================*/
void Bmp::set_header()
{
    const char*  magic_num = "BM";
    const kvs::UInt32 offset = 54;
    const kvs::UInt32 bpp = 3;
    const kvs::UInt32 padding = kvs::UInt32( m_height * ( m_width % 4 ) );

    //m_fileh.type = 0x4d42; // "BM"; '0x424d', if big endian.
    //m_fileh.type = 0x424d; // "BM"; '0x424d', if big endian.
    memcpy( &( m_file_header.m_type ), magic_num, sizeof( kvs::UInt16 ) );
    m_file_header.m_size = kvs::UInt32( offset + m_width * m_height * bpp + padding );
    m_file_header.m_reserved1 = 0;
    m_file_header.m_reserved2 = 0;
    m_file_header.m_offset = offset;

    m_info_header.m_size = 40;
    m_info_header.m_width = kvs::Int32( m_width );
    m_info_header.m_height = kvs::Int32( m_height );
    m_info_header.m_nplanes = 1;
    m_info_header.m_bpp = 24;
    m_info_header.m_compression = 0L; // 0L: no compress,
                                        // 1L: 8-bit run-length encoding, 2L: 4-bit
    m_info_header.m_bitmapsize = kvs::UInt32( m_width * m_height * bpp + padding );
    m_info_header.m_hresolution = 0;
    m_info_header.m_vresolution = 0;
    m_info_header.m_colsused = 0;
    m_info_header.m_colsimportant = 0;
}

} // end of namespace kvs
