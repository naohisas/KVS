/****************************************************************************/
/**
 *  @file   ImageBase.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ImageBase.h"
#include "ColorImage.h"
#include "GrayImage.h"
#include "RGBColor.h"
#include <kvs/Type>
#include <utility>


namespace
{

/*==========================================================================*/
/**
 *  Convert 'bit' to 'byte'.
 *  @param value [in] bit value
 *  @return converted byte value from bit value
 */
/*==========================================================================*/
inline size_t BitToByte( size_t value )
{
    return value >> 3;
}

/*==========================================================================*/
/**
 *  Convert 'byte' to 'bit'.
 *  @param value [in] byte value
 *  @return converted bit value from byte value
 */
/*==========================================================================*/
inline size_t ByteToBit( size_t value )
{
    return value << 3;
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Flip the image data.
 */
/*===========================================================================*/
void ImageBase::flip()
{
    const size_t stride = m_width * ::BitToByte( m_bpp );

    kvs::UInt8* pdata = m_pixels.data();
    const size_t end_line = m_height / 2;
    for ( size_t i = 0; i < end_line; i++ )
    {
        kvs::UInt8* src = pdata + ( i * stride );
        kvs::UInt8* dst = pdata + ( ( m_height - i - 1 ) * stride );
        for ( size_t j = 0; j < stride; j++ )
        {
            std::swap( *src, *dst );
            src++; dst++;
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Copy the specified image data.
 *  @param  image [in] image data
 */
/*===========================================================================*/
void ImageBase::copy( const ImageBase& image )
{
    *this = image;
}

/*==========================================================================*/
/**
 *  Create the image.
 *  @param width [in] image width
 *  @param height [in] image height
 *  @param type [in] pixel type
 *  @return true, if the create process is done successfully
 */
/*==========================================================================*/
bool ImageBase::create( const size_t width, const size_t height, const ImageType type )
{
    m_width = width;
    m_height = height;
    m_npixels = width * height;

    if ( type == Bit )
    {
        m_padding = ::ByteToBit( ::BitToByte( width + 7 ) ) - width;
        m_bpp = 1;
        m_bpl = ::BitToByte( width + 7 );
    }
    else
    {
        const size_t ncomponents = static_cast<size_t>( type );
        m_padding = 0;
        m_bpp = ::ByteToBit( sizeof(kvs::UInt8) ) * ncomponents;
        m_bpl = width * ::BitToByte( m_bpp );
    }

    m_size = height * m_bpl;
    m_pixels.allocate( m_size );
    m_pixels.fill( 0 );
    return true;
}

/*==========================================================================*/
/**
 *  Create the image.
 *  @param width [in] image width
 *  @param height [in] image height
 *  @param type [in] pixel type
 *  @param pixels [in] pixel data
 *  @return true, if the create process is done successfully
 */
/*==========================================================================*/
bool ImageBase::create(
    const size_t width,
    const size_t height,
    const ImageType type,
    const kvs::ValueArray<kvs::UInt8>& pixels )
{
    m_width = width;
    m_height = height;
    m_npixels = width * height;

    if ( type == Bit )
    {
        m_padding = ::ByteToBit( ::BitToByte( width + 7 ) ) - width;
        m_bpp = 1;
        m_bpl = ::BitToByte( width + 7 );
    }
    else
    {
        const size_t ncomponents = static_cast<size_t>( type );
        m_padding = 0;
        m_bpp = ::ByteToBit( sizeof( kvs::UInt8 ) ) * ncomponents;
        m_bpl = width * ::BitToByte( m_bpp );
    }

    m_size = height * m_bpl;
    if ( m_size != pixels.size() ) { return false; }

    m_pixels = pixels;

    return true;
}

template <typename ImageDataType, typename Interpolator>
void ImageBase::resizeImage( const size_t width, const size_t height, ImageDataType* image )
{
    // Resized image.
    ImageDataType resized_image( width, height );

    // Interpolator.
    Interpolator interpolator;
    interpolator.attach( image );

    const double ratio_width  = m_width / static_cast<double>( width );
    const double ratio_height = m_height / static_cast<double>( height );
    for ( size_t j = 0; j < height; j++ )
    {
        const double v = j * ratio_height;
        interpolator.setV( v );

        for ( size_t i = 0; i < width; i++ )
        {
            const double u = i * ratio_width;
            interpolator.setU( u );

            typename ImageDataType::PixelType pixel = interpolator();
            resized_image.setPixel( i, j, pixel );
        }
    }

    *image = resized_image;
}

// Specialization.
template class ImageBase::NearestNeighborInterpolator<kvs::GrayImage>;
template class ImageBase::NearestNeighborInterpolator<kvs::ColorImage>;

template class ImageBase::BilinearInterpolator<kvs::GrayImage>;
template class ImageBase::BilinearInterpolator<kvs::ColorImage>;

} // end of namespace kvs
