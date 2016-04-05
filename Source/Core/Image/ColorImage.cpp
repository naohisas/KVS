/****************************************************************************/
/**
 *  @file ColorImage.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorImage.cpp 1571 2013-05-09 14:49:50Z naohisa.sakamoto@gmail.com $
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
#include <kvs/Ppm>
#include <kvs/Pgm>
#include <kvs/Pbm>
#include <kvs/Tiff>
#include <kvs/Dicom>


namespace kvs
{

/*==========================================================================*/
/**
 *  Constructs a new color image.
 */
/*==========================================================================*/
ColorImage::ColorImage()
{
}

/*==========================================================================*/
/**
 *  Constructs a new color image.
 *  @param width [in] image width
 *  @param height [in] image height
 */
/*==========================================================================*/
ColorImage::ColorImage( const size_t width, const size_t height )
{
    BaseClass::create( width, height, ImageBase::Color );
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
    ImageBase::create( width, height, kvs::ImageBase::Color, pixels );
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
    return( kvs::RGBColor( pixels[index3], pixels[index3+1], pixels[index3+2] ) );
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
    return( kvs::RGBColor( pixels[index3], pixels[index3+1], pixels[index3+2] ) );
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
 */
/*===========================================================================*/
void ColorImage::scale( const double ratio )
{
    const size_t width = static_cast<size_t>( BaseClass::width() * ratio );
    const size_t height = static_cast<size_t>( BaseClass::height() * ratio );
    BaseClass::resizeImage<ColorImage,ColorImage::Bilinear>( width, height, this );
}

/*===========================================================================*/
/**
 *  @brief  Scales the image data.
 *  @param  ratio [in] scaling ratio
 *  @param  method [in] Interpolation method
 */
/*===========================================================================*/
template <typename InterpolationMethod>
void ColorImage::scale( const double ratio, InterpolationMethod method )
{
    kvs::IgnoreUnusedVariable( method );

    const size_t width = static_cast<size_t>( BaseClass::width() * ratio );
    const size_t height = static_cast<size_t>( BaseClass::height() * ratio );
    BaseClass::resizeImage<ColorImage,InterpolationMethod>( width, height, this );
}

// Specialization.
template
void ColorImage::scale( const double ratio, ColorImage::NearestNeighbor method );

template
void ColorImage::scale( const double ratio, ColorImage::Bilinear method );

/*===========================================================================*/
/**
 *  @brief  Resizes the image data.
 *  @param  width  [in] resized width
 *  @param  height [in] resized height
 */
/*===========================================================================*/
void ColorImage::resize( const size_t width, const size_t height )
{
    BaseClass::resizeImage<ColorImage,ColorImage::Bilinear>( width, height, this );
}

/*===========================================================================*/
/**
 *  @brief  Resizes the image data.
 *  @param  width [in] resized width
 *  @param  height [in] resized height
 *  @param  method [in] Interpolation method
 */
/*===========================================================================*/
template <typename InterpolationMethod>
void ColorImage::resize( const size_t width, const size_t height, InterpolationMethod method )
{
    kvs::IgnoreUnusedVariable( method );

    BaseClass::resizeImage<ColorImage,InterpolationMethod>( width, height, this );
}

// Specialization.
template
void ColorImage::resize( const size_t width, const size_t height, ColorImage::NearestNeighbor method );

template
void ColorImage::resize( const size_t width, const size_t height, ColorImage::Bilinear method );

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
            const BaseClass::ImageType type = BaseClass::Color;
            return( BaseClass::create( kvsml.width(), kvsml.height(), type, kvsml.pixels() ) );
        }
        if ( kvsml.pixelType() == "gray" )
        {
            kvs::GrayImage image( kvsml.width(), kvsml.height(), kvsml.pixels() );
            return( this->read_image( image ) );
        }
    }

    // Bitmap image.
    if ( kvs::Bmp::CheckExtension( filename ) )
    {
        const kvs::Bmp bmp( filename );
        const BaseClass::ImageType type = BaseClass::Color;
        return( BaseClass::create( bmp.width(), bmp.height(), type, bmp.pixels() ) );
    }

    // PPM image.
    if ( kvs::Ppm::CheckExtension( filename ) )
    {
        const kvs::Ppm ppm( filename );
        const BaseClass::ImageType type = BaseClass::Color;
        return( BaseClass::create( ppm.width(), ppm.height(), type, ppm.pixels() ) );
    }

    // PGM image.
    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        kvs::GrayImage image; image.read( filename );
        return( this->read_image( image ) );
    }

    // PBM image.
    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        kvs::BitImage image; image.read( filename );
        return( this->read_image( image ) );
    }

    // TIFF image.
    if ( kvs::Tiff::CheckExtension( filename ) )
    {
        const kvs::Tiff tiff( filename );
        if ( tiff.colorMode() == kvs::Tiff::Color24 )
        {
            const kvs::ValueArray<kvs::UInt8>& data = tiff.rawData().asValueArray<kvs::UInt8>();
            const BaseClass::ImageType type = BaseClass::Color;
            return( BaseClass::create( tiff.width(), tiff.height(), type, data ) );
        }
        if ( tiff.colorMode() == kvs::Tiff::Gray8 )
        {
            const kvs::ValueArray<kvs::UInt8>& data = tiff.rawData().asValueArray<kvs::UInt8>();
            kvs::GrayImage image( tiff.width(), tiff.height(), data );
            return( this->read_image( image ) );
        }
        if ( tiff.colorMode() == kvs::Tiff::Gray16 )
        {
            kvsMessageError( "TIFF image (16bits gray-scale) is not supported." );
            return( false );
        }
    }

    // DICOM image.
    if ( kvs::Dicom::CheckExtension( filename ) )
    {
        const kvs::Dicom dcm( filename );
        kvs::GrayImage image( dcm.column(), dcm.row(), dcm.pixelData() );
        return( this->read_image( image ) );
    }

    kvsMessageError( "Read-method for %s is not implemented.",
                     filename.c_str() );

    return( false );
}

/*==========================================================================*/
/**
 *  Write a image file.
 *  @param filename [in] filename
 *  @return true, if the writing process is done successfully
 */
/*==========================================================================*/
bool ColorImage::write( const std::string& filename )
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
        return( kvsml.write( filename ) );
    }

    // Bitmap image.
    if ( kvs::Bmp::CheckExtension( filename ) )
    {
        kvs::Bmp bmp( BaseClass::width(), BaseClass::height(), BaseClass::pixels() );
        return( bmp.write( filename ) );
    }

    // PPM image.
    if ( kvs::Ppm::CheckExtension( filename ) )
    {
        kvs::Ppm ppm( BaseClass::width(), BaseClass::height(), BaseClass::pixels() );
        return( ppm.write( filename ) );
    }

    // PGM image.
    if ( kvs::Pgm::CheckExtension( filename ) )
    {
        kvs::GrayImage image( *this );
        return( image.write( filename ) );
    }

    // PBM image.
    if ( kvs::Pbm::CheckExtension( filename ) )
    {
        kvs::BitImage image( kvs::GrayImage( *this ) );
        return( image.write( filename ) );
    }

    kvsMessageError( "Write-method for %s is not implemented.",
                     filename.c_str() );

    return( false );
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
    if ( !BaseClass::create( image.width(), image.height(), kvs::ImageBase::Color ) )
    {
        return( false );
    }

    kvs::UInt8* pixels = BaseClass::pixelData().data();
    const kvs::UInt8* data = image.pixels().data();
    const size_t npixels = BaseClass::numberOfPixels();
    for ( size_t index = 0, index3 = 0; index < npixels; index++, index3 += 3 )
    {
        const kvs::UInt8 pixel = data[ index ];
        pixels[ index3 + 0 ] = pixel;
        pixels[ index3 + 1 ] = pixel;
        pixels[ index3 + 2 ] = pixel;
    }

    return( true );
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
    if ( !BaseClass::create( image.width(), image.height(), kvs::ImageBase::Color ) )
    {
        return( false );
    }

    kvs::UInt8* pixels = BaseClass::pixelData().data();
    const size_t width = image.width();
    const size_t height = image.height();
    size_t index3 = 0;
    for ( size_t j = 0; j < height; j++ )
    {
        for ( size_t i = 0; i < width; i++, index3 += 3 )
        {
            const kvs::UInt8 pixel = image.pixel( i, j ) ? 255 : 0;
            pixels[ index3 + 0 ] = pixel;
            pixels[ index3 + 1 ] = pixel;
            pixels[ index3 + 2 ] = pixel;
        }
    }

    return( true );
}

} // end of namespace kvs
