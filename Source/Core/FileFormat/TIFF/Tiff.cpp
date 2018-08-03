/****************************************************************************/
/**
 *  @file   Tiff.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Tiff.cpp 1313 2012-09-19 08:19:01Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "Tiff.h"
#include "ValueType.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/File>
#include <algorithm>


namespace kvs
{

bool Tiff::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "tiff" || file.extension() == "TIFF" ||
         file.extension() == "tif"  || file.extension() == "TIF" )
    {
        return true;
    }

    return false;
}

Tiff::Tiff()
{
}

Tiff::Tiff( const std::string& filename ):
    m_width( 0 ),
    m_height( 0 ),
    m_bits_per_sample( 0 ),
    m_color_mode( Tiff::UnknownColorMode )
{
    this->read( filename );
}

Tiff::~Tiff()
{
    m_raw_data.release();
}

bool Tiff::isSupported() const
{
    bool ret = true;

    if ( this->get_compression_mode() != 1 )
    {
        kvsMessageError("Not supported compressed TIFF image.");
        ret = false;
    }

    return ret;
}

void Tiff::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Filename : " << BaseClass::filename() << std::endl;
    os << indent << "Width : " << m_width << std::endl;
    os << indent << "Height : " << m_height << std::endl;
    os << indent << "Bits per sample : " << m_bits_per_sample << std::endl;

    m_header.print( os, indent );
    m_ifd.print( os, indent );
}

bool Tiff::read( const std::string& filename )
{
    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    // Open the file.
    std::ifstream ifs( filename.c_str(), std::ios::binary | std::ios::in );
    if( !ifs.is_open() )
    {
        kvsMessageError( "Cannot open %s.", filename.c_str() );
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    // Read header information.
    if ( !m_header.read( ifs ) )
    {
        kvsMessageError( "Cannot read header." );
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    if ( !m_ifd.read( ifs ) )
    {
        kvsMessageError( "Cannot read IFD." );
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    // Chech whether this file is supported or not.
    if ( !this->isSupported() )
    {
        ifs.close();
        BaseClass::setSuccess( false );
        return false;
    }

    // Get the image information.
    m_width = this->get_width();
    m_height = this->get_height();
    m_bits_per_sample = this->get_bits_per_sample();
    m_color_mode = this->get_color_mode();
    m_raw_data = this->get_raw_data( ifs );

    ifs.close();

    return true;
}

bool Tiff::write( const std::string& filename )
{
    kvs::IgnoreUnusedVariable( filename );
    return true;
}

size_t Tiff::get_width() const
{
    // Image width in pixels.
    const kvs::UInt16 TIFF_IMAGE_WIDTH = 256;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_IMAGE_WIDTH ) );

    // No default.
    if ( entry == m_ifd.entryList().end() )
    {
        kvsMessageError("Cannot find the image width in 0th-IFD.");
        return 0;
    }

    size_t ret = 0;
    switch( entry->type() )
    {
    case kvs::tiff::Short:
        ret = entry->values().asValueArray<kvs::UInt16>()[0]; // kvs::tiff::Short
        break;
    case kvs::tiff::Long:
        ret = entry->values().asValueArray<kvs::UInt32>()[0]; // kvs::tiff::Long
        break;
    default:
        break;
    }

    return ret;
}

size_t Tiff::get_height() const
{
    // Image height in pixels.
    const kvs::UInt16 TIFF_IMAGE_HEIGHT = 257;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_IMAGE_HEIGHT ) );

    // No default.
    if ( entry == m_ifd.entryList().end() )
    {
        kvsMessageError("Cannot find the image height in 0th-IFD.");
        return 0;
    }

    size_t ret = 0;
    switch( entry->type() )
    {
    case kvs::tiff::Short:
        ret = entry->values().asValueArray<kvs::UInt16>()[0]; // kvs::tiff::Short
        break;
    case kvs::tiff::Long:
        ret = entry->values().asValueArray<kvs::UInt32>()[0]; // kvs::tiff::Long
        break;
    default:
        break;
    }

    return ret;
}

size_t Tiff::get_bits_per_sample() const
{
    // Bits per channel (sample).
    const kvs::UInt16 TIFF_BITSPERSAMPLE = 258;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_BITSPERSAMPLE ) );

    // Default = 1.
    if ( entry == m_ifd.entryList().end() )
    {
        const size_t default_value = 1;
        return default_value;
    }

    kvs::ValueArray<kvs::UInt16> values = entry->values().asValueArray<kvs::UInt16>();
    size_t ret = 0;
    for ( size_t i = 0; i < entry->count(); i++ )
    {
        ret += values[i]; // kvs::tiff::Short
    }

    return ret;
}

size_t Tiff::get_samples_per_pixel() const
{
    // Samples per pixel
    const kvs::UInt16 TIFF_SAMPLESPERPIXEL = 277;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_SAMPLESPERPIXEL ) );

    // Default = 1.
    if ( entry == m_ifd.entryList().end() )
    {
        const size_t default_value = 1;
        return default_value;
    }

    return entry->values().asValueArray<kvs::UInt16>()[0]; // kvs::tiff::Short
}

size_t Tiff::get_photometirc_interpretation() const
{
    // Photometric interpretation.
    /*   0: min value is white
     *   1: min value is black
     *   2: RGB color model
     *   3: color map indexed
     *   4: holdout mask
     *   5: color separations
     *   6: CCIR 601
     *   8: 1976 CIE L*a*b*
     */
    const kvs::UInt16 TIFF_PHOTOMETRIC = 262;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_PHOTOMETRIC ) );

    // Unknown default value.
    if ( entry == m_ifd.entryList().end() )
    {
        kvsMessageError("Cannot find the photometric interpretation in 0th-IFD.");
        return 0;
    }

    return entry->values().asValueArray<kvs::UInt16>()[0]; // kvs::tiff::Short
}

size_t Tiff::get_rows_per_strip() const
{
    // Rows per strip of data.
    const kvs::UInt16 TIFF_ROWSPERSTRIP = 278;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_ROWSPERSTRIP ) );

    // Default = 2**32 - 1.
    if ( entry == m_ifd.entryList().end() )
    {
        const size_t default_value = ( kvs::UInt64( 1 ) << 32 ) - 1;
        return default_value;
    }

    size_t ret = 0;
    switch( entry->type() )
    {
    case kvs::tiff::Short:
        ret = entry->values().asValueArray<kvs::UInt16>()[0]; // kvs::tiff::Short
        break;
    case kvs::tiff::Long:
        ret = entry->values().asValueArray<kvs::UInt32>()[0]; // kvs::tiff::Long
        break;
    default:
        break;
    }

    return ret;
}

size_t Tiff::get_compression_mode() const
{
    // Data compression technique.
    /*       1: none compression
     *       2: CCITT modified Huffman RLE
     *       3: CCITT Group 3 fax encoding
     *       4: CCITT Group 4 fax encoding
     *       5: LZW
     *       6: 6.0 JPEG
     *       7: JPEG DCT compression
     *   32766: NeXT 2-bit RLE
     *   32771: #1 w/ word alignment
     *   32773: Macintosh RLE
     *   32809: ThunderScan RLE
     *   32908: Pixar companded 10bit LZW
     *   32909: Pixar companded 11bit ZIP
     *   32946: Deflate compression
     *   32947: Kodak DCS encoding
     *   34661: ISO JBIG
     */
    const kvs::UInt16 TIFF_COMPRESSION = 259;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_COMPRESSION ) );

    // Default = 1 (non compression).
    if ( entry == m_ifd.entryList().end() )
    {
        const size_t default_value = 1;
        return default_value;
    }

    return entry->values().asValueArray<kvs::UInt16>()[0]; // kvs::tiff::Short
}

kvs::AnyValueArray Tiff::get_strip_offsets() const
{
    // Offsets to the data strips.
    const kvs::UInt16 TIFF_STRIPOFFSETS = 273;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_STRIPOFFSETS ) );

    // No default.
    if ( entry == m_ifd.entryList().end() )
    {
        kvsMessageError("Cannot find the strip offsets in 0th-IFD.");
    }

    return entry->values();
}

kvs::AnyValueArray Tiff::get_strip_bytes() const
{
    // Bytes counts for strips.
    const kvs::UInt16 TIFF_STRIPBYTECOUNT = 279;

    kvs::tiff::ImageFileDirectory::EntryList::const_iterator entry;
    entry = std::find( m_ifd.entryList().begin(),
                       m_ifd.entryList().end(),
                       kvs::tiff::Entry( TIFF_STRIPBYTECOUNT ) );

    // No default.
    if ( entry == m_ifd.entryList().end() )
    {
        kvsMessageError("Cannot find the strip bytes counts in 0th-IFD.");
    }

    return entry->values();
}

Tiff::ColorMode Tiff::get_color_mode() const
{
    Tiff::ColorMode color_mode = Tiff::UnknownColorMode;

    const size_t bps = ( m_bits_per_sample > 0 ) ? m_bits_per_sample : this->get_bits_per_sample();
    const size_t spp = this->get_samples_per_pixel();
    if ( spp == 1 )
    {
        switch ( bps )
        {
        case 8:  color_mode = Tiff::Gray8;  break;
        case 16: color_mode = Tiff::Gray16; break;
        default: color_mode = Tiff::UnknownColorMode; break;
        }
    }
    else if ( spp == 3 )
    {
        switch ( bps )
        {
        case 24: color_mode = Tiff::Color24;  break;
        default: color_mode = Tiff::UnknownColorMode; break;
        }
    }

    size_t photometric_interpretation = this->get_photometirc_interpretation();
    switch ( photometric_interpretation )
    {
    case 0:
    case 1:
        if ( !( color_mode == Tiff::Gray8 || color_mode == Tiff::Gray16 ) )
        {
            kvsMessageError("Incorrect the photometric interpretation.");
            color_mode = Tiff::UnknownColorMode;
        }
        break;
    case 2:  color_mode = Tiff::Color24;  break;
    default: color_mode = Tiff::UnknownColorMode; break;
    }

    return color_mode;
}

kvs::AnyValueArray Tiff::get_raw_data( std::ifstream& ifs ) const
{
    kvs::AnyValueArray raw_data;

    const kvs::AnyValueArray offsets = this->get_strip_offsets();
    const kvs::AnyValueArray bytes   = this->get_strip_bytes();
    const size_t             count   = offsets.size();

    kvs::ValueArray<kvs::UInt32> offset = offsets.asValueArray<kvs::UInt32>();
    kvs::ValueArray<kvs::UInt32> byte = bytes.asValueArray<kvs::UInt32>();

    if ( m_color_mode == Tiff::Gray8 )
    {
        raw_data.allocate<kvs::UInt8>( m_width * m_height );
        kvs::UInt8* data = reinterpret_cast<kvs::UInt8*>( raw_data.data() );
        for ( size_t i = 0; i < count; i++ )
        {
            ifs.seekg( offset[i], std::ios::beg );
            ifs.read( reinterpret_cast<char*>( data ), byte[i] );
            data += byte[i] / sizeof( kvs::UInt8 );
        }
    }

    else if ( m_color_mode == Tiff::Gray16 )
    {
        raw_data.allocate<kvs::UInt16>( m_width * m_height );
        kvs::UInt16* data = reinterpret_cast<kvs::UInt16*>( raw_data.data() );
        for ( size_t i = 0; i < count; i++ )
        {
            ifs.seekg( offset[i], std::ios::beg );
            ifs.read( reinterpret_cast<char*>( data ), byte[i] );
            data += byte[i] / sizeof( kvs::UInt16 );
        }
    }

    else if ( m_color_mode == Tiff::Color24 )
    {
        raw_data.allocate<kvs::UInt8>( m_width * m_height * 3 );
        kvs::UInt8* data = reinterpret_cast<kvs::UInt8*>( raw_data.data() );
        for ( size_t i = 0; i < count; i++ )
        {
            ifs.seekg( offset[i], std::ios::beg );
            ifs.read( reinterpret_cast<char*>( data ), byte[i] );
            data += byte[i] / sizeof( kvs::UInt8 );
        }
    }

    return raw_data;
}

} // end of namespace kvs
