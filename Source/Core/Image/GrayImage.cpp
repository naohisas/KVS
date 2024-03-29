/****************************************************************************/
/**
 *  @file   GrayImage.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "GrayImage.h"
#include "ColorImage.h"
#include "BitImage.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/Math>
#include <kvs/File>
#include <kvs/KVSMLImageObject>
#include <kvs/Bmp>
#include <kvs/Png>
#include <kvs/Ppm>
#include <kvs/Pbm>
#include <kvs/Pgm>
#include <kvs/Tiff>
#include <kvs/Dicom>
#include <algorithm>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Returns gray-scaling method by the mean-value method.
 */
/*===========================================================================*/
GrayImage::GrayScalingMethod GrayImage::MeanValue()
{
    return [] ( const kvs::ColorImage& image, BaseClass::PixelData& data )
    {
        const size_t width = image.width();
        const size_t height = image.height();
        const kvs::UInt8* image_data = image.pixels().data();
        for( size_t j = 0; j < height; j++ )
        {
            const size_t col_line_index = j * image.bytesPerLine();
            const size_t gry_line_index = j * image.width();
            for( size_t i = 0; i < width; i++ )
            {
                const size_t col_pixel_index = col_line_index + 3 * i;
                const size_t gry_pixel_index = gry_line_index + i;

                unsigned int value = 0;
                value += image_data[ col_pixel_index + 0 ];
                value += image_data[ col_pixel_index + 1 ];
                value += image_data[ col_pixel_index + 2 ];

                data[ gry_pixel_index ] = static_cast<kvs::UInt8>(value / 3);
            }
        }
    };
}

/*===========================================================================*/
/**
 *  @brief  Returns gray-scaling method by the middle-value method.
 */
/*===========================================================================*/
GrayImage::GrayScalingMethod GrayImage::MiddleValue()
{
    return [] ( const kvs::ColorImage& image, BaseClass::PixelData& data )
    {
        const size_t width = image.width();
        const size_t height = image.height();
        const kvs::UInt8* image_data = image.pixels().data();
        for( size_t j = 0; j < height; j++ )
        {
            const size_t col_line_index = j * image.bytesPerLine();
            const size_t gry_line_index = j * image.width();
            for( size_t i = 0; i < width; i++ )
            {
                const size_t col_pixel_index = col_line_index + 3 * i;
                const size_t gry_pixel_index = gry_line_index + i;

                const unsigned int r = image_data[ col_pixel_index + 0 ];
                const unsigned int g = image_data[ col_pixel_index + 1 ];
                const unsigned int b = image_data[ col_pixel_index + 2 ];
                const unsigned int max = kvs::Math::Max( r, g, b );
                const unsigned int min = kvs::Math::Min( r, g, b );
                const unsigned int value = ( max + min ) / 2;

                data[ gry_pixel_index ] = static_cast<kvs::UInt8>(value);
            }
        }
    };
}

/*===========================================================================*/
/**
 *  @brief  Returns gray-scaling by the median-value method.
 */
/*===========================================================================*/
GrayImage::GrayScalingMethod GrayImage::MedianValue()
{
    return [] ( const kvs::ColorImage& image, BaseClass::PixelData& data )
    {
        const size_t width = image.width();
        const size_t height = image.height();
        const kvs::UInt8* image_data = image.pixels().data();
        for( size_t j = 0; j < height; j++ )
        {
            const size_t col_line_index = j * image.bytesPerLine();
            const size_t gry_line_index = j * image.width();
            for( size_t i = 0; i < width; i ++ )
            {
                const size_t col_pixel_index = col_line_index + 3 * i;
                const size_t gry_pixel_index = gry_line_index + i;

                unsigned int pixel[3];
                pixel[0] = image_data[ col_pixel_index + 0 ];
                pixel[1] = image_data[ col_pixel_index + 1 ];
                pixel[2] = image_data[ col_pixel_index + 2 ];
                std::sort( pixel, pixel + 3 );

                data[ gry_pixel_index ] = static_cast<kvs::UInt8>(pixel[1]);
            }
        }
    };
}

/*===========================================================================*/
/**
 *  @brief  Returns gray-scaling by the NTSC weighted mean-value method.
 */
/*===========================================================================*/
GrayImage::GrayScalingMethod GrayImage::NTSCWeightedMeanValue()
{
    return [] ( const kvs::ColorImage& image, BaseClass::PixelData& data )
    {
        const size_t width = image.width();
        const size_t height = image.height();
        const kvs::UInt8* image_data = image.pixels().data();
        for( size_t j = 0; j < height; j++ )
        {
            const size_t col_line_index = j * image.bytesPerLine();
            const size_t gry_line_index = j * image.width();
            for( size_t i = 0; i < width; i++ )
            {
                const size_t col_pixel_index = col_line_index + 3 * i;
                const size_t gry_pixel_index = gry_line_index + i;

                const unsigned int r = image_data[ col_pixel_index + 0 ];
                const unsigned int g = image_data[ col_pixel_index + 1 ];
                const unsigned int b = image_data[ col_pixel_index + 2 ];

                // value = ( 0.298912 * R + 0.586611 * G + 0.114478 * B )
                //       = ( 2 * R + 4 * G + B ) / 7
                const unsigned int value = ( 2 * r + 4 * g + b ) / 7;

                data[ gry_pixel_index ] = static_cast<kvs::UInt8>(value);
            }
        }
    };
}

/*===========================================================================*/
/**
 *  @brief  Returns gray-scaling by the HDTV weighted mean-value method.
 */
/*===========================================================================*/
GrayImage::GrayScalingMethod GrayImage::HDTVWeightedMeanValue()
{
    return [] ( const kvs::ColorImage& image, BaseClass::PixelData& data )
    {
        const double gamma_value = 2.2;
        const size_t width = image.width();
        const size_t height = image.height();
        const kvs::UInt8* image_data = image.pixels().data();
        for( size_t j = 0; j < height; j++ )
        {
            const size_t col_line_index = j * image.bytesPerLine();
            const size_t gry_line_index = j * image.width();
            for( size_t i = 0; i < width; i++ )
            {
                const size_t col_pixel_index = col_line_index + 3 * i;
                const size_t gry_pixel_index = gry_line_index + i;

                const unsigned int r = image_data[ col_pixel_index + 0 ];
                const unsigned int g = image_data[ col_pixel_index + 1 ];
                const unsigned int b = image_data[ col_pixel_index + 2 ];

                const double R = static_cast<double>(r) / 255.0;
                const double G = static_cast<double>(g) / 255.0;
                const double B = static_cast<double>(b) / 255.0;

                const double RR = std::pow( R, gamma_value ) * 0.222015;
                const double GG = std::pow( G, gamma_value ) * 0.706655;
                const double BB = std::pow( B, gamma_value ) * 0.071330;

                const double V = std::pow( ( RR + GG + BB ), ( 1.0 / gamma_value ) );
                const unsigned int value = kvs::Math::Round( V * 255.0 );

                data[ gry_pixel_index ] = static_cast<kvs::UInt8>(value);
            }
        }
    };
}

/*==========================================================================*/
/**
 *  Constructs a new gray-scale image.
 *  @param width [in] image width
 *  @param height [in] image height
 */
/*==========================================================================*/
GrayImage::GrayImage( const size_t width, const size_t height )
{
    this->create( width, height );
}

/*==========================================================================*/
/**
 *  Constructs a new gray-scale image.
 *  @param width [in] image width
 *  @param height [in] image height
 *  @param data [in] pixel data array
 */
/*==========================================================================*/
GrayImage::GrayImage(
    const size_t width,
    const size_t height,
    const PixelData& data )
{
    this->create( width, height, data );
}

/*==========================================================================*/
/**
 *  Constructs a new gray-scale image.
 *  @param width [in] image width
 *  @param height [in] image height
 *  @param data [in] pixel data array
 *  @param channel [in] channel number
 */
/*==========================================================================*/
GrayImage::GrayImage(
    const size_t width,
    const size_t height,
    const PixelData& data,
    const int channel )
{
    this->create( width, height, data, channel );
}

/*==========================================================================*/
/**
 *  Constructs a new gray-scale image.
 *  @param width [in] image width
 *  @param height [in] image height
 *  @param data [in] normalized pixel data array
 */
/*==========================================================================*/
GrayImage::GrayImage(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::Real32>& data )
{
    this->create( width, height, data );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new gray-scale image from the color image.
 *  @param  image [in] color image
 *  @param  channel [in] channel number
 */
/*===========================================================================*/
GrayImage::GrayImage( const kvs::ColorImage& image, const int channel )
{
    this->create( image.width(), image.height(), image.pixels(), channel );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new gray-scale image from the color image.
 *  @param  image [in] color image
 *  @param  method [in] gray scaling method
 */
/*===========================================================================*/
GrayImage::GrayImage( const kvs::ColorImage& image, GrayScalingMethod method )
{
    BaseClass::create( image.width(), image.height(), kvs::ImageBase::Gray );
    method( image, BaseClass::pixelData() );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a copy of other bit image.
 *  @param  image [in] bit image
 */
/*===========================================================================*/
GrayImage::GrayImage( const kvs::BitImage& image )
{
    this->read_image( image );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new GrayImage class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
GrayImage::GrayImage( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Creates a gray image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @return true if the create process is done successfully
 */
/*===========================================================================*/
bool GrayImage::create( const size_t width, const size_t height )
{
    return BaseClass::create( width, height, kvs::ImageBase::Gray );
}

/*===========================================================================*/
/**
 *  @brief  Creates a gray image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] pixel data
 *  @return true if the create process is done successfully
 */
/*===========================================================================*/
bool GrayImage::create(
    const size_t width,
    const size_t height,
    const PixelData& pixels )
{
    return BaseClass::create( width, height, kvs::ImageBase::Gray, pixels );
}

/*===========================================================================*/
/**
 *  @brief  Creates a gray image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] pixel data
 *  @param  channel [in] channel number
 *  @return true if the create process is done successfully
 */
/*===========================================================================*/
bool GrayImage::create(
    const size_t width,
    const size_t height,
    const PixelData& pixels,
    const int channel )
{
    if ( pixels.size() == width * height )
    {
        // gray-scale data
        return this->create( width, height, pixels );
    }
    else
    {
        // color data (stride = 3:rgb, 4:rgba)
        PixelData data( width * height );
        const size_t stride = pixels.size() / ( width * height );
        for ( size_t i = 0; i < data.size(); ++i )
        {
            data[i] = pixels[ stride * i + channel ];
        }
        return BaseClass::create( width, height, kvs::ImageBase::Gray, data );
    }
}

/*===========================================================================*/
/**
 *  @brief  Creates a gray image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] normalized pixel data
 *  @return true if the create process is done successfully
 */
/*===========================================================================*/
bool GrayImage::create(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::Real32>& pixels )
{
    PixelData data( width * height );
    for ( size_t i = 0; i < data.size(); ++i )
    {
        data[i] = static_cast<int>( pixels[i] * 255 );
    }

    return BaseClass::create( width, height, kvs::ImageBase::Gray, data );
}

/*==========================================================================*/
/**
 *  Returns the pixel value.
 *  @param index [in] pixel index
 *  @return pixel value
 */
/*==========================================================================*/
kvs::UInt8 GrayImage::pixel( const size_t index ) const
{
    const kvs::UInt8* pixels = BaseClass::pixels().data();
    return pixels[ index ];
}

/*==========================================================================*/
/**
 *  Returns the pixel value.
 *  @param i [in] pixel index along the horizontal axis
 *  @param j [in] pixel index along the vertical axis
 *  @return pixel value
 */
/*==========================================================================*/
kvs::UInt8 GrayImage::pixel( const size_t i, const size_t j ) const
{
    const kvs::UInt8* pixels = BaseClass::pixels().data();
    return pixels[ BaseClass::width() * j + i ];
}

/*==========================================================================*/
/**
 *  Set the pixel value.
 *  @param index [in] pixel index
 *  @param pixel [in] pixel value
 */
/*==========================================================================*/
void GrayImage::setPixel( const size_t index, const kvs::UInt8 pixel )
{
    kvs::UInt8* pixels = BaseClass::pixelData().data();
    pixels[ index ] = pixel;
}

/*==========================================================================*/
/**
 *  Set the pixel value.
 *  @param i [in] pixel index along the horizontal axis
 *  @param j [in] pixel index along the vertical axis
 *  @param pixel [in] pixel value
 */
/*==========================================================================*/
void GrayImage::setPixel( const size_t i, const size_t j, const kvs::UInt8 pixel )
{
    kvs::UInt8* pixels = BaseClass::pixelData().data();
    pixels[ BaseClass::width() * j + i ] = pixel;
}

/*===========================================================================*/
/**
 *  @brief  Scales the image data.
 *  @param  ratio [in] scaling ratio
 *  @param  interpolator [in] interpolation method
 */
/*===========================================================================*/
void GrayImage::scale( const double ratio, Interpolator interpolator )
{
    const size_t width = static_cast<size_t>( BaseClass::width() * ratio );
    const size_t height = static_cast<size_t>( BaseClass::height() * ratio );
    BaseClass::resizeImage( width, height, this, interpolator );
}

/*===========================================================================*/
/**
 *  @brief  Resizes the image data.
 *  @param  width  [in] resized width
 *  @param  height [in] resized height
 *  @param  interpolator [in] interpolation method
 */
/*===========================================================================*/
void GrayImage::resize( const size_t width, const size_t height, Interpolator interpolator )
{
    BaseClass::resizeImage( width, height, this, interpolator );
}

/*==========================================================================*/
/**
 *  Read a image file.
 *  @param filename [in] filename
 *  @return true, if the reading process is done successfully
 */
/*==========================================================================*/
bool GrayImage::read( const std::string& filename )
{
    // KVSML image.
    if ( kvs::KVSMLImageObject::CheckExtension( filename ) )
    {
        const kvs::KVSMLImageObject kvsml( filename );
        if ( kvsml.pixelType() == "color" )
        {
            kvs::ColorImage image( kvsml.width(), kvsml.height(), kvsml.pixels() );
            return( this->read_image( image ) );
        }
        if ( kvsml.pixelType() == "gray" )
        {
            const BaseClass::ImageType type = BaseClass::Gray;
            return BaseClass::create( kvsml.width(), kvsml.height(), type, kvsml.pixels() );
        }
    }

    // Bitmap, PNG and PPM image.
    if ( kvs::Bmp::CheckExtension( filename ) ||
         kvs::Png::CheckExtension( filename ) ||
         kvs::Ppm::CheckExtension( filename ) )
    {
        kvs::ColorImage image; image.read( filename );
        return this->read_image( image );
    }

    // PGM image.
    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        const kvs::Pgm pgm( filename );
        const BaseClass::ImageType type = BaseClass::Gray;
        return BaseClass::create( pgm.width(), pgm.height(), type, pgm.pixels() );
    }

    // PBM image.
    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        kvs::BitImage image; image.read( filename );
        return this->read_image( image );
    }

    // TIFF image.
    if ( kvs::Tiff::CheckExtension( filename ) )
    {
        const kvs::Tiff tiff( filename );
        if ( tiff.colorMode() == kvs::Tiff::Color24 )
        {
            const PixelData& data = tiff.rawData().asValueArray<kvs::UInt8>();
            kvs::ColorImage image( tiff.width(), tiff.height(), data );
            return this->read_image( image );
        }
        if ( tiff.colorMode() == kvs::Tiff::Gray8 )
        {
            const PixelData& data = tiff.rawData().asValueArray<kvs::UInt8>();
            const BaseClass::ImageType type = BaseClass::Gray;
            return BaseClass::create( tiff.width(), tiff.height(), type, data );
        }
        if ( tiff.colorMode() == kvs::Tiff::Gray16 )
        {
            kvsMessageError( "TIFF image (16bits gray-scale) is not supported." );
            return false;
        }
    }

    // DICOM image.
    if ( kvs::Dicom::CheckExtension( filename ) )
    {
        const kvs::Dicom dcm( filename );
        const BaseClass::ImageType type = BaseClass::Gray;
        return BaseClass::create( dcm.width(), dcm.height(), type, dcm.pixelData() );
    }

    kvsMessageError( "Read-method for %s is not implemented.",
                     filename.c_str() );

    return false;
}

/*==========================================================================*/
/**
 *  Write a image file.
 *  @param filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*==========================================================================*/
bool GrayImage::write( const std::string& filename ) const
{
    // KVSML image.
    if ( kvs::KVSMLImageObject::CheckExtension( filename ) )
    {
        kvs::KVSMLImageObject kvsml;
        kvsml.setWidth( BaseClass::width() );
        kvsml.setHeight( BaseClass::height() );
        kvsml.setPixelType( "gray" );
        kvsml.setWritingDataType( kvs::KVSMLImageObject::Ascii );
        kvsml.setPixels( BaseClass::pixels() );
        return kvsml.write( filename );
    }

    // Bitmap and PPM image.
    if ( kvs::Bmp::CheckExtension( filename ) ||
         kvs::Png::CheckExtension( filename ) ||
         kvs::Ppm::CheckExtension( filename ) )
    {
        kvs::ColorImage image( *this );
        return image.write( filename );
    }

    // PGM image.
    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        kvs::Pgm pgm( BaseClass::width(), BaseClass::height(), BaseClass::pixels() );
        return pgm.write( filename );
    }

    // PBM image.
    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        kvs::BitImage image( *this );
        return image.write( filename );
    }

    kvsMessageError( "Write-method for %s is not implemented.",
                     filename.c_str() );

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Reads the color image.
 *  @param  image [in] color image
 *  @return true, if the reading process is done successfully.
 */
/*===========================================================================*/
bool GrayImage::read_image( const kvs::ColorImage& image )
{
    if ( BaseClass::create( image.width(), image.height(), BaseClass::Gray ) )
    {
        auto method = GrayImage::MeanValue();
        method( image, BaseClass::pixelData() );
        return true;
    }

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Reads the bit image.
 *  @param  image [in] bit image
 *  @return true, if the reading process is done successfully.
 */
/*===========================================================================*/
bool GrayImage::read_image( const kvs::BitImage& image )
{
    if ( BaseClass::create( image.width(), image.height(), kvs::ImageBase::Gray ) )
    {
        auto* pixels = BaseClass::pixelData().data();
        const auto width = image.width();
        const auto height = image.height();
        size_t index = 0;
        for ( size_t j = 0; j < height; j++ )
        {
            for ( size_t i = 0; i < width; i++, index++ )
            {
                const kvs::UInt8 pixel = image.pixel( i, j ) ? 255 : 0;
                pixels[ index ] = pixel;
            }
        }
        return true;
    }

    return false;
}

} // end of namespace kvs
