/****************************************************************************/
/**
 *  @file ImageBase.h
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: ImageBase.h 1571 2013-05-09 14:49:50Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#ifndef KVS__IMAGE_BASE_H_INCLUDE
#define KVS__IMAGE_BASE_H_INCLUDE

#include <kvs/ValueArray>
#include <kvs/Type>
#include <kvs/Vector2>
#include <kvs/Deprecated>


namespace kvs
{

class ColorImage;
class GrayImage;

/*==========================================================================*/
/**
 *  Image base class.
 */
/*==========================================================================*/
class ImageBase
{
public:

    // Image type (number of components)
    enum ImageType
    {
        Bit = 0, ///< bit image
        Gray = 1, ///< gray image
        Color = 3  ///< color image
    };

public:

    template <typename ImageDataType> class NearestNeighborInterpolator;
    template <typename ImageDataType> class BilinearInterpolator;

    typedef NearestNeighborInterpolator<kvs::GrayImage> NearestNeighborInterpolatorGray;
    typedef NearestNeighborInterpolator<kvs::ColorImage> NearestNeighborInterpolatorColor;
    typedef BilinearInterpolator<kvs::GrayImage> BilinearInterpolatorGray;
    typedef BilinearInterpolator<kvs::ColorImage> BilinearInterpolatorColor;

private:

    size_t m_width; ///< image width [pix]
    size_t m_height; ///< image height [pix]
    size_t m_npixels; ///< number of pixels
    size_t m_padding; ///< padding bit for bit-image
    size_t m_bpp; ///< bits per pixel [bit]
    size_t m_bpl; ///< bytes per line [byte]
    size_t m_size; ///< data size [byte]
    kvs::ValueArray<kvs::UInt8> m_pixels; ///< pixel data array

public:

    ImageBase();
    virtual ~ImageBase();

    size_t width() const;
    size_t height() const;
    size_t bytesPerLine() const;
    size_t bitsPerPixel() const;
    size_t numberOfPixels() const;
    size_t padding() const;
    size_t size() const;
    const kvs::ValueArray<kvs::UInt8>& pixels() const;

    void flip();
    void copy( const kvs::ImageBase& image );
    bool create( const size_t width, const size_t height, const ImageType type );
    bool create( const size_t width, const size_t height, const ImageType type, const kvs::ValueArray<kvs::UInt8>& pixels );

protected:

    kvs::ValueArray<kvs::UInt8>& pixelData();
    template <typename ImageDataType, typename Interpolator>
    void resizeImage( const size_t width, const size_t height, ImageDataType* image );

public:
    KVS_DEPRECATED( const kvs::ValueArray<kvs::UInt8>& data() const ) { return this->pixels(); }
    KVS_DEPRECATED( kvs::ValueArray<kvs::UInt8>& data() ) { return this->pixelData(); }
};

template <typename ImageDataType>
class ImageBase::NearestNeighborInterpolator
{
protected:

    const ImageDataType* m_reference_image;
    kvs::Vector2d m_p;

public:

    void attach( const ImageDataType* image );
    void setU( const double u );
    void setV( const double v );
    const typename ImageDataType::PixelType operator () () const;
};

template <typename ImageDataType>
class ImageBase::BilinearInterpolator
{
protected:

    const ImageDataType* m_reference_image;
    kvs::Vector2d m_pmin;
    kvs::Vector2d m_pmax;
    kvs::Vector2d m_rate;

public:

    void attach( const ImageDataType* image );
    void setU( const double u );
    void setV( const double v );
    const typename ImageDataType::PixelType operator () () const;
};

} // end of namespace kvs

#endif // KVS__IMAGE_BASE_H_INCLUDE
