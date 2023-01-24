/****************************************************************************/
/**
 *  @file   BitImage.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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
    using BaseClass = kvs::ImageBase;
    using PixelType = bool;

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

    bool create( const size_t width, const size_t height, const bool bit = true );
    bool create( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels );

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
