/****************************************************************************/
/**
 *  @file HSVColor.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: HSVColor.cpp 1316 2012-09-24 10:40:16Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include "HSVColor.h"
#include "RGBColor.h"
#include <kvs/Math>


namespace kvs
{

HSVColor::HSVColor( float hue, float saturation, float value ):
    m_hue( hue ),
    m_saturation( saturation ),
    m_value( value )
{
}

HSVColor::HSVColor( const HSVColor& hsv ):
    m_hue( hsv.m_hue ),
    m_saturation( hsv.m_saturation ),
    m_value( hsv.m_value )
{
}

HSVColor::HSVColor( const RGBColor& rgb )
{
    *this = rgb;
}

HSVColor& HSVColor::operator += ( const HSVColor& hsv )
{
    m_hue += hsv.m_hue;
    m_saturation += hsv.m_saturation;
    m_value += hsv.m_value;
    return( *this );
}

HSVColor& HSVColor::operator -= ( const HSVColor& hsv )
{
    m_hue -= hsv.m_hue;
    m_saturation -= hsv.m_saturation;
    m_value -= hsv.m_value;
    return( *this );
}

HSVColor& HSVColor::operator = ( const HSVColor& hsv )
{
    m_hue = hsv.m_hue;
    m_saturation = hsv.m_saturation;
    m_value = hsv.m_value;
    return( *this );
}

/*==========================================================================*/
/**
 *  '=' operator. ( convert RGB to HSV )
 *
 *  @param rgb [in] RGB color
 */
/*==========================================================================*/
HSVColor& HSVColor::operator = ( const RGBColor& rgb )
{
    float R = float( rgb.r() / 255.0f );
    float G = float( rgb.g() / 255.0f );
    float B = float( rgb.b() / 255.0f );

    float min_rgb = kvs::Math::Min( R, G, B );
    float max_rgb = kvs::Math::Max( R, G, B );

    float delta = max_rgb - min_rgb;

    m_value = max_rgb;

    if( kvs::Math::IsZero( delta ) )
    {
        m_hue = 0.0f;
        m_saturation = 0.0f;
    }
    else
    {
        m_saturation = delta / max_rgb;

        float delta_r = ( ( ( max_rgb - R ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
        float delta_g = ( ( ( max_rgb - G ) / 6.0f ) + ( delta / 2.0f ) ) / delta;
        float delta_b = ( ( ( max_rgb - B ) / 6.0f ) + ( delta / 2.0f ) ) / delta;

        if( kvs::Math::Equal( R, max_rgb ) ) m_hue = delta_b - delta_g;
        else if( kvs::Math::Equal( G, max_rgb ) ) m_hue = ( 1.0f / 3.0f ) + delta_r - delta_b;
        else if( kvs::Math::Equal( B, max_rgb ) ) m_hue = ( 2.0f / 3.0f ) + delta_g - delta_r;

        if( m_hue < 0 ) m_hue += 1;
        if( m_hue > 1 ) m_hue -= 1;
    }

    return( *this );
}

void HSVColor::set( float hue, float saturation, float value )
{
    m_hue        = hue;
    m_saturation = saturation;
    m_value      = value;
}

float HSVColor::h() const
{
    return( m_hue );
}

float HSVColor::hue() const
{
    return( m_hue );
}

float HSVColor::s() const
{
    return( m_saturation );
}

float HSVColor::saturation() const
{
    return( m_saturation );
}

float HSVColor::v() const
{
    return( m_value );
}

float HSVColor::value() const
{
    return( m_value );
}

float HSVColor::intensity() const
{
    return( m_value );
}

} // end of namespace kvs
