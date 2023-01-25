/****************************************************************************/
/**
 *  @file   GrayImage.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <limits>
#include "ImageBase.h"
#include "ColorImage.h"


namespace kvs
{

class RGBColor;
class BitImage;

/*==========================================================================*/
/**
 *  Gray image class.
 */
/*==========================================================================*/
class GrayImage : public kvs::ImageBase
{
public:
    using BaseClass = kvs::ImageBase;
    using PixelType = kvs::UInt8;

    // Interpolator
    using Interpolator = BaseClass::GrayInterpolator;
    static Interpolator Nearest() { return BaseClass::GrayNearest; }
    static Interpolator Bilinear() { return BaseClass::GrayBilinear; }

    // Gray-scaling method
    using GrayScalingMethod = std::function<void(const kvs::ColorImage&, BaseClass::PixelData&)>;
    static GrayScalingMethod MeanValue();
    static GrayScalingMethod MiddleValue();
    static GrayScalingMethod MedianValue();
    static GrayScalingMethod NTSCWeightedMeanValue();
    static GrayScalingMethod HDTVWeightedMeanValue();

public:
    GrayImage() = default;
    GrayImage( const size_t width, const size_t height );
    GrayImage( const size_t width, const size_t height, const PixelData& data );
    GrayImage( const size_t width, const size_t height, const PixelData& data, const int channel );
    GrayImage( const size_t width, const size_t height, const kvs::ValueArray<kvs::Real32>& data );
    GrayImage( const kvs::ColorImage& image, const int channel );
    GrayImage( const kvs::ColorImage& image, GrayScalingMethod method = MedianValue() );
    explicit GrayImage( const kvs::BitImage& image );
    explicit GrayImage( const std::string& filename );

    bool create( const size_t width, const size_t height );
    bool create( const size_t width, const size_t height, const PixelData& pixels );
    bool create( const size_t width, const size_t height, const PixelData& pixels, const int channel );
    bool create( const size_t width, const size_t height, const kvs::ValueArray<kvs::Real32>& data );

    kvs::UInt8 pixel( const size_t index ) const;
    kvs::UInt8 pixel( const size_t i, const size_t j ) const;

    void setPixel( const size_t index, const kvs::UInt8 pixel );
    void setPixel( const size_t i, const size_t j, const kvs::UInt8 pixel );

    void scale( const double ratio, Interpolator interpolator = Bilinear() );
    void resize( const size_t width, const size_t height, Interpolator interpolator = Bilinear() );
    bool read( const std::string& filename );
    bool write( const std::string& filename ) const;

private:
    bool read_image( const kvs::ColorImage& image );
    bool read_image( const kvs::BitImage& image );
};

} // end of namespace kvs
