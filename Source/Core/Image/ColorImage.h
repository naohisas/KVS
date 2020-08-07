/****************************************************************************/
/**
 *  @file   ColorImage.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <limits>
#include "ImageBase.h"


namespace kvs
{

class RGBColor;
class GrayImage;
class BitImage;

/*==========================================================================*/
/**
 *  Color image class.
 */
/*==========================================================================*/
class ColorImage : public kvs::ImageBase
{
public:
    typedef kvs::ImageBase BaseClass;
    typedef kvs::RGBColor PixelType;
    typedef BaseClass::NearestNeighborInterpolatorColor NearestNeighbor;
    typedef BaseClass::BilinearInterpolatorColor Bilinear;

public:
    ColorImage();
    ColorImage( const size_t width, const size_t height );
    ColorImage( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels );
    explicit ColorImage( const kvs::GrayImage& image );
    explicit ColorImage( const kvs::BitImage& image );
    explicit ColorImage( const std::string& filename );

    bool create( const size_t width, const size_t height );
    bool create( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& pixels );

    kvs::RGBColor pixel( const size_t index ) const;
    kvs::RGBColor pixel( const size_t i, const size_t j ) const;

    void setPixel( const size_t index, const kvs::RGBColor& pixel );
    void setPixel( const size_t i, const size_t j, const kvs::RGBColor& pixel );

    void scale( const double ratio );
    template <typename InterpolationMethod>
    void scale( const double ratio, InterpolationMethod method );
    void resize( const size_t width, const size_t height );
    template <typename InterpolationMethod>
    void resize( const size_t width, const size_t height, InterpolationMethod method );
    bool read( const std::string& filename );
    bool write( const std::string& filename ) const;

private:
    bool read_image( const kvs::GrayImage& image );
    bool read_image( const kvs::BitImage& image );
};

} // end of namespace kvs
