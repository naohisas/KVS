/****************************************************************************/
/**
 *  @file   ImageBase.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
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

    using NearestNeighborInterpolatorGray = NearestNeighborInterpolator<kvs::GrayImage>;
    using NearestNeighborInterpolatorColor = NearestNeighborInterpolator<kvs::ColorImage>;
    using BilinearInterpolatorGray = BilinearInterpolator<kvs::GrayImage>;
    using BilinearInterpolatorColor = BilinearInterpolator<kvs::ColorImage>;
    using PixelData = kvs::ValueArray<kvs::UInt8>;

private:
    size_t m_width; ///< image width [pix]
    size_t m_height; ///< image height [pix]
    size_t m_npixels; ///< number of pixels
    size_t m_padding; ///< padding bit for bit-image
    size_t m_bpp; ///< bits per pixel [bit]
    size_t m_bpl; ///< bytes per line [byte]
    size_t m_size; ///< data size [byte]
    PixelData m_pixels; ///< pixel data array

public:
    ImageBase();
    virtual ~ImageBase();

    size_t width() const { return m_width; }
    size_t height() const { return m_height; }
    size_t bytesPerLine() const { return m_bpl; }
    size_t bitsPerPixel() const { return m_bpp; }
    size_t numberOfPixels() const { return m_npixels; }
    size_t padding() const { return m_padding; }
    size_t size() const { return m_size; }
    const PixelData& pixels() const { return m_pixels; }

    void flip();
    void copy( const kvs::ImageBase& image );
    bool create( const size_t width, const size_t height, const ImageType type );
    bool create( const size_t width, const size_t height, const ImageType type, const PixelData& pixels );

protected:
    PixelData& pixelData() { return m_pixels; }
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
