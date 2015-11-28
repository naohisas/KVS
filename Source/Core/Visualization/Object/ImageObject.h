/*****************************************************************************/
/**
 *  @file   ImageObject.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageObject.h 1775 2014-05-16 13:49:42Z naohisa.sakamoto@gmail.com $
 */
/*****************************************************************************/
#pragma once
#include <ostream>
#include <kvs/ObjectBase>
#include <kvs/ClassName>
#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Module>
#include <kvs/Indent>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Image object class.
 */
/*===========================================================================*/
class ImageObject : public kvs::ObjectBase
{
    kvsModule( kvs::ImageObject, Object );
    kvsModuleBaseClass( kvs::ObjectBase );

public:

    enum PixelType
    {
        Gray8   =  8, ///< 8 bit gray pixel
        Gray16  = 16, ///< 16 bit gray pixel
        Color24 = 24, ///< 24 bit RGB color pixel (8x8x8 bits)
        Color32 = 32, ///< 32 bit RGBA color pixel (8x8x8x8 bits)
        UnknownPixelType
    };

private:

    ImageObject::PixelType m_type; ///< pixel type
    size_t m_width; ///< image width
    size_t m_height; ///< image height
    kvs::ValueArray<kvs::UInt8> m_pixels; ///< pixel data

public:

    ImageObject();
    ImageObject(
        const size_t width,
        const size_t height,
        const kvs::ValueArray<kvs::UInt8>& pixels,
        const PixelType type = Color24 );

    void shallowCopy( const ImageObject& object );
    void deepCopy( const ImageObject& object );
    void print( std::ostream& os, const kvs::Indent& indent = kvs::Indent(0) ) const;
    bool read( const std::string& filename );
    bool write( const std::string& filename, const bool ascii = true, const bool external = false ) const;

    PixelType pixelType() const { return m_type; }
    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    const kvs::ValueArray<kvs::UInt8>& pixels() const { return m_pixels; }
    size_t bitsPerPixel() const { return m_type; }
    size_t bytesPerPixel() const { return m_type >> 3; }
    size_t numberOfChannels() const;

    void setSize( const size_t width, const size_t height ) { m_width = width; m_height = height; }
    void setPixels( const kvs::ValueArray<kvs::UInt8>& pixels, const PixelType type = Color24 ) { m_pixels = pixels; m_type = type; }

public:
    KVS_DEPRECATED( const kvs::ValueArray<kvs::UInt8>& data() const ) { return this->pixels(); }
    KVS_DEPRECATED( size_t nchannels() const ) { return this->numberOfChannels(); }
    KVS_DEPRECATED( friend std::ostream& operator << ( std::ostream& os, const ImageObject& object ) );
    KVS_DEPRECATED( PixelType type() const ) { return this->pixelType(); }
};

} // end of namespace kvs
