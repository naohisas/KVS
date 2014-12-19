/****************************************************************************/
/**
 *  @file Ppm.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Ppm.cpp 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Ppm.h"
#include <string>
#include <iostream>
#include <fstream>
#include <kvs/File>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Checks the file extension.
 *  @param  filename [in] filename
 *  @return true if the file is a PPM format.
 */
/*===========================================================================*/
bool Ppm::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "ppm" || file.extension() == "PPM" )
    {
        return true;
    }

    return false;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Ppm class.
 */
/*==========================================================================*/
Ppm::Ppm()
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Ppm class.
 *  @param  width  [in] width
 *  @param  height [in] height
 *  @param  pixels [in] pixel data
 */
/*==========================================================================*/
Ppm::Ppm( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels ):
    m_width( width ),
    m_height( height ),
    m_pixels( pixels )
{
    this->set_header();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Ppm class.
 *  @param filename [in] PPM image filename
 */
/*==========================================================================*/
Ppm::Ppm( const std::string& filename )
{
    this->read( filename );
}

/*==========================================================================*/
/**
 *  @brief  Returns the header information.
 *  @return header information
 */
/*==========================================================================*/
const Ppm::Header& Ppm::header() const
{
    return m_header;
}

/*==========================================================================*/
/**
 *  @brief  Returns the image width.
 *  @return image width
 */
/*==========================================================================*/
size_t Ppm::width() const
{
    return m_width;
}

/*==========================================================================*/
/**
 *  @brief  Returns the image height.
 *  @return image height
 */
/*==========================================================================*/
size_t Ppm::height() const
{
    return m_height;
}

/*==========================================================================*/
/**
 *  @brief  Returns the pixel data.
 *  @return pixel data
 */
/*==========================================================================*/
const kvs::ValueArray<kvs::UInt8>& Ppm::pixels() const
{
    return m_pixels;
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the PBM image.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Ppm::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    m_header.print( os, indent );
}

/*==========================================================================*/
/**
 *  @brief  Reads PPM image.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*==========================================================================*/
bool Ppm::read( const std::string& filename )
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
    m_header.read( ifs );

    // Get the image information.
    m_width = m_header.width();
    m_height = m_header.height();

    // Allocate the pixel data.
    const size_t npixels = m_width * m_height;
    m_pixels.allocate( npixels * 3 );

    // Ascii data.
    if ( m_header.isP3() )
    {
        for ( size_t i = 0, i3 = 0; i < npixels; i++, i3 += 3 )
        {
            size_t r, g, b;
            ifs >> r >> g >> b;

            m_pixels[ i3 + 0 ] = static_cast<kvs::UInt8>( r );
            m_pixels[ i3 + 1 ] = static_cast<kvs::UInt8>( g );
            m_pixels[ i3 + 2 ] = static_cast<kvs::UInt8>( b );
        }
    }
    // Binary data.
    else if ( m_header.isP6() )
    {
        ifs.read( reinterpret_cast<char*>( m_pixels.data() ), m_header.size() );
    }
    else
    {
        kvsMessageError( "%s is not PPM format.", filename.c_str() );
        ifs.close();

        BaseClass::setSuccess( false );
        return false;
    }

    ifs.close();

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Writes PPM image.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*==========================================================================*/
bool Ppm::write( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Open the file.
    std::ofstream ofs( filename.c_str(), std::ios::binary | std::ios::out | std::ios::trunc );
    if( !ofs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        BaseClass::setSuccess( false );
        return false;
    }

    // Write header information.
    this->set_header();
    m_header.write( ofs );

    ofs.write( reinterpret_cast<char*>( m_pixels.data() ), m_header.size() );
    ofs.close();

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Sets header information.
 */
/*===========================================================================*/
void Ppm::set_header()
{
    const std::string format = "P6";
    m_header.set( format, m_width, m_height );
}

} // end of namespace kvs
