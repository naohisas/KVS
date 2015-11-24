/****************************************************************************/
/**
 *  @file   Background.h
 *  @author Naohisa Sakamoto
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: Background.h 1571 2013-05-09 14:49:50Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#pragma once

#include <kvs/RGBAColor>
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

protected:

    Type m_type;///< background type
    bool m_image_changed; ///< flag for changing background image
    kvs::RGBAColor m_color[4]; ///< color on the corners
    kvs::ColorImage m_image; ///< background image
    kvs::Texture2D m_texture; ///< background image texture

public:

    Background();
    Background( const kvs::RGBAColor& c );
    Background( const kvs::RGBAColor& c0, const kvs::RGBAColor& c1 );
    Background( const kvs::RGBAColor& c0, const kvs::RGBAColor& c1, const kvs::RGBAColor& c2, const kvs::RGBAColor& c3 );
    virtual ~Background();

    Background& operator = ( const Background& bg );

    void setColor( const kvs::RGBAColor& c );
    void setColor( const kvs::RGBAColor& c0, const kvs::RGBAColor& c1 );
    void setColor( const kvs::RGBAColor& c0, const kvs::RGBAColor& c1, const kvs::RGBAColor& c2, const kvs::RGBAColor& c3 );
    void setImage( const kvs::ColorImage& image );

    const kvs::RGBColor& color( size_t index = 0 ) const;

    void apply();

private:

    void apply_mono_color();
    void apply_gradation_color();
    void apply_image();
};

} // end of namespace kvs
