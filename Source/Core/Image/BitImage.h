/****************************************************************************/
/**
 *  @file   BitImage.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <limits>
#include <functional>
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

    // Binarization method
    using BinarizationMethod = std::function<void(const kvs::GrayImage&, BaseClass::PixelData&)>;
    static BinarizationMethod PTile();
    static BinarizationMethod Distinction();
    static BinarizationMethod Byer();
    static BinarizationMethod Halftone();
    static BinarizationMethod EmphasizedHalftone();
    static BinarizationMethod Screw();
    static BinarizationMethod DeformedScrew();
    static BinarizationMethod DotConcentrate();

public:
    BitImage() = default;
    BitImage( const size_t width, const size_t height, const bool bit = true );
    BitImage( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& data );
    BitImage( const kvs::GrayImage& image, BinarizationMethod method = PTile() );
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

} // end of namespace kvs
