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

inline kvs::UInt8 Lerp(
    const kvs::UInt8 p1,
    const kvs::UInt8 p2,
    const kvs::UInt8 p3,
    const kvs::UInt8 p4,
    const double xrate,
    const double yrate )
{
    const auto x0 = kvs::Math::Mix( static_cast<double>(p1), static_cast<double>(p3), xrate );
    const auto x1 = kvs::Math::Mix( static_cast<double>(p2), static_cast<double>(p4), xrate );
    auto y = kvs::Math::Round( kvs::Math::Mix( x0, x1, yrate ) );
    kvs::Math::Clamp( y, 0, 255 );
    return static_cast<kvs::UInt8>( y );
}

} // end of namespace


namespace kvs
{

kvs::UInt8 ImageBase::GrayNearest( double u, double v, const GrayImage& image )
{
    const auto x = static_cast<size_t>( kvs::Math::Floor( u ) );
    const auto y = static_cast<size_t>( kvs::Math::Floor( v ) );
    return image.pixel( x, y );
}

kvs::UInt8 ImageBase::GrayBilinear( double u, double v, const GrayImage& image )
{
    const auto w = image.width() - 1;
    const auto h = image.height() - 1;
    const auto x0 = static_cast<size_t>( kvs::Math::Floor( u ) );
    const auto y0 = static_cast<size_t>( kvs::Math::Floor( v ) );
    const auto x1 = x0 + ( w > x0 ? 1 : 0 );
    const auto y1 = y0 + ( h > y0 ? 1 : 0 );

    const auto p1 = image.pixel( x0, y0 );
    const auto p2 = image.pixel( x0, y1 );
    const auto p3 = image.pixel( x1, y0 );
    const auto p4 = image.pixel( x1, y1 );

    const auto xrate = u - static_cast<double>( x0 );
    const auto yrate = v - static_cast<double>( y0 );
    return ::Lerp( p1, p2, p3, p4, xrate, yrate );
}

kvs::RGBColor ImageBase::ColorNearest( double u, double v, const ColorImage& image )
{
    const auto x = static_cast<size_t>( kvs::Math::Floor( u ) );
    const auto y = static_cast<size_t>( kvs::Math::Floor( v ) );
    return image.pixel( x, y );
}

kvs::RGBColor ImageBase::ColorBilinear( double u, double v, const ColorImage& image )
{
    const auto w = image.width() - 1;
    const auto h = image.height() - 1;
    const auto x0 = static_cast<size_t>( kvs::Math::Floor( u ) );
    const auto y0 = static_cast<size_t>( kvs::Math::Floor( v ) );
    const auto x1 = x0 + ( w > x0 ? 1 : 0 );
    const auto y1 = y0 + ( h > y0 ? 1 : 0 );

    const auto p1 = image.pixel( x0, y0 );
    const auto p2 = image.pixel( x0, y1 );
    const auto p3 = image.pixel( x1, y0 );
    const auto p4 = image.pixel( x1, y1 );

    const auto xrate = u - static_cast<double>( x0 );
    const auto yrate = v - static_cast<double>( y0 );
    return {
        ::Lerp( p1.r(), p2.r(), p3.r(), p4.r(), xrate, yrate ),
        ::Lerp( p1.g(), p2.g(), p3.g(), p4.g(), xrate, yrate ),
        ::Lerp( p1.b(), p2.b(), p3.b(), p4.b(), xrate, yrate )
    };
}

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

template <typename Image, typename Interpolator>
void ImageBase::resizeImage(
    const size_t width,
    const size_t height,
    Image* image,
    Interpolator interpolator )
{
    Image resized_image( width, height );

    const double ratio_width  = m_width / static_cast<double>( width );
    const double ratio_height = m_height / static_cast<double>( height );
    for ( size_t j = 0; j < height; j++ )
    {
        const double v = j * ratio_height;
        for ( size_t i = 0; i < width; i++ )
        {
            const double u = i * ratio_width;
            const auto pixel = interpolator( u, v, *image );
            resized_image.setPixel( i, j, pixel );
        }
    }

    *image = resized_image;
}

template void ImageBase::resizeImage<kvs::GrayImage,ImageBase::GrayInterpolator>(
    const size_t, const size_t, kvs::GrayImage*, GrayInterpolator );

template void ImageBase::resizeImage<kvs::ColorImage,ImageBase::ColorInterpolator>(
    const size_t, const size_t, kvs::ColorImage*, ColorInterpolator );

} // end of namespace kvs
