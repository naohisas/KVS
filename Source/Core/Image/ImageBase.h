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
#include <kvs/Math>
#include <kvs/RGBColor>
#include <kvs/Deprecated>
#include <functional>


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

    using PixelData = kvs::ValueArray<kvs::UInt8>;

protected:
    using GrayInterpolator = std::function<kvs::UInt8(double,double,const GrayImage&)>;
    using ColorInterpolator = std::function<kvs::RGBColor(double,double,const ColorImage&)>;
    static kvs::UInt8 GrayNearest( double u, double v, const GrayImage& image );
    static kvs::UInt8 GrayBilinear( double u, double v, const GrayImage& image );
    static kvs::RGBColor ColorNearest( double u, double v, const ColorImage& image );
    static kvs::RGBColor ColorBilinear( double u, double v, const ColorImage& image );

private:
    size_t m_width = 0; ///< image width [pix]
    size_t m_height = 0; ///< image height [pix]
    size_t m_npixels = 0; ///< number of pixels
    size_t m_padding = 0; ///< padding bit for bit-image
    size_t m_bpp = 0; ///< bits per pixel [bit]
    size_t m_bpl = 0; ///< bytes per line [byte]
    size_t m_size = 0; ///< data size [byte]
    PixelData m_pixels{}; ///< pixel data array

public:
    ImageBase() = default;
    virtual ~ImageBase() = default;

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

    template <typename Image, typename Interpolator>
    void resizeImage(
        const size_t width,
        const size_t height,
        Image* image,
        Interpolator interpolator );

public:
    KVS_DEPRECATED( const kvs::ValueArray<kvs::UInt8>& data() const ) { return this->pixels(); }
    KVS_DEPRECATED( kvs::ValueArray<kvs::UInt8>& data() ) { return this->pixelData(); }
};

} // end of namespace kvs
