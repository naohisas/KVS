/****************************************************************************/
/**
 *  @file BitImage.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: BitImage.h 1571 2013-05-09 14:49:50Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__BIT_IMAGE_H_INCLUDE
#define KVS__BIT_IMAGE_H_INCLUDE

#include <limits>
#include "ImageBase.h"
#include "GrayImage.h"


namespace kvs
{

class RGBColor;
class ColorImage;

/*==========================================================================*/
/**
 *  Bit image class.
 */
/*==========================================================================*/
class BitImage : public kvs::ImageBase
{
public:

    typedef kvs::ImageBase BaseClass;
    typedef bool PixelType;

public:

    // Binarization method.

    struct PTile
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct Distinction
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct Byer
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct Halftone
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct EmphasizedHalftone
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct Screw
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct DeformedScrew
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct DotConcentrate
    {
        void operator () ( const kvs::GrayImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

public:

    BitImage();
    BitImage( const size_t width, const size_t height, const bool bit = true );
    BitImage( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& data );
    explicit BitImage( const kvs::GrayImage& image );
    template <typename BinarizationMethod>
    BitImage( const kvs::GrayImage& image, BinarizationMethod method );
    explicit BitImage( const std::string& filename );

    bool pixel( const size_t index ) const;
    bool pixel( const size_t i, const size_t j ) const;

    void setPixel( const size_t index, const bool pixel );
    void setPixel( const size_t i, const size_t j, const bool pixel );

    size_t count() const;
    void fill( const bool bit );
    void invert( const size_t index );
    void invert( const size_t i, const size_t j );
    void invert();
    bool read( const std::string& filename );
    bool write( const std::string& filename );

private:

    void set_bit( const size_t i, const size_t j );
    void reset_bit( const size_t i, const size_t j );
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new bit image from the color image.
 *  @param  image [in] gray image
 *  @param  method [in] binarization method
 */
/*===========================================================================*/
template <typename BinarizationMethod>
inline BitImage::BitImage( const kvs::GrayImage& image, BinarizationMethod method )
{
    BaseClass::create( image.width(), image.height(), kvs::ImageBase::Bit );
    method( image, BaseClass::pixelData() );
}

} // end of namespace kvs

#endif // KVS__BIT_IMAGE_H_INCLUDE
