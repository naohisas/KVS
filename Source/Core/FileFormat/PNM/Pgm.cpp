/****************************************************************************/
/**
 *  @file Pgm.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Pgm.cpp 1402 2012-12-07 01:32:08Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Pgm.h"
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
 *  @return true if the file is a PGM format.
 */
/*===========================================================================*/
bool Pgm::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "pgm" || file.extension() == "PGM" )
    {
        return true;
    }

    return false;
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Pgm class.
 */
/*==========================================================================*/
Pgm::Pgm( void )
{
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Pgm class.
 *  @param  width  [in] width
 *  @param  height [in] height
 *  @param  pixels [in] pixel data
 */
/*==========================================================================*/
Pgm::Pgm( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels ):
    m_width( width ),
    m_height( height ),
    m_pixels( pixels )
{
    this->set_header();
}

/*==========================================================================*/
/**
 *  @brief  Constructs a new Pgm class.
 *  @param  filename [in] PGM image filename
 */
/*==========================================================================*/
Pgm::Pgm( const std::string& filename )
{
    this->read( filename );
}

/*==========================================================================*/
/**
 *  @brief  Returns the header information.
 *  @return header information
 */
/*==========================================================================*/
const Pgm::Header& Pgm::header() const
{
    return m_header;
}

/*==========================================================================*/
/**
 *  @brief  Returns the image width.
 *  @return image width
 */
/*==========================================================================*/
size_t Pgm::width() const
{
    return m_width;
}

/*==========================================================================*/
/**
 *  @brief  Returns the image height.
 *  @return image height
 */
/*==========================================================================*/
size_t Pgm::height() const
{
    return m_height;
}

/*==========================================================================*/
/**
 *  @brief  Returns the pixel data.
 *  @return pixel data
 */
/*==========================================================================*/
const kvs::ValueArray<kvs::UInt8>& Pgm::pixels() const
{
    return m_pixels;
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the PGM image.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Pgm::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    m_header.print( os, indent );
}

/*==========================================================================*/
/**
 *  @brief  Reads PGM image.
 *  @param  filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*==========================================================================*/
bool Pgm::read( const std::string& filename )
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
    m_pixels.allocate( npixels );

    // Ascii data.
    if ( m_header.isP2() )
    {
        for ( size_t i = 0; i < npixels; i++ )
        {
            size_t v;
            ifs >> v;

            m_pixels[i] = static_cast<kvs::UInt8>( v );
        }
    }
    // Binary data.
    else if ( m_header.isP5() )
    {
        ifs.read( reinterpret_cast<char*>( m_pixels.data() ), m_header.size() );
    }
    else
    {
        kvsMessageError( "%s is not PGM format.", filename.c_str() );
        ifs.close();

        BaseClass::setSuccess( false );
        return false;
    }

    ifs.close();

    return true;
}

/*==========================================================================*/
/**
 *  @brief  Writes PGM image.
 *  @param  filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*==========================================================================*/
bool Pgm::write( const std::string& filename )
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
void Pgm::set_header()
{
    const std::string format = "P5";
    m_header.set( format, m_width, m_height );
}

} // end of namespace kvs
