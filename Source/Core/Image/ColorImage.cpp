/****************************************************************************/
/**
 *  @file   ColorImage.cpp
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#include "ColorImage.h"
#include "GrayImage.h"
#include "BitImage.h"
#include <kvs/IgnoreUnusedVariable>
#include <kvs/KVSMLImageObject>
#include <kvs/RGBColor>
#include <kvs/File>
#include <kvs/Bmp>
#include <kvs/Png>
#include <kvs/Ppm>
#include <kvs/Pgm>
#include <kvs/Pbm>
#include <kvs/Png>
#include <kvs/Tiff>
#include <kvs/Dicom>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructs a new color image.
 *  @param width [in] image width
 *  @param height [in] image height
 */
/*==========================================================================*/
ColorImage::ColorImage( const size_t width, const size_t height )
{
    this->create( width, height );
}

/*==========================================================================*/
/**
 *  Constructs a new color image from the given pixel data.
 *  @param width [in] image width
 *  @param height [in] image height
 *  @param pixels [in] pixel data array
 */
/*==========================================================================*/
ColorImage::ColorImage(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::UInt8>& pixels )
{
    this->create( width, height, pixels );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a copy of other gray image.
 *  @param  image [in] gray image
 */
/*===========================================================================*/
ColorImage::ColorImage( const kvs::GrayImage& image )
{
    this->read_image( image );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a copy of other bit image.
 *  @param  image [in] bit image
 */
/*===========================================================================*/
ColorImage::ColorImage( const kvs::BitImage& image )
{
    this->read_image( image );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ColorImage class.
 *  @param  filename [in] filename
 */
/*===========================================================================*/
ColorImage::ColorImage( const std::string& filename )
{
    this->read( filename );
}

/*===========================================================================*/
/**
 *  @brief  Creates a color image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @return true if the create process is done successfully
 */
/*===========================================================================*/
bool ColorImage::create( const size_t width, const size_t height )
{
    return BaseClass::create( width, height, ImageBase::Color );
}

/*===========================================================================*/
/**
 *  @brief  Creates a color image.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] pixel data (rgb or rgba pixels)
 *  @return true if the create process is done successfully
 */
/*===========================================================================*/
bool ColorImage::create(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::UInt8>& pixels )
{
    if ( pixels.size() == width * height * 3 )
    {
        // RGB color
        return ImageBase::create( width, height, kvs::ImageBase::Color, pixels );
    }
    else if ( pixels.size() == width * height * 4 )
    {
        // RGBA color
        kvs::ValueArray<kvs::UInt8> rgb( width * height * 3 );
        for ( size_t i = 0; i < width * height; ++i )
        {
            rgb[ 3 * i + 0 ] = pixels[ 4 * i + 0 ];
            rgb[ 3 * i + 1 ] = pixels[ 4 * i + 1 ];
            rgb[ 3 * i + 2 ] = pixels[ 4 * i + 2 ];
        }
        return ImageBase::create( width, height, kvs::ImageBase::Color, rgb );
    }
    return false;
}

/*==========================================================================*/
/**
 *  Returns the pixel color.
 *  @param index [in] pixel index
 *  @return pixel color
 */
/*==========================================================================*/
kvs::RGBColor ColorImage::pixel( const size_t index ) const
{
    const size_t index3 = index * 3;
    const kvs::UInt8* pixels = BaseClass::pixels().data();
    return { pixels[index3], pixels[index3+1], pixels[index3+2] };
}

/*==========================================================================*/
/**
 *  Returns the pixel color.
 *  @param i [in] pixel index along the horizontal axis
 *  @param j [in] pixel index along the vertical axis
 *  @return pixel color
 */
/*==========================================================================*/
kvs::RGBColor ColorImage::pixel( const size_t i, const size_t j ) const
{
    const size_t index3 = ( BaseClass::width() * j + i ) * 3;
    const kvs::UInt8* pixels = BaseClass::pixels().data();
    return { pixels[index3], pixels[index3+1], pixels[index3+2] };
}

/*==========================================================================*/
/**
 *  Set the pixel color.
 *  @param index [in] pixel index
 *  @param pixel [in] pixel color
 */
/*==========================================================================*/
void ColorImage::setPixel( const size_t index, const kvs::RGBColor& pixel )
{
    const size_t index3 = index * 3;
    kvs::UInt8* pixels = BaseClass::pixelData().data();
    pixels[ index3 + 0 ] = pixel.r();
    pixels[ index3 + 1 ] = pixel.g();
    pixels[ index3 + 2 ] = pixel.b();
}

/*==========================================================================*/
/**
 *  Set the pixel color.
 *  @param i [in] pixel index along the horizontal axis
 *  @param j [in] pixel index along the vertical axis
 *  @param pixel [in] pixel color
 */
/*==========================================================================*/
void ColorImage::setPixel( const size_t i, const size_t j, const kvs::RGBColor& pixel )
{
    const size_t index3 = ( BaseClass::width() * j + i ) * 3;
    kvs::UInt8* pixels = BaseClass::pixelData().data();
    pixels[ index3 + 0 ] = pixel.r();
    pixels[ index3 + 1 ] = pixel.g();
    pixels[ index3 + 2 ] = pixel.b();
}

/*===========================================================================*/
/**
 *  @brief  Scales the image data.
 *  @param  ratio [in] scaling ratio
 *  @param  interpolator [in] interpolation method
 */
/*===========================================================================*/
void ColorImage::scale( const double ratio, Interpolator interpolator )
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
void ColorImage::resize( const size_t width, const size_t height, Interpolator interpolator )
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
bool ColorImage::read( const std::string& filename )
{
    // KVSML image.
    if ( kvs::KVSMLImageObject::CheckExtension( filename ) )
    {
        const kvs::KVSMLImageObject kvsml( filename );
        if ( kvsml.pixelType() == "color" )
        {
            const auto type = BaseClass::Color;
            return BaseClass::create( kvsml.width(), kvsml.height(), type, kvsml.pixels() );
        }
        if ( kvsml.pixelType() == "gray" )
        {
            kvs::GrayImage image( kvsml.width(), kvsml.height(), kvsml.pixels() );
            return this->read_image( image );
        }
    }

    // Bitmap image.
    if ( kvs::Bmp::CheckExtension( filename ) )
    {
        const kvs::Bmp bmp( filename );
        const auto type = BaseClass::Color;
        return BaseClass::create( bmp.width(), bmp.height(), type, bmp.pixels() );
    }

    // PPM image.
    if ( kvs::Ppm::CheckExtension( filename ) )
    {
        const kvs::Ppm ppm( filename );
        const auto type = BaseClass::Color;
        return BaseClass::create( ppm.width(), ppm.height(), type, ppm.pixels() );
    }

    // PGM image.
    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        kvs::GrayImage image; image.read( filename );
        return this->read_image( image );
    }

    // PBM image.
    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        kvs::BitImage image; image.read( filename );
        return this->read_image( image );
    }

    // PNG image.
    if ( kvs::Png::CheckExtension( filename ) )
    {
        const kvs::Png png( filename );
        if ( png.bytesPerPixel() == 1 )
        {
            kvs::GrayImage image( png.width(), png.height(), png.pixels() );
            return this->read_image( image );
        }
        else if ( png.bytesPerPixel() == 3 )
        {
            const auto type = BaseClass::Color;
            return BaseClass::create( png.width(), png.height(), type, png.pixels() );
        }
        else if ( png.bytesPerPixel() == 4 )
        {
            const size_t npixels = png.width() * png.height();
            kvs::ValueArray<kvs::UInt8> pixels( npixels * 3 );
            for ( size_t i = 0; i < npixels; ++i )
            {
                pixels[ 3 * i + 0 ] = png.pixels()[ 4 * i + 0 ];
                pixels[ 3 * i + 1 ] = png.pixels()[ 4 * i + 1 ];
                pixels[ 3 * i + 2 ] = png.pixels()[ 4 * i + 2 ];
            }
            const auto type = BaseClass::Color;
            return BaseClass::create( png.width(), png.height(), type, pixels );
        }
        else
        {
            kvsMessageError() << "PNG image (2-bpp) is not supported." << std::endl;
            return false;
        }
    }

    // TIFF image.
    if ( kvs::Tiff::CheckExtension( filename ) )
    {
        const kvs::Tiff tiff( filename );
        if ( tiff.colorMode() == kvs::Tiff::Color24 )
        {
            const auto& data = tiff.rawData().asValueArray<kvs::UInt8>();
            const auto type = BaseClass::Color;
            return BaseClass::create( tiff.width(), tiff.height(), type, data );
        }
        if ( tiff.colorMode() == kvs::Tiff::Gray8 )
        {
            const auto& data = tiff.rawData().asValueArray<kvs::UInt8>();
            kvs::GrayImage image( tiff.width(), tiff.height(), data );
            return this->read_image( image );
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
        kvs::GrayImage image( dcm.column(), dcm.row(), dcm.pixelData() );
        return this->read_image( image );
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
bool ColorImage::write( const std::string& filename ) const
{
    // KVSML image.
    if ( kvs::KVSMLImageObject::CheckExtension( filename ) )
    {
        kvs::KVSMLImageObject kvsml;
        kvsml.setWidth( BaseClass::width() );
        kvsml.setHeight( BaseClass::height() );
        kvsml.setPixelType( "color" );
        kvsml.setWritingDataType( kvs::KVSMLImageObject::Ascii );
        kvsml.setPixels( BaseClass::pixels() );
        return kvsml.write( filename );
    }

    // Bitmap image.
    if ( kvs::Bmp::CheckExtension( filename ) )
    {
        kvs::Bmp bmp( BaseClass::width(), BaseClass::height(), BaseClass::pixels() );
        return bmp.write( filename );
    }

    // PNG image.
    if ( kvs::Png::CheckExtension( filename ) )
    {
        kvs::Png png( BaseClass::width(), BaseClass::height(), BaseClass::pixels() );
        return png.write( filename );
    }

    // PPM image.
    if ( kvs::Ppm::CheckExtension( filename ) )
    {
        kvs::Ppm ppm( BaseClass::width(), BaseClass::height(), BaseClass::pixels() );
        return ppm.write( filename );
    }

    // PGM image.
    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        kvs::GrayImage image( *this );
        return image.write( filename );
    }

    // PBM image.
    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        kvs::BitImage image( kvs::GrayImage( *this ) );
        return image.write( filename );
    }

    kvsMessageError( "Write-method for %s is not implemented.",
                     filename.c_str() );

    return false;
}

/*===========================================================================*/
/**
 *  @brief  Reads the gray image.
 *  @param  image [in] gray image
 *  @return true, if the reading process is done successfully.
 */
/*===========================================================================*/
bool ColorImage::read_image( const kvs::GrayImage& image )
{
    if ( BaseClass::create( image.width(), image.height(), kvs::ImageBase::Color ) )
    {
        auto* pixels = BaseClass::pixelData().data();
        const auto* data = image.pixels().data();
        const auto npixels = BaseClass::numberOfPixels();
        for ( size_t index = 0, index3 = 0; index < npixels; index++, index3 += 3 )
        {
            const auto pixel = data[ index ];
            pixels[ index3 + 0 ] = pixel;
            pixels[ index3 + 1 ] = pixel;
            pixels[ index3 + 2 ] = pixel;
        }
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
bool ColorImage::read_image( const kvs::BitImage& image )
{
    if ( BaseClass::create( image.width(), image.height(), kvs::ImageBase::Color ) )
    {
        auto* pixels = BaseClass::pixelData().data();
        const auto width = image.width();
        const auto height = image.height();
        size_t index3 = 0;
        for ( size_t j = 0; j < height; j++ )
        {
            for ( size_t i = 0; i < width; i++, index3 += 3 )
            {
                const auto pixel = image.pixel( i, j ) ? 255 : 0;
                pixels[ index3 + 0 ] = pixel;
                pixels[ index3 + 1 ] = pixel;
                pixels[ index3 + 2 ] = pixel;
            }
        }
        return true;
    }

    return false;
}

} // end of namespace kvs
