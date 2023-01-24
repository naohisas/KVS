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
    using NearestNeighbor = BaseClass::NearestNeighborInterpolator<GrayImage>;
    using Bilinear = BaseClass::BilinearInterpolator<GrayImage>;

public:
    // Gray-scaling method.

    struct MeanValue
    {
        void operator () ( const kvs::ColorImage& image, PixelData& data );
    };

    struct MiddleValue
    {
        void operator () ( const kvs::ColorImage& image, PixelData& data );
    };

    struct MedianValue
    {
        void operator () ( const kvs::ColorImage& image, PixelData& data );
    };

    struct NTSCWeightedMeanValue
    {
        void operator () ( const kvs::ColorImage& image, PixelData& data );
    };

    struct HDTVWeightedMeanValue
    {
        void operator () ( const kvs::ColorImage& image, PixelData& data );
    };

public:
    GrayImage();
    GrayImage( const size_t width, const size_t height );
    GrayImage( const size_t width, const size_t height, const PixelData& data );
    GrayImage( const size_t width, const size_t height, const PixelData& data, const int channel );
    GrayImage( const size_t width, const size_t height, const kvs::ValueArray<kvs::Real32>& data );
    explicit GrayImage( const kvs::BitImage& image );
    explicit GrayImage( const kvs::ColorImage& image );
    GrayImage( const kvs::ColorImage& image, const int channel );
    template <typename GrayScalingMethod>
    GrayImage( const kvs::ColorImage& image, GrayScalingMethod method );
    explicit GrayImage( const std::string& filename );

    bool create( const size_t width, const size_t height );
    bool create( const size_t width, const size_t height, const PixelData& pixels );
    bool create( const size_t width, const size_t height, const PixelData& pixels, const int channel );
    bool create( const size_t width, const size_t height, const kvs::ValueArray<kvs::Real32>& data );

    kvs::UInt8 pixel( const size_t index ) const;
    kvs::UInt8 pixel( const size_t i, const size_t j ) const;

    void setPixel( const size_t index, const kvs::UInt8 pixel );
    void setPixel( const size_t i, const size_t j, const kvs::UInt8 pixel );

    void scale( const double ratio );
    template <typename InterpolationMethod>
    void scale( const double ratio, InterpolationMethod method );
    void resize( const size_t width, const size_t height );
    template <typename InterpolationMethod>
    void resize( const size_t width, const size_t height, InterpolationMethod method );
    bool read( const std::string& filename );
    bool write( const std::string& filename ) const;

private:
    bool read_image( const kvs::ColorImage& image );
    bool read_image( const kvs::BitImage& image );
};

/*===========================================================================*/
/**
 *  @brief  Constructs a new gray-scale image from the color image.
 *  @param  image [in] color image
 *  @param  method [in] gray scaling method
 */
/*===========================================================================*/
template <typename GrayScalingMethod>
inline GrayImage::GrayImage( const kvs::ColorImage& image, GrayScalingMethod method )
{
    BaseClass::create( image.width(), image.height(), kvs::ImageBase::Gray );
    method( image, BaseClass::pixelData() );
}

} // end of namespace kvs
