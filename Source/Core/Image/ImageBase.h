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
    using PixelData = kvs::ValueArray<kvs::UInt8>;

    template <typename ImageT> class NearestNeighborInterpolator;
    template <typename ImageT> class BilinearInterpolator;

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
    template <typename ImageDataType, typename Interpolator>
    void resizeImage( const size_t width, const size_t height, ImageDataType* image );

public:
    KVS_DEPRECATED( const kvs::ValueArray<kvs::UInt8>& data() const ) { return this->pixels(); }
    KVS_DEPRECATED( kvs::ValueArray<kvs::UInt8>& data() ) { return this->pixelData(); }
};

template <typename ImageT>
class ImageBase::NearestNeighborInterpolator
{
protected:
    const ImageT* m_reference_image = nullptr;
    kvs::Vec2d m_p{ 0.0, 0.0 };

public:
    void attach( const ImageT* image ) { m_reference_image = image; }
    void setU( const double u ) { m_p.x() = kvs::Math::Floor( u ); }
    void setV( const double v ) { m_p.y() = kvs::Math::Floor( v ); }
    const typename ImageT::PixelType operator () () const
    {
        const size_t x = static_cast<size_t>( m_p.x() );
        const size_t y = static_cast<size_t>( m_p.y() );
        return m_reference_image->pixel( x, y );
    }
};

template <typename ImageT>
class ImageBase::BilinearInterpolator
{
protected:
    const ImageT* m_reference_image = nullptr;
    kvs::Vec2d m_pmin{ 0.0, 0.0 };
    kvs::Vec2d m_pmax{ 0.0, 0.0 };
    kvs::Vec2d m_rate{ 0.0, 0.0 };

public:
    void attach( const ImageT* image ) { m_reference_image = image; }
    void setU( const double u )
    {
        const auto width = static_cast<double>( m_reference_image->width() - 1 );
        m_pmin.x() = kvs::Math::Floor( u );
        m_pmax.x() = m_pmin.x() + ( width > m_pmin.x() ? 1.0 : 0.0 );
        m_rate.x() = u - static_cast<double>( m_pmin.x() );
    }
    void setV( const double v )
    {
        const auto height = static_cast<double>( m_reference_image->height() - 1 );
        m_pmin.y() = kvs::Math::Floor( v );
        m_pmax.y() = m_pmin.y() + ( height > m_pmin.y() ? 1.0 : 0.0 );
        m_rate.y() = v - static_cast<double>( m_pmin.y() );
    }

    const typename ImageT::PixelType operator () () const
    {
        const auto pmin_x = static_cast<size_t>( m_pmin.x() );
        const auto pmin_y = static_cast<size_t>( m_pmin.y() );
        const auto pmax_x = static_cast<size_t>( m_pmax.x() );
        const auto pmax_y = static_cast<size_t>( m_pmax.y() );
        const auto p1 = m_reference_image->pixel( pmin_x, pmin_y );
        const auto p2 = m_reference_image->pixel( pmin_x, pmax_y );
        const auto p3 = m_reference_image->pixel( pmax_x, pmin_y );
        const auto p4 = m_reference_image->pixel( pmax_x, pmax_y );
        return this->lerp( p1, p2, p3, p4, m_rate.x(), m_rate.y() );
    }

private:
    kvs::UInt8 lerp(
        const kvs::UInt8 p1,
        const kvs::UInt8 p2,
        const kvs::UInt8 p3,
        const kvs::UInt8 p4,
        const double xrate,
        const double yrate ) const
    {
        const auto x0 = kvs::Math::Mix( static_cast<double>(p1), static_cast<double>(p3), xrate );
        const auto x1 = kvs::Math::Mix( static_cast<double>(p2), static_cast<double>(p4), xrate );
        auto y = kvs::Math::Round( kvs::Math::Mix( x0, x1, yrate ) );
        kvs::Math::Clamp( y, 0, 255 );
        return static_cast<kvs::UInt8>( y );
    }

    kvs::RGBColor lerp(
        const kvs::RGBColor p1,
        const kvs::RGBColor p2,
        const kvs::RGBColor p3,
        const kvs::RGBColor p4,
        const double xrate,
        const double yrate ) const
    {
        return {
            this->lerp( p1.r(), p2.r(), p3.r(), p4.r(), xrate, yrate ),
            this->lerp( p1.g(), p2.g(), p3.g(), p4.g(), xrate, yrate ),
            this->lerp( p1.b(), p2.b(), p3.b(), p4.b(), xrate, yrate )
        };
    }

};

} // end of namespace kvs
