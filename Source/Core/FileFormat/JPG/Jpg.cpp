/*****************************************************************************/
/**
 *  @file   Jpg.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "Jpg.h"
#include <kvs/Message>
#include <kvs/Assert>
#include <kvs/File>
#include <kvs/SharedPointer>
#include <kvs/ValueArray>
#include "../../NanoVG/stb_image.h"
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
 *  @brief  Check extension of JPG image file.
 *  @param  filename [in] filename
 *  @return true if the specified file has an extension of the JPG image
 */
/*===========================================================================*/
bool Jpg::CheckExtension( const std::string& filename )
{
    const kvs::File file( filename );
    if ( file.extension() == "jpg" || file.extension() == "JPG" ||
         file.extension() == "jpeg" || file.extension() == "JPEG" )
    {
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new JPG image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] pixel data
 */
/*===========================================================================*/
Jpg::Jpg(
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
 *  @brief  Constructs a new JPG image from specified file.
 *  @param  filename [in] filename of JPG image data
 */
/*===========================================================================*/
Jpg::Jpg( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Outputs JPG image information.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void Jpg::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Width: " << m_width << std::endl;
    os << indent << "Height: " << m_height << std::endl;
    os << indent << "Bytes per pixel: " << m_bpp << std::endl;
    os << indent << "Quality: " << m_quality << std::endl;
}

/*===========================================================================*/
/**
 *  @brief  Reads JPG image file.
 *  @param  filename [in] filename of JPG image data
 *  @return true if the reading process is done successfully
 */
/*===========================================================================*/
bool Jpg::read( const std::string& filename )
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
 *  @param  filename [in] filename of JPG image data
 *  @return true if the writing process is done successfully
 */
/*===========================================================================*/
bool Jpg::write( const std::string& filename )
{
    KVS_ASSERT( m_width > 0 && m_height > 0 );
    KVS_ASSERT( m_pixels.size() > 0 );

    BaseClass::setFilename( filename );
    BaseClass::setSuccess( true );

    const int width = static_cast<int>( m_width );
    const int height = static_cast<int>( m_height );
    const int bpp = m_bpp;
    const void* pixels = m_pixels.data();
    auto ret = stbi_write_png( filename.c_str(), width, height, bpp, pixels, m_quality );
    if ( !ret )
    {
        kvsMessageError() << std::string( stbi_failure_reason() ) << std::endl;
        BaseClass::setSuccess( false );
        return false;
    }

    return true;
}

} // end of namespace kvs
