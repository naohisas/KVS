/*****************************************************************************/
/**
 *  @file   ImageObject.cpp
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageObject.cpp 1775 2014-05-16 13:49:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#include "ImageObject.h"
#include <string>


namespace
{

/*===========================================================================*/
/**
 *  @brief  Returns the pixel type name.
 *  @param  type [in] pixel type
 *  @return pixel type name
 */
/*===========================================================================*/
const std::string GetPixelTypeName( const kvs::ImageObject::PixelType type )
{
    switch( type )
    {
    case kvs::ImageObject::Gray8: return "8 bit gray pixel";
    case kvs::ImageObject::Gray16: return "16 bit gray pixel";
    case kvs::ImageObject::Color24: return "24 bit RGB color pixel (8x8x8 bits)";
    case kvs::ImageObject::Color32: return "32 bit RGBA color pixel (8x8x8x8 bits)";
    default: return "unknown pixel type";
    }
}

} // end of namespace


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ImageObject class.
 */
/*===========================================================================*/
ImageObject::ImageObject()
{
    BaseClass::setObjectType( Image );
}

/*===========================================================================*/
/**
 *  @brief  Constructs a new ImageObject class.
 *  @param  width [in] image width
 *  @param  height [in] image height
 *  @param  pixels [in] pixel data array
 *  @param  type [in] color type
 */
/*==========================================================================*/
ImageObject::ImageObject(
    const size_t width,
    const size_t height,
    const kvs::ValueArray<kvs::UInt8>& pixels,
    const ImageObject::PixelType type ):
    m_type( type ),
    m_width( width ),
    m_height( height ),
    m_pixels( pixels )
{
}

/*===========================================================================*/
/**
 *  @brief  Shallow copy.
 *  @param  other [in] image object
 */
/*===========================================================================*/
void ImageObject::shallowCopy( const ImageObject& other )
{
    BaseClass::operator=( other );
    this->m_type = other.pixelType();
    this->m_width = other.width();
    this->m_height = other.height();
    this->m_pixels = other.pixels();
}

/*===========================================================================*/
/**
 *  @brief  Deep copy.
 *  @param  other [in] image object
 */
/*===========================================================================*/
void ImageObject::deepCopy( const ImageObject& other )
{
    BaseClass::operator=( other );
    this->m_type = other.pixelType();
    this->m_width = other.width();
    this->m_height = other.height();
    this->m_pixels = other.pixels().clone();
}

/*===========================================================================*/
/**
 *  @brief  Prints information of the image object.
 *  @param  os [in] output stream
 *  @param  indent [in] indent
 */
/*===========================================================================*/
void ImageObject::print( std::ostream& os, const kvs::Indent& indent ) const
{
    os << indent << "Object type : " << "image object" << std::endl;
    os << indent << "Width : " << this->width() << std::endl;
    os << indent << "Height : " << this->height() << std::endl;
    os << indent << "Bits per pixel : " << this->bitsPerPixel() << std::endl;
    os << indent << "Bytes per pixel : " << this->bytesPerPixel() << std::endl;
    os << indent << "Pixel type : " << ::GetPixelTypeName( this->pixelType() ) << std::endl;
}

/*==========================================================================*/
/**
 *  @brief  Returns the number of color channels.
 *  @return number of color channels
 */
/*==========================================================================*/
size_t ImageObject::numberOfChannels() const
{
    size_t ret = 0;
    switch ( m_type )
    {
    case ImageObject::Gray8:   ret = 1; break;
    case ImageObject::Gray16:  ret = 1; break;
    case ImageObject::Color24: ret = 3; break;
    case ImageObject::Color32: ret = 4; break;
    default: break;
    }

    return ret;
}

/*===========================================================================*/
/**
 *  @brief  '<<' operator.
 */
/*===========================================================================*/
std::ostream& operator << ( std::ostream& os, const ImageObject& object )
{
    os << "Object type:  " << "image object" << std::endl;
    os << "Width:  " << object.width() << std::endl;
    os << "Height:  " << object.height() << std::endl;
    os << "Bits per pixel:  " << object.bitsPerPixel() << std::endl;
    os << "Bytes per pixel:  " << object.bytesPerPixel() << std::endl;
    os << "Pixel type:  " << ::GetPixelTypeName( object.pixelType() );

    return os;
}

} // end of namespace kvs
