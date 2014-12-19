/****************************************************************************/
/**
 *  @file ColorImage.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ColorImage.h 1339 2012-11-07 07:23:28Z s.yamada0808@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__COLOR_IMAGE_H_INCLUDE
#define KVS__COLOR_IMAGE_H_INCLUDE

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
    bool write( const std::string& filename );

private:

    bool read_image( const kvs::GrayImage& image );
    bool read_image( const kvs::BitImage& image );
};

} // end of namespace kvs

#endif // KVS__COLOR_IMAGE_H_INCLUDE
