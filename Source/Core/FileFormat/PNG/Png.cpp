/*****************************************************************************/
/**
 *  @file   Png.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Png.h"
#include <kvs/Message>
#include <kvs/Assert>
#include <kvs/File>
#include <kvs/SharedPointer>
#include <kvs/ValueArray>

//#define STB_IMAGE_IMPLEMENTATION
#include "../../NanoVG/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../NanoVG/stb_image_write.h"


namespace
{

/*===========================================================================*/
/**
 *  @brief  Deleter class for stbi image data.
 */
/*===========================================================================*/
class Deleter
{
public:
    void operator ()( kvs::UInt8* p )
    {
        stbi_image_free( p );
    }
};

}

namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Check extension of PNG image file.
 *  @param  filename [in] filename
 *  @return true if the specified file has an extension of the PNG image
 */
/*===========================================================================*/
bool Png::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "png" || file.extension() == "PNG" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PNG image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] pixel data
 */
/*===========================================================================*/
Png::Png(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::UInt8>& pixels ):
    m_width( width ),
    m_height( height ),
    m_bpp( pixels.byteSize() / ( width * height ) ),
    m_pixels( pixels )
{
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new PNG image from specified file.
 *  @param  filename [in] filename of PNG image data
 */
/*===========================================================================*/
Png::Png( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Outputs PNG image information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Png::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Width: " << m_width << std::endl;
    os << indent << "Height: " << m_height << std::endl;
    os << indent << "Bytes per pixel: " << m_bpp << std::endl;
    os << indent << "Compression level: " << m_compression_level << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Reads PNG image file.
 *  @param  filename [in] filename of PNG image data
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool Png::read( const std::string& filename )
{
    KVS_ASSERT( ( 0 <= m_bpp ) && ( m_bpp <= 4 ) );

    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    int width = 0;
    int height = 0;
    int bpp = 0;
    auto* pixels = stbi_load( filename.c_str(), &width, &height, &bpp, m_bpp );
    if ( !pixels )
    {
        kvsMessageError() << std::string( stbi_failure_reason() ) << std::endl;
        BaseClass::setSuccess( false );
        return false;
    }

    m_width = static_cast<size_t>( width );
    m_height = static_cast<size_t>( height );
    m_bpp = bpp;

    m_pixels = kvs::ValueArray<kvs::UInt8>(
        kvs::SharedPointer<kvs::UInt8>( pixels, ::Deleter() ),
        m_width * m_height * m_bpp );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Writes the image data to file.
 *  @param  filename [in] filename of PNG image data
 *  @return true if the writing process is done successfully
 */
/*===========================================================================*/
bool Png::write( const std::string& filename )
{
    KVS_ASSERT( m_width > 0 && m_height > 0 );
    KVS_ASSERT( m_pixels.size() > 0 );

    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    const int width = static_cast<int>( m_width );
    const int height = static_cast<int>( m_height );
    const int bpp = m_bpp;
    const void* pixels = m_pixels.data();
    stbi_write_png_compression_level = m_compression_level;
    auto ret = stbi_write_png( filename.c_str(), width, height, bpp, pixels, width * bpp );
    if ( !ret )
    {
        kvsMessageError() << std::string( stbi_failure_reason() ) << std::endl;
        BaseClass::setSuccess( false );
        return false;
    }

    return true;
}

} // end of namespace kvs
