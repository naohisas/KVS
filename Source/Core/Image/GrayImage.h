/****************************************************************************/
/**
 *  @file GrayImage.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: GrayImage.h 1571 2013-05-09 14:49:50Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__GRAY_IMAGE_H_INCLUDE
#define KVS__GRAY_IMAGE_H_INCLUDE

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

    typedef kvs::ImageBase BaseClass;
    typedef kvs::UInt8 PixelType;

    typedef BaseClass::NearestNeighborInterpolatorGray NearestNeighbor;
    typedef BaseClass::BilinearInterpolatorGray Bilinear;

public:

    // Gray-scaling method.

    struct MeanValue
    {
        void operator () ( const kvs::ColorImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct MiddleValue
    {
        void operator () ( const kvs::ColorImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct MedianValue
    {
        void operator () ( const kvs::ColorImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct NTSCWeightedMeanValue
    {
        void operator () ( const kvs::ColorImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

    struct HDTVWeightedMeanValue
    {
        void operator () ( const kvs::ColorImage& image, kvs::ValueArray<kvs::UInt8>& data );
    };

public:

    GrayImage();
    GrayImage( const size_t width, const size_t height );
    GrayImage( const size_t width, const size_t height, const kvs::ValueArray<kvs::UInt8>& data );
    explicit GrayImage( const kvs::BitImage& image );
    explicit GrayImage( const kvs::ColorImage& image );
    template <typename GrayScalingMethod>
    GrayImage( const kvs::ColorImage& image, GrayScalingMethod method );
    explicit GrayImage( const std::string& filename );

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
    bool write( const std::string& filename );

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

#endif // KVS__GRAY_IMAGE_H_INCLUDE
