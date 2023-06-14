/****************************************************************************/
/**
 *  @file   Background.h
 *  @author Naohisa Sakamoto
 */
/****************************************************************************/
#pragma once
#include <kvs/RGBColor>
#include <kvs/ColorImage>
#include <kvs/Texture2D>


namespace kvs
{

/*==========================================================================*/
/**
 *  Background class.
 */
/*==========================================================================*/
class Background
{
public:
    enum Type
    {
        MonoColor = 0, ///< mono color background
        TwoSideColor, ///< gradation color background using two colors
        FourCornersColor, ///< gradation color background using four colors
        Image ///< image background
    };

private:
    Type m_type = MonoColor; ///< background type
    bool m_image_changed = false; ///< flag for changing background image
    kvs::RGBColor m_color[4]; ///< color on the corners
    kvs::Real32 m_opacity = 0.0f; ///< opacity
    kvs::ColorImage m_image{}; ///< background image
    kvs::Texture2D m_texture{}; ///< background image texture

public:
    Background() = default;
    Background( const kvs::RGBColor& c );
    Background( const kvs::RGBColor& c0, const kvs::RGBColor& c1 );
    Background( const kvs::RGBColor& c0, const kvs::RGBColor& c1, const kvs::RGBColor& c2, const kvs::RGBColor& c3 );
    virtual ~Background() = default;

    Background& operator = ( const Background& bg );

    void setOpacity( const kvs::Real32 opacity ) { m_opacity = opacity; }
    void setColor( const kvs::RGBColor& c );
    void setColor( const kvs::RGBColor& c0, const kvs::RGBColor& c1 );
    void setColor( const kvs::RGBColor& c0, const kvs::RGBColor& c1, const kvs::RGBColor& c2, const kvs::RGBColor& c3 );
    void setImage( const kvs::ColorImage& image );

    const kvs::RGBColor& color( size_t index = 0 ) const;

    void apply();

private:
    void apply_mono_color();
    void apply_gradation_color();
    void apply_image();
};

} // end of namespace kvs
