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
#include <kvs/KVSMLImageObject>


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
    switch ( type )
    {
    case kvs::ImageObject::Gray8: return "8 bit gray pixel";
    case kvs::ImageObject::Gray16: return "16 bit gray pixel";
    case kvs::ImageObject::Color24: return "24 bit RGB color pixel (8x8x8 bits)";
    case kvs::ImageObject::Color32: return "32 bit RGBA color pixel (8x8x8x8 bits)";
    default: return "unknown";
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a pixel type from the pixel type name.
 *  @param  pixel_type_name [in] pixel type name
 *  @return pixel type
 */
/*===========================================================================*/
kvs::ImageObject::PixelType GetPixelType( const std::string& pixel_type_name )
{
    if ( pixel_type_name == "gray" )
    {
        return kvs::ImageObject::Gray8;
    }
    else if ( pixel_type_name == "color" )
    {
        return kvs::ImageObject::Color24;
    }
    else
    {
        return kvs::ImageObject::UnknownPixelType;
    }
}

/*===========================================================================*/
/**
 *  @brief  Returns a writing data type.
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return writing data type
 */
/*===========================================================================*/
kvs::KVSMLImageObject::WritingDataType GetWritingDataType( const bool ascii, const bool external )
{
    if ( ascii )
    {
        if ( external ) { return kvs::KVSMLImageObject::ExternalAscii; }
        else { return kvs::KVSMLImageObject::Ascii; }
    }
    else
    {
        return kvs::KVSMLImageObject::ExternalBinary;
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

/*===========================================================================*/
/**
 *  @brief  Read an image object from the specified file in KVSML.
 *  @param  filename [in] input filename
 *  @return true, if the reading process is done successfully
 */
/*===========================================================================*/
bool ImageObject::read( const std::string& filename )
{
    if ( !kvs::KVSMLImageObject::CheckExtension( filename ) )
    {
        kvsMessageError("%s is not an image object file in KVSML.", filename.c_str());
        return false;
    }

    kvs::KVSMLImageObject kvsml;
    if ( !kvsml.read( filename ) ) { return false; }

    this->setSize( kvsml.width(), kvsml.height() );
    this->setPixels( kvsml.pixels(), ::GetPixelType( kvsml.pixelType() ) );

    return true;
}

/*===========================================================================*/
/**
 *  @brief  Write the image object to the specfied file in KVSML.
 *  @param  filename [in] output filename
 *  @param  ascii [in] ascii (true = default) or binary (true)
 *  @param  external [in] external (true) or internal (false = default)
 *  @return true, if the writing process is done successfully
 */
/*===========================================================================*/
bool ImageObject::write( const std::string& filename, const bool ascii, const bool external ) const
{
    kvs::KVSMLImageObject kvsml;
    kvsml.setWritingDataType( ::GetWritingDataType( ascii, external ) );
    kvsml.setWidth( this->width() );
    kvsml.setHeight( this->height() );

    return kvsml.write( filename );
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
