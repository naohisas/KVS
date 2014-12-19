/****************************************************************************/
/**
 *  @file RGBColor.cpp
 */
/*----------------------------------------------------------------------------
 *
 *  Copyright (c) Visualization Laboratory, Kyoto University.
 *  All rights reserved.
 *  See http://www.viz.media.kyoto-u.ac.jp/kvs/copyright/ for details.
 *
 *  $Id: RGBColor.cpp 1798 2014-08-04 05:25:10Z naohisa.sakamoto@gmail.com $
 */
/****************************************************************************/
#include <kvs/Math>
#include <kvs/Type>
#include <kvs/Assert>
#include "RGBColor.h"
#include "HSVColor.h"
#include "RGBAColor.h"


namespace kvs
{

RGBColor RGBColor::Black()
{
    return RGBColor( 0, 0, 0 );
}

RGBColor RGBColor::White()
{
    return RGBColor( 255, 255, 255 );
}

RGBColor RGBColor::Red()
{
    return RGBColor( 255, 0, 0 );
}

RGBColor RGBColor::Green()
{
    return RGBColor( 0, 255, 0 );
}

RGBColor RGBColor::Blue()
{
    return RGBColor( 0, 0, 255 );
}

RGBColor RGBColor::Yellow()
{
    return RGBColor( 255, 255, 0 );
}

RGBColor RGBColor::Cyan()
{
    return RGBColor( 0, 255, 255 );
}

RGBColor RGBColor::Magenta()
{
    return RGBColor( 255, 0, 255 );
}

RGBColor::RGBColor():
    m_red( 0 ),
    m_green( 0 ),
    m_blue( 0 )
{
}

RGBColor::RGBColor( kvs::UInt8 red, kvs::UInt8 green, kvs::UInt8 blue ):
    m_red( red ),
    m_green( green ),
    m_blue( blue )
{
}

RGBColor::RGBColor( const kvs::UInt8 rgb[3] ):
    m_red( rgb[0] ),
    m_green( rgb[1] ),
    m_blue( rgb[2] )
{
}

RGBColor::RGBColor( const RGBColor& rgb ):
    m_red( rgb.m_red ),
    m_green( rgb.m_green ),
    m_blue( rgb.m_blue )
{
}

RGBColor::RGBColor( const HSVColor& hsv )
{
    *this = hsv;
}

RGBColor::RGBColor( const kvs::Vec3& rgb )
{
    *this = rgb;
}

RGBColor& RGBColor::operator += ( const RGBColor& rgb )
{
    m_red   += rgb.m_red;
    m_green += rgb.m_green;
    m_blue  += rgb.m_blue;
    return *this;
}

RGBColor& RGBColor::operator -= ( const RGBColor& rgb )
{
    m_red   -= rgb.m_red;
    m_green -= rgb.m_green;
    m_blue  -= rgb.m_blue;
    return *this;
}

RGBColor& RGBColor::operator = ( const RGBColor& rgb )
{
    m_red   = rgb.m_red;
    m_green = rgb.m_green;
    m_blue  = rgb.m_blue;
    return *this;
}

/*==========================================================================*/
/**
 *  '=' operator.
 *  @param rgba [in] RGB-a color
 */
/*==========================================================================*/
RGBColor& RGBColor::operator = ( const RGBAColor& rgba )
{
    m_red   = rgba.r();
    m_green = rgba.g();
    m_blue  = rgba.b();

    return *this;
}

/*==========================================================================*/
/**
 *  '=' operator. ( convert HSV to RGB )
 *  @param hsv [in] HSV color
 */
/*==========================================================================*/
RGBColor& RGBColor::operator = ( const HSVColor& hsv )
{
    if( kvs::Math::IsZero( hsv.s() ) )
    {
        m_red   = static_cast<kvs::UInt8>( hsv.v() * 255 );
        m_green = static_cast<kvs::UInt8>( hsv.v() * 255 );
        m_blue  = static_cast<kvs::UInt8>( hsv.v() * 255 );
    }
    else
    {
        float h = ( hsv.h() < 1.0f ? hsv.h() : hsv.h() - 1.0f ) * 6.0f;
        int   i = int( h );

        float tmp1 = hsv.v() * ( 1 - hsv.s() );
        float tmp2 = hsv.v() * ( 1 - hsv.s() * ( h - i ) );
        float tmp3 = hsv.v() * ( 1 - hsv.s() * ( 1 - h + i ) );

        float tmp_r, tmp_g, tmp_b;
        switch( i )
        {
        case 0:
        {
            tmp_r = hsv.v();
            tmp_g = tmp3;
            tmp_b = tmp1;
            break;
        }
        case 1:
        {
            tmp_r = tmp2;
            tmp_g = hsv.v();
            tmp_b = tmp1;
            break;
        }
        case 2:
        {
            tmp_r = tmp1;
            tmp_g = hsv.v();
            tmp_b = tmp3;
            break;
        }
        case 3:
        {
            tmp_r = tmp1;
            tmp_g = tmp2;
            tmp_b = hsv.v();
            break;
        }
        case 4:
        {
            tmp_r = tmp3;
            tmp_g = tmp1;
            tmp_b = hsv.v();
            break;
        }
        default:
        {
            tmp_r = hsv.v();
            tmp_g = tmp1;
            tmp_b = tmp2;
            break;
        }
        }

        m_red   = static_cast<kvs::UInt8>( tmp_r * 255.0f + 0.5f );
        m_green = static_cast<kvs::UInt8>( tmp_g * 255.0f + 0.5f );
        m_blue  = static_cast<kvs::UInt8>( tmp_b * 255.0f + 0.5f );
    }

    return *this;
}

RGBColor& RGBColor::operator = ( const kvs::Vec3& rgb )
{
    KVS_ASSERT( 0.0f <= rgb.x() && rgb.x() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgb.y() && rgb.y() <= 1.0f );
    KVS_ASSERT( 0.0f <= rgb.z() && rgb.z() <= 1.0f );

    m_red = kvs::Math::Round( rgb.x() * 255.0f );
    m_green = kvs::Math::Round( rgb.y() * 255.0f );
    m_blue = kvs::Math::Round( rgb.z() * 255.0f );
    return *this;
}

void RGBColor::set( kvs::UInt8 red, kvs::UInt8 green, kvs::UInt8 blue )
{
    m_red = red;
    m_green = green;
    m_blue = blue;
}

kvs::Vec3 RGBColor::toVec3() const
{
    const float r = static_cast<float>( m_red ) / 255.0f;
    const float g = static_cast<float>( m_green ) / 255.0f;
    const float b = static_cast<float>( m_blue ) / 255.0f;
    return kvs::Vec3( r, g, b );
}

} // end of namespace kvs
